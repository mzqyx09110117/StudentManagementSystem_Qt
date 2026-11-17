#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""索引性能对比：在同一份 20 万条成绩数据上，比较有无索引的查询耗时。

输出：
  docs/assets/index_benchmark.png
  docs/assets/index_benchmark.json
"""
import json
import os
import random
import sqlite3
import statistics
import tempfile
import time
from datetime import datetime, timedelta
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent.parent
SCHEMA = (ROOT / "sql" / "schema.sql").read_text(encoding="utf-8")
ASSETS = ROOT / "docs" / "assets"
ASSETS.mkdir(parents=True, exist_ok=True)

STUDENTS = 5000
SUBJECTS = 20
GRADES_PER_STUDENT = 40  # 20 门课 × 2 个学期

QUERIES = [
    (
        "按课程+学期统计人数",
        "SELECT COUNT(*) FROM grades WHERE subject_id = ? AND semester = ?",
        ("S001", "2024-2025-1"),
    ),
    (
        "按课程统计人数",
        "SELECT COUNT(*) FROM grades WHERE subject_id = ?",
        ("S001",),
    ),
    (
        "按班级筛选学生",
        "SELECT COUNT(*) FROM students WHERE class_name = ?",
        ("人工智能01班",),
    ),
    (
        "审计日志最近 50 条",
        "SELECT id FROM audit_log ORDER BY log_time DESC LIMIT 50",
        (),
    ),
]


def build_database(path):
    con = sqlite3.connect(path)
    con.execute("PRAGMA foreign_keys = ON")
    con.executescript(SCHEMA)

    con.executemany(
        "INSERT INTO users(user_id, username, password_hash, user_type) VALUES(?,?,?,?)",
        [(t, t, "x", "teacher") for t in ("teacher_001", "teacher_002")],
    )
    con.executemany(
        "INSERT INTO teachers(teacher_id, name) VALUES(?,?)",
        [("teacher_001", "张伟"), ("teacher_002", "李娜")],
    )
    con.executemany(
        "INSERT INTO classes(class_name, grade_year) VALUES(?,?)",
        [(f"人工智能{i:02d}班", 2024) for i in range(1, 11)],
    )
    con.executemany(
        "INSERT INTO subjects(subject_id, subject_name, credit, teacher_id) VALUES(?,?,?,?)",
        [
            (f"S{i:03d}", f"课程{i}", 2.0 + (i % 4), "teacher_001" if i % 2 else "teacher_002")
            for i in range(1, SUBJECTS + 1)
        ],
    )

    students = []
    for i in range(STUDENTS):
        students.append(
            (
                f"2024{i + 1:04d}",
                f"学生{i + 1}",
                "男" if i % 2 == 0 else "女",
                18 + (i % 4),
                f"人工智能{(i % 10) + 1:02d}班",
            )
        )
    con.executemany(
        "INSERT INTO students(student_id, name, gender, age, class_name) VALUES(?,?,?,?,?)",
        students,
    )

    random.seed(42)
    semesters = ("2024-2025-1", "2024-2025-2")
    grades = []
    for sid, _name, _gender, _age, _cls in students:
        for subject_index in range(1, SUBJECTS + 1):
            ability = 60 + (int(sid[-2:]) % 30)
            for semester in semesters:
                score = max(35, min(100, round(ability + random.gauss(0, 8))))
                grades.append(
                    (sid, f"S{subject_index:03d}", semester, score,
                     "2025-01-10" if semester.endswith("1") else "2025-06-20",
                     "teacher_001" if subject_index % 2 else "teacher_002")
                )
    con.executemany(
        "INSERT INTO grades(student_id, subject_id, semester, score, exam_date, entered_by) "
        "VALUES(?,?,?,?,?,?)",
        grades,
    )

    base_time = datetime(2025, 1, 1, 8, 0, 0)
    audit_rows = [
        ("grades", "INSERT", f"2024{i:05d}/S001/2024-2025-1", "teacher_001",
         (base_time + timedelta(seconds=i * 37)).strftime("%Y-%m-%d %H:%M:%S"))
        for i in range(50000)
    ]
    con.executemany(
        "INSERT INTO audit_log(table_name, operation, record_key, operator, log_time) "
        "VALUES(?,?,?,?,?)",
        audit_rows,
    )
    con.commit()
    return con, len(grades)


def benchmark(con, sql, params, repeat=25):
    cur = con.cursor()
    cur.execute(sql, params)
    cur.fetchall()
    timings = []
    for _ in range(repeat):
        start = time.perf_counter()
        cur.execute(sql, params)
        cur.fetchall()
        timings.append((time.perf_counter() - start) * 1000.0)
    return statistics.median(timings)


def explain(con, sql, params):
    cur = con.cursor()
    cur.execute("EXPLAIN QUERY PLAN " + sql, params)
    return [row[-1] for row in cur.fetchall()]


def main():
    db_path = os.path.join(tempfile.gettempdir(), "sms_index_benchmark.db")
    try:
        os.remove(db_path)
    except FileNotFoundError:
        pass

    con, grade_count = build_database(db_path)
    con.execute("ANALYZE")

    with_index = []
    plans_with = []
    for name, sql, params in QUERIES:
        with_index.append(benchmark(con, sql, params))
        plans_with.append(explain(con, sql, params))

    indexes = [
        "idx_grades_subject_sem",
        "idx_grades_student_sem",
        "idx_students_class",
        "idx_students_name",
        "idx_grades_entered_by",
        "idx_subjects_teacher",
        "idx_subjects_name",
        "idx_audit_time",
        "idx_audit_operator",
        "idx_audit_table",
    ]
    for index in indexes:
        con.execute(f"DROP INDEX IF EXISTS {index}")
    con.execute("ANALYZE")
    con.commit()

    without_index = []
    plans_without = []
    for name, sql, params in QUERIES:
        without_index.append(benchmark(con, sql, params))
        plans_without.append(explain(con, sql, params))

    con.close()
    try:
        os.remove(db_path)
    except OSError:
        pass

    result = {
        "students": STUDENTS,
        "subjects": SUBJECTS,
        "grades": grade_count,
        "audit_rows": 50000,
        "note": "学生编号查询由 UNIQUE(student_id,subject_id,semester) 自动索引覆盖，"
                "无需额外索引，因此不列入对比。",
        "queries": [],
    }
    for i, (name, _sql, _params) in enumerate(QUERIES):
        speedup = (without_index[i] / with_index[i]) if with_index[i] else 0
        result["queries"].append(
            {
                "name": name,
                "with_index_ms": round(with_index[i], 3),
                "without_index_ms": round(without_index[i], 3),
                "speedup": round(speedup, 1),
                "plan_with_index": plans_with[i],
                "plan_without_index": plans_without[i],
            }
        )

    (ASSETS / "index_benchmark.json").write_text(
        json.dumps(result, ensure_ascii=False, indent=2), encoding="utf-8"
    )

    labels = [q[0] for q in QUERIES]
    x = range(len(labels))
    width = 0.36

    plt.rcParams["font.sans-serif"] = ["Microsoft YaHei", "SimHei"]
    plt.rcParams["axes.unicode_minus"] = False
    fig, ax = plt.subplots(figsize=(8.6, 4.6), dpi=200)
    bars1 = ax.bar([i - width / 2 for i in x], with_index, width,
                   label="有显式索引", color="#2563eb")
    bars2 = ax.bar([i + width / 2 for i in x], without_index, width,
                   label="删除显式索引", color="#94a3b8")
    ax.set_yscale("log")
    ax.set_ylabel("查询耗时（毫秒，对数轴）")
    ax.set_title(f"显式索引优化前后查询耗时对比（{grade_count:,} 条成绩）")
    ax.set_xticks(list(x))
    ax.set_xticklabels(labels)
    ax.legend()
    ax.grid(axis="y", alpha=0.3, which="both")
    for bars in (bars1, bars2):
        for bar in bars:
            height = bar.get_height()
            ax.annotate(f"{height:.2f}",
                        xy=(bar.get_x() + bar.get_width() / 2, height),
                        xytext=(0, 4), textcoords="offset points",
                        ha="center", va="bottom", fontsize=7)
    fig.tight_layout()
    fig.savefig(ASSETS / "index_benchmark.png")
    plt.close(fig)

    print(json.dumps(result, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
