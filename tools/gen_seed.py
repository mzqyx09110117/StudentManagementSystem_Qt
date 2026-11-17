#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""生成可重复的演示数据 sql/seed.sql（随机种子固定为 2026）。

账号（users/teachers）由应用程序在首次运行时用加盐哈希创建，
本脚本只生成班级、学生、课程、成绩等教学数据。
"""
import random
from pathlib import Path

random.seed(2026)

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "sql" / "seed.sql"

CLASSES = [
    ("人工智能2401", 2024, "张伟"),
    ("人工智能2402", 2024, "李娜"),
    ("人工智能2403", 2024, "张伟"),
    ("数据科学2401", 2024, "王芳"),   # 空班级，用于演示空班级可持久化
]

NAMES = [
    "王小明", "李思涵", "张雨轩", "刘子墨", "陈欣怡", "杨浩然",
    "赵梓涵", "黄诗琪", "周俊杰", "吴梦瑶", "徐嘉豪", "孙语桐",
    "胡宇轩", "朱雨欣", "高梓睿", "林诗涵", "何俊宇", "郭欣妍",
    "马天佑", "罗佳琪", "梁子涵", "宋欣然", "郑浩宇", "谢雨桐",
    "韩明轩", "唐雅静", "冯思远", "邓紫涵", "曹宇航", "彭静怡",
]

# (subject_id, name, credit, teacher_id)
SUBJECTS = [
    ("S001", "高等数学", 5.0, "teacher_001"),
    ("S002", "大学英语", 3.0, "teacher_001"),
    ("S003", "程序设计基础", 4.0, "teacher_001"),
    ("S004", "数据结构", 4.0, "teacher_001"),
    ("S005", "线性代数", 3.0, "teacher_002"),
    ("S006", "大学物理", 4.0, "teacher_002"),
    ("S007", "数据库原理", 4.0, "teacher_002"),
    ("S008", "人工智能导论", 3.0, "teacher_002"),
]

SEMESTERS = [("2024-2025-1", "2025-01-08"), ("2024-2025-2", "2025-06-18")]

DIFFICULTY = {
    "S001": -2, "S002": 2, "S003": 1, "S004": -3,
    "S005": -1, "S006": -2, "S007": 2, "S008": 3,
}


def sql_quote(value: str) -> str:
    return "'" + value.replace("'", "''") + "'"


def clamp(value: int, low: int = 35, high: int = 100) -> int:
    return max(low, min(high, value))


def main() -> None:
    lines = []
    lines.append("-- ============================================================")
    lines.append("-- 学生信息管理系统 演示数据（由 tools/gen_seed.py 生成）")
    lines.append("-- 依赖：schema.sql 已执行，且 teachers 表已存在 teacher_001/teacher_002")
    lines.append("-- 数据均为虚构，仅用于课程演示")
    lines.append("-- ============================================================")
    lines.append("")
    lines.append("-- 班级（含一个空班级，验证空班级可持久化）")
    for name, year, advisor in CLASSES:
        lines.append(
            "INSERT OR IGNORE INTO classes (class_name, grade_year, advisor, created_time) "
            "VALUES (%s, %d, %s, '2024-08-20 09:00:00');"
            % (sql_quote(name), year, sql_quote(advisor))
        )
    lines.append("")

    lines.append("-- 学生：2024001-2024030，其中 2024030 暂未分班")
    students = []
    for idx in range(30):
        student_id = f"2024{idx + 1:03d}"
        if idx < 10:
            class_name = "人工智能2401"
        elif idx < 20:
            class_name = "人工智能2402"
        elif idx < 29:
            class_name = "人工智能2403"
        else:
            class_name = None
        name = NAMES[idx]
        gender = "男" if idx % 2 == 0 else "女"
        age = 18 + (idx % 4)
        phone = f"138{10000000 + idx * 137:08d}"
        address = f"示例省示例市大学路{idx + 1}号"
        status = "休学" if student_id == "2024030" else "在读"
        students.append({"id": student_id, "name": name, "class": class_name})
        lines.append(
            "INSERT OR IGNORE INTO students "
            "(student_id, name, gender, age, class_name, phone, address, status, created_time) "
            "VALUES (%s, %s, %s, %d, %s, %s, %s, %s, '2024-09-01 09:00:00');"
            % (
                sql_quote(student_id),
                sql_quote(name),
                sql_quote(gender),
                age,
                sql_quote(class_name) if class_name else "NULL",
                sql_quote(phone),
                sql_quote(address),
                sql_quote(status),
            )
        )
    lines.append("")

    lines.append("-- 课程（分属两位教师）")
    for subject_id, name, credit, teacher_id in SUBJECTS:
        lines.append(
            "INSERT OR IGNORE INTO subjects "
            "(subject_id, subject_name, credit, teacher_id, description, created_time) VALUES "
            "(%s, %s, %.1f, %s, %s, '2024-08-25 10:00:00');"
            % (
                sql_quote(subject_id),
                sql_quote(name),
                credit,
                sql_quote(teacher_id),
                sql_quote(f"{name}课程，由教师 {teacher_id} 授课"),
            )
        )
    lines.append("")

    lines.append("-- 两个学期的成绩：30 名学生 × 8 门课程 × 2 个学期 = 480 条")
    for index, student in enumerate(students):
        ability = random.randint(62, 90)
        for subject_id, _name, _credit, teacher_id in SUBJECTS:
            for semester_index, (semester, exam_date) in enumerate(SEMESTERS):
                gain = 3 if semester_index == 1 else 0
                noise = random.gauss(0, 7)
                score = clamp(round(ability + DIFFICULTY[subject_id] + gain + noise))
                day_offset = int(subject_id[1:]) % 7
                date = exam_date[:-2] + f"{int(exam_date[-2:]) + day_offset:02d}"
                lines.append(
                    "INSERT OR IGNORE INTO grades "
                    "(student_id, subject_id, semester, score, exam_date, entered_by) VALUES "
                    "(%s, %s, %s, %d, %s, %s);"
                    % (
                        sql_quote(student["id"]),
                        sql_quote(subject_id),
                        sql_quote(semester),
                        score,
                        sql_quote(date),
                        sql_quote(teacher_id),
                    )
                )
    lines.append("")
    lines.append("UPDATE schema_meta SET seed_version = 1 WHERE id = 1;")
    lines.append("")

    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text("\n".join(lines), encoding="utf-8")
    print(f"生成 {OUT}：{len(lines)} 行")


if __name__ == "__main__":
    main()
