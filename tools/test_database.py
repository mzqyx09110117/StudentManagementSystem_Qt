#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""数据库层自动化测试：在内存库中执行 schema.sql + seed.sql，逐项验证
约束、触发器、视图和事务相关的关键行为。

运行：python -I tools/test_database.py
"""
import sqlite3
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SCHEMA = (ROOT / "sql" / "schema.sql").read_text(encoding="utf-8")
SEED = (ROOT / "sql" / "seed.sql").read_text(encoding="utf-8")

passed = 0
failed = 0


def check(name, condition, detail=""):
    global passed, failed
    if condition:
        passed += 1
        print(f"  PASS  {name}")
    else:
        failed += 1
        print(f"  FAIL  {name}  {detail}")


def expect_error(name, sql, params=()):
    global passed, failed
    try:
        conn.execute(sql, params)
        conn.commit()
    except sqlite3.DatabaseError as exc:
        passed += 1
        print(f"  PASS  {name}  ({type(exc).__name__}: {str(exc)[:60]})")
        conn.rollback()
        return
    failed += 1
    print(f"  FAIL  {name}  （本应失败却成功）")
    conn.rollback()


def scalar(sql, params=()):
    return conn.execute(sql, params).fetchone()[0]


conn = sqlite3.connect(":memory:")
conn.execute("PRAGMA foreign_keys = ON")
conn.executescript(SCHEMA)

# 种子数据依赖两名教师账号
for teacher_id, name in [("teacher_001", "张伟"), ("teacher_002", "李娜")]:
    conn.execute(
        "INSERT INTO users(user_id, username, password_hash, user_type) VALUES(?,?,?,?)",
        (teacher_id, teacher_id, "test", "teacher"),
    )
    conn.execute(
        "INSERT INTO teachers(teacher_id, name) VALUES(?,?)", (teacher_id, name)
    )
conn.commit()
conn.executescript(SEED)

print("== 基础数据 ==")
check("完整性检查", scalar("PRAGMA integrity_check") == "ok")
check("外键检查无冲突", conn.execute("PRAGMA foreign_key_check").fetchone() is None)
check("班级数 = 4", scalar("SELECT COUNT(*) FROM classes") == 4)
check("学生数 = 30", scalar("SELECT COUNT(*) FROM students") == 30)
check("课程数 = 8", scalar("SELECT COUNT(*) FROM subjects") == 8)
check("成绩数 = 480", scalar("SELECT COUNT(*) FROM grades") == 480)
check("空班级存在", scalar("SELECT COUNT(*) FROM classes WHERE class_name='数据科学2401'") == 1)
check("未分班学生存在", scalar("SELECT COUNT(*) FROM students WHERE class_name IS NULL") == 1)

print("== CHECK / 触发器 ==")
expect_error(
    "成绩 101 被拒绝",
    "INSERT INTO grades(student_id,subject_id,semester,score) VALUES('2024001','S001','2025-2026-1',101)",
)
expect_error(
    "负分被拒绝",
    "INSERT INTO grades(student_id,subject_id,semester,score) VALUES('2024001','S001','2025-2026-1',-1)",
)
expect_error(
    "非法学期格式被拒绝",
    "INSERT INTO grades(student_id,subject_id,semester,score) VALUES('2024001','S001','2025-1',88)",
)

print("== 唯一约束与 UPSERT ==")
expect_error(
    "重复成绩被拒绝",
    "INSERT INTO grades(student_id,subject_id,semester,score) VALUES('2024001','S001','2024-2025-1',90)",
)
conn.execute("UPDATE app_context SET current_user='teacher_001' WHERE id=1")
before = scalar("SELECT COUNT(*) FROM audit_log")
conn.execute(
    "INSERT INTO grades(student_id,subject_id,semester,score,entered_by) VALUES(?,?,?,?,?) "
    "ON CONFLICT(student_id,subject_id,semester) DO UPDATE SET score=excluded.score, "
    "updated_at=datetime('now','localtime')",
    ("2024001", "S001", "2025-2026-1", 91, "teacher_001"),
)
conn.commit()
check("UPSERT 新增成功", scalar("SELECT score FROM grades WHERE student_id='2024001' AND subject_id='S001' AND semester='2025-2026-1'") == 91)
check("审计日志 INSERT 记录", scalar("SELECT COUNT(*) FROM audit_log") == before + 1)
check(
    "审计操作者正确",
    scalar("SELECT operator FROM audit_log ORDER BY id DESC LIMIT 1") == "teacher_001",
)
conn.execute(
    "INSERT INTO grades(student_id,subject_id,semester,score) VALUES('2024001','S001','2025-2026-1',95) "
    "ON CONFLICT(student_id,subject_id,semester) DO UPDATE SET score=excluded.score"
)
conn.commit()
check("UPSERT 更新成功", scalar("SELECT score FROM grades WHERE student_id='2024001' AND subject_id='S001' AND semester='2025-2026-1'") == 95)
check("审计日志 UPDATE 记录", scalar("SELECT COUNT(*) FROM audit_log") == before + 2)
check(
    "审计保存旧值/新值",
    scalar("SELECT old_value LIKE '%91%' AND new_value LIKE '%95%' FROM audit_log ORDER BY id DESC LIMIT 1") == 1,
)

print("== 外键级联 ==")
grades_before = scalar("SELECT COUNT(*) FROM grades WHERE student_id='2024002'")
conn.execute("DELETE FROM students WHERE student_id='2024002'")
conn.commit()
check(
    "删除学生级联删除成绩",
    scalar("SELECT COUNT(*) FROM grades WHERE student_id='2024002'") == 0,
    f"删除前 {grades_before} 条",
)
check(
    "删除学生后无孤儿成绩",
    scalar("SELECT COUNT(*) FROM grades g LEFT JOIN students s ON s.student_id=g.student_id WHERE s.student_id IS NULL") == 0,
)
expect_error("删除有成绩的课程被 RESTRICT", "DELETE FROM subjects WHERE subject_id='S002'")
conn.execute("INSERT INTO subjects(subject_id,subject_name,credit,teacher_id) VALUES('S999','临时课程',2.0,'teacher_001')")
conn.commit()
conn.execute("DELETE FROM subjects WHERE subject_id='S999'")
conn.commit()
check("删除无成绩课程成功", scalar("SELECT COUNT(*) FROM subjects WHERE subject_id='S999'") == 0)

print("== 视图 ==")
check("成绩明细视图行数", scalar("SELECT COUNT(*) FROM v_student_grades") == scalar("SELECT COUNT(*) FROM grades"))
stats_rows = scalar("SELECT COUNT(*) FROM v_class_subject_stats")
# 48 个原始分组 + UPSERT 新增的 2024001/S001/2025-2026-1 分组
check("班级统计视图行数 = 49", stats_rows == 49, f"实际 {stats_rows}")
top_gpa = scalar("SELECT MAX(gpa) FROM v_student_overview")
check("GPA 在 0-4 之间", 0.0 <= top_gpa <= 4.0, f"实际 {top_gpa}")
check(
    "班级排名从 1 开始",
    scalar("SELECT MIN(class_rank) FROM v_student_overview WHERE class_name IS NOT NULL") == 1,
)
check("教师工作量视图 2 行", scalar("SELECT COUNT(*) FROM v_teacher_workload") == 2)
check("审计展示视图可查询", scalar("SELECT COUNT(*) FROM v_audit_display") >= 2)

print("== 班级级联 ==")
# 此前已删除 2024002，人工智能2401 还剩 9 人
conn.execute("UPDATE classes SET class_name='人工智能2401A' WHERE class_name='人工智能2401'")
conn.commit()
check(
    "班级改名级联学生",
    scalar("SELECT COUNT(*) FROM students WHERE class_name='人工智能2401A'") == 9,
)
conn.execute("DELETE FROM classes WHERE class_name='人工智能2401A'")
conn.commit()
check(
    "删除班级学生置 NULL",
    scalar("SELECT COUNT(*) FROM students WHERE class_name IS NULL") == 10,
)

print("== 管理员保护 ==")
conn.execute("INSERT INTO users(user_id,username,password_hash,user_type) VALUES('admin_001','admin','x','admin')")
conn.commit()
expect_error("不能插入第二个管理员", "INSERT INTO users(user_id,username,password_hash,user_type) VALUES('admin_002','admin2','x','admin')")
expect_error("不能删除最后一个管理员", "DELETE FROM users WHERE user_id='admin_001'")
expect_error(
    "不能把最后一个管理员改成教师",
    "UPDATE users SET user_type='teacher' WHERE user_id='admin_001'",
)

print("== 事务回滚 ==")
before_classes = scalar("SELECT COUNT(*) FROM classes")
try:
    conn.execute("BEGIN")
    conn.execute("INSERT INTO classes(class_name, grade_year) VALUES('回滚测试班', 2024)")
    conn.execute("INSERT INTO classes(class_name, grade_year) VALUES('回滚测试班', 2024)")  # 冲突
    conn.commit()
except sqlite3.DatabaseError:
    conn.rollback()
check(
    "事务回滚后无残留",
    scalar("SELECT COUNT(*) FROM classes") == before_classes
    and scalar("SELECT COUNT(*) FROM classes WHERE class_name='回滚测试班'") == 0,
)

print("== 最终检查 ==")
check("最终完整性", scalar("PRAGMA integrity_check") == "ok")
check("最终外键检查", conn.execute("PRAGMA foreign_key_check").fetchone() is None)

conn.close()
print()
print(f"结果：{passed} 通过，{failed} 失败")
raise SystemExit(0 if failed == 0 else 1)
