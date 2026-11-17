#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""使用 Graphviz 生成作品说明书中的结构图与流程图。"""
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "docs" / "assets"
OUT.mkdir(parents=True, exist_ok=True)
DOT = Path("D:/Graphviz/bin/dot.exe")
DOT_DIR = Path("D:/Graphviz/bin")
ENGINES = {"schema_compact": "neato", "modules": "neato", "er_compact": "dot"}
EXTRA_ARGS = {"schema_compact": ["-n"], "modules": ["-n"]}

COMMON = """
graph [fontname="Microsoft YaHei", fontsize=11, bgcolor="white", pad="0.3", nodesep="0.45", ranksep="0.55"];
node [fontname="Microsoft YaHei", fontsize=11, color="#334155", fontcolor="#0f172a"];
edge [fontname="Microsoft YaHei", fontsize=10, color="#475569", fontcolor="#334155"];
"""

DIAGRAMS = {
    "er_global": f"""
digraph ER {{
  graph [rankdir=LR, splines=true];
  {COMMON}
  node [shape=box, style="rounded,filled", fillcolor="#dbeafe", height=0.7];
  user [label="用户\\nusers"];
  teacher [label="教师\\nteachers"];
  klass [label="班级\\nclasses"];
  student [label="学生\\nstudents"];
  subject [label="课程\\nsubjects"];
  admin [label="管理员", fillcolor="#e0e7ff"];

  node [shape=diamond, style="filled", fillcolor="#fef3c7", height=0.9, width=1.2];
  has_account [label="拥有账号"];
  teach [label="讲授"];
  belongs [label="属于"];
  grade_rel [label="取得成绩"];

  user -> has_account [label="1"];
  has_account -> teacher [label="1"];
  has_account -> student [label="1"];
  admin -> has_account [style=dashed, label="角色"];
  teacher -> teach [label="1"];
  teach -> subject [label="N"];
  klass -> belongs [label="1"];
  belongs -> student [label="N"];
  student -> grade_rel [label="M"];
  grade_rel -> subject [label="N"];

  note [shape=note, style="filled", fillcolor="#f8fafc", label="“取得成绩”联系属性：学期、分数、考试日期、录入教师\\n实体属性详见数据字典与关系模式"];
  grade_rel -> note [style=dashed, arrowhead=none];
}}
""",
    "schema": f"""
digraph Schema {{
  graph [rankdir=LR, splines=ortho];
  {COMMON}
  node [shape=plain, fontsize=10];

  users [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#eff6ff">
    <TR><TD COLSPAN="2" BGCOLOR="#2563eb"><FONT COLOR="white"><B>users 用户</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>user_id</B> PK</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">username</TD><TD ALIGN="LEFT">TEXT UNIQUE</TD></TR>
    <TR><TD ALIGN="LEFT">password_hash / salt</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">user_type</TD><TD ALIGN="LEFT">admin/teacher/student</TD></TR>
    <TR><TD ALIGN="LEFT">密保问题 / 答案哈希</TD><TD ALIGN="LEFT">TEXT</TD></TR>
  </TABLE>>];

  teachers [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#f0fdf4">
    <TR><TD COLSPAN="2" BGCOLOR="#16a34a"><FONT COLOR="white"><B>teachers 教师</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>teacher_id</B> PK/FK</TD><TD ALIGN="LEFT">users.user_id</TD></TR>
    <TR><TD ALIGN="LEFT">name / title / phone</TD><TD ALIGN="LEFT">TEXT</TD></TR>
  </TABLE>>];

  classes [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#fefce8">
    <TR><TD COLSPAN="2" BGCOLOR="#ca8a04"><FONT COLOR="white"><B>classes 班级</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>class_name</B> PK</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">grade_year</TD><TD ALIGN="LEFT">INTEGER</TD></TR>
    <TR><TD ALIGN="LEFT">advisor</TD><TD ALIGN="LEFT">TEXT</TD></TR>
  </TABLE>>];

  students [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#fdf2f8">
    <TR><TD COLSPAN="2" BGCOLOR="#db2777"><FONT COLOR="white"><B>students 学生</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>student_id</B> PK</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">name / gender / age</TD><TD ALIGN="LEFT">TEXT/INTEGER</TD></TR>
    <TR><TD ALIGN="LEFT">class_name</TD><TD ALIGN="LEFT">FK → classes</TD></TR>
    <TR><TD ALIGN="LEFT">user_id</TD><TD ALIGN="LEFT">FK → users</TD></TR>
    <TR><TD ALIGN="LEFT">status / phone / address</TD><TD ALIGN="LEFT">TEXT</TD></TR>
  </TABLE>>];

  subjects [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#fff7ed">
    <TR><TD COLSPAN="2" BGCOLOR="#ea580c"><FONT COLOR="white"><B>subjects 课程</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>subject_id</B> PK</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">subject_name</TD><TD ALIGN="LEFT">UNIQUE(teacher_id,name)</TD></TR>
    <TR><TD ALIGN="LEFT">credit</TD><TD ALIGN="LEFT">REAL</TD></TR>
    <TR><TD ALIGN="LEFT">teacher_id</TD><TD ALIGN="LEFT">FK → teachers</TD></TR>
  </TABLE>>];

  grades [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#f5f3ff">
    <TR><TD COLSPAN="2" BGCOLOR="#7c3aed"><FONT COLOR="white"><B>grades 成绩</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>id</B> PK</TD><TD ALIGN="LEFT">INTEGER</TD></TR>
    <TR><TD ALIGN="LEFT">student_id</TD><TD ALIGN="LEFT">FK → students</TD></TR>
    <TR><TD ALIGN="LEFT">subject_id</TD><TD ALIGN="LEFT">FK → subjects</TD></TR>
    <TR><TD ALIGN="LEFT">semester / score</TD><TD ALIGN="LEFT">TEXT / REAL</TD></TR>
    <TR><TD ALIGN="LEFT">entered_by</TD><TD ALIGN="LEFT">FK → teachers</TD></TR>
    <TR><TD ALIGN="LEFT">UNIQUE(student,subject,semester)</TD><TD ALIGN="LEFT"></TD></TR>
  </TABLE>>];

  audit [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="4" BGCOLOR="#f8fafc">
    <TR><TD COLSPAN="2" BGCOLOR="#64748b"><FONT COLOR="white"><B>audit_log 审计日志</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>id</B> PK</TD><TD ALIGN="LEFT">INTEGER</TD></TR>
    <TR><TD ALIGN="LEFT">table_name / operation</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">record_key / old / new</TD><TD ALIGN="LEFT">TEXT</TD></TR>
    <TR><TD ALIGN="LEFT">operator / log_time</TD><TD ALIGN="LEFT">TEXT</TD></TR>
  </TABLE>>];

  users -> teachers [dir=both, arrowtail=crow, arrowhead=none, label="1:1"];
  users -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:0..1"];
  classes -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  teachers -> subjects [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  students -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  subjects -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  grades -> audit [style=dashed, arrowhead=vee, label="触发器"];
}}
""",
    "modules": f"""
digraph Modules {{
  graph [rankdir=TB];
  {COMMON}
  node [shape=box, style="rounded,filled", fillcolor="#dbeafe", width=1.5];
  root [label="学生信息管理系统", fillcolor="#2563eb", fontcolor="white", width=2.2];

  node [fillcolor="#e0f2fe"];
  admin [label="管理员端"];
  teacher [label="教师端"];
  student [label="学生端"];

  node [shape=box, style="rounded,filled", fillcolor="#f8fafc", width=1.7];
  a1 [label="教师账号管理"]; a2 [label="数据看板"]; a3 [label="审计日志"]; a4 [label="备份 / 恢复"];
  t1 [label="学生管理"]; t2 [label="班级管理"]; t3 [label="课程管理"]; t4 [label="成绩管理"];
  t5 [label="统计报表"]; t6 [label="账号 / 密保"];
  s1 [label="个人信息"]; s2 [label="成绩查询"]; s3 [label="GPA / 排名"]; s4 [label="修改密码"];

  root -> admin; root -> teacher; root -> student;
  {{rank=same; admin; teacher; student;}}
  admin -> {{a1 a2 a3 a4}};
  teacher -> {{t1 t2 t3 t4 t5 t6}};
  student -> {{s1 s2 s3 s4}};
}}
""",
    "architecture": f"""
digraph Architecture {{
  graph [rankdir=TB];
  {COMMON}
  node [shape=box, style="rounded,filled", width=4.2];
  ui [label="表现层  Qt Widgets\\n登录 / 教师端 / 管理员端 / 学生端 / QSS 主题", fillcolor="#dbeafe"];
  biz [label="业务逻辑层\\n认证鉴权、学生/班级/课程/成绩、统计报表、备份恢复", fillcolor="#e0e7ff"];
  dao [label="数据访问层\\nDatabaseManager（单例）、SQL 脚本执行、事务、预处理语句", fillcolor="#fef3c7"];
  db [label="SQLite 3.51  StudentSystem.db\\n表 + 视图 + 触发器 + 索引 + WAL", shape=cylinder, fillcolor="#dcfce7", height=1.2];

  ui -> biz [label="信号槽 / 业务调用"];
  biz -> dao [label="统一 API"];
  dao -> db [label="QSQLITE"];
  db -> dao [label="结果集 / 错误", style=dashed];
}}
""",
    "flow_login": f"""
digraph LoginFlow {{
  graph [rankdir=TB];
  {COMMON}
  node [shape=box, style="rounded,filled", fillcolor="#f8fafc", width=2.0];
  start [label="开始", shape=oval, fillcolor="#dcfce7"];
  input [label="输入账号 / 密码"];
  verify [label="查询 users 表\\n校验盐 + 迭代哈希", shape=diamond, fillcolor="#fef3c7", height=1.2];
  fail [label="失败次数 +1\\n≥5 次锁定 30 秒", fillcolor="#fee2e2"];
  role [label="判断 user_type", shape=diamond, fillcolor="#fef3c7", height=1.1];
  must [label="must_change_pw = 1 ?", shape=diamond, fillcolor="#fef3c7", height=1.1];
  change [label="强制修改密码"];
  admin [label="管理员主窗口"];
  teacher [label="教师主窗口"];
  student [label="学生主窗口"];
  end [label="写入 app_context\\n进入主界面", shape=oval, fillcolor="#dcfce7"];

  start -> input -> verify;
  verify -> fail [label="否"];
  fail -> input [style=dashed];
  verify -> must [label="是"];
  must -> change [label="是"];
  must -> role [label="否"];
  change -> role;
  role -> admin [label="admin"];
  role -> teacher [label="teacher"];
  role -> student [label="student"];
  admin -> end; teacher -> end; student -> end;
}}
""",
    "flow_grade": f"""
digraph GradeFlow {{
  graph [rankdir=TB];
  {COMMON}
  node [shape=box, style="rounded,filled", fillcolor="#f8fafc", width=2.4];
  start [label="教师进入成绩录入", shape=oval, fillcolor="#dcfce7"];
  choose [label="选择学生、本人课程、学期"];
  valid [label="成绩 0-100？\\n学期格式合法？", shape=diamond, fillcolor="#fef3c7", height=1.1];
  exists [label="该成绩已存在？", shape=diamond, fillcolor="#fef3c7", height=1.0];
  confirm [label="提示并确认覆盖"];
  upsert [label="INSERT ... ON CONFLICT\\nDO UPDATE（UPSERT）"];
  trigger [label="触发器：写 audit_log\\n记录旧值 / 新值 / 操作者"];
  commit [label="语句提交，刷新列表"];
  error [label="拒绝并提示", fillcolor="#fee2e2"];
  end [label="结束", shape=oval, fillcolor="#dcfce7"];

  start -> choose -> valid;
  valid -> error [label="否"];
  valid -> exists [label="是"];
  exists -> confirm [label="是"];
  exists -> upsert [label="否"];
  confirm -> upsert;
  upsert -> trigger -> commit -> end;
  error -> choose [style=dashed];
}}
""",
}


DIAGRAMS["er_compact"] = f"""
digraph ERCompact {{
  graph [rankdir=TB, nodesep=0.55, ranksep=0.75];
  graph [fontname="Microsoft YaHei", fontsize=15, bgcolor="white", pad="0.3"];
  node [fontname="Microsoft YaHei", fontsize=15, color="#334155", fontcolor="#0f172a"];
  edge [fontname="Microsoft YaHei", fontsize=13, color="#475569", fontcolor="#334155"];

  node [shape=box, style="rounded,filled", fillcolor="#dbeafe", height=0.8];
  user [label="用户 users\nPK user_id"];
  teacher [label="教师 teachers\nPK teacher_id"];
  klass [label="班级 classes\nPK class_name"];
  student [label="学生 students\nPK student_id"];
  subject [label="课程 subjects\nPK subject_id"];

  node [shape=diamond, style="filled", fillcolor="#fef3c7", height=1.0, width=1.5];
  grade_rel [label="取得成绩\n（grades）"];

  user -> teacher [label="拥有账号 1:1"];
  user -> student [label="拥有账号 1:1"];
  klass -> student [label="属于 1:N"];
  teacher -> subject [label="讲授 1:N"];
  student -> grade_rel [label="M"];
  subject -> grade_rel [label="N"];

  note [shape=note, style="filled", fillcolor="#f8fafc", fontsize=12,
        label="成绩联系属性：学期、分数、考试日期、录入教师\n其余属性见数据字典 / 关系模式"];
  grade_rel -> note [style=dashed, arrowhead=none];
}}
"""

DIAGRAMS["schema_compact"] = f"""
digraph SchemaCompact {{
  graph [rankdir=LR, nodesep=0.5, ranksep=0.7];
  graph [fontname="Microsoft YaHei", fontsize=13, bgcolor="white", pad="0.3"];
  node [fontname="Microsoft YaHei", fontsize=13, color="#334155", fontcolor="#0f172a"];
  edge [fontname="Microsoft YaHei", fontsize=12, color="#475569", fontcolor="#334155"];
  node [shape=plain];

  users [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#eff6ff">
    <TR><TD BGCOLOR="#2563eb"><FONT COLOR="white"><B>users 用户</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>user_id</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">username / user_type</TD></TR>
    <TR><TD ALIGN="LEFT">password / 密保</TD></TR>
  </TABLE>>];
  teachers [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#f0fdf4">
    <TR><TD BGCOLOR="#16a34a"><FONT COLOR="white"><B>teachers 教师</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>teacher_id</B> (PK/FK)</TD></TR>
    <TR><TD ALIGN="LEFT">name / title</TD></TR>
  </TABLE>>];
  classes [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#fefce8">
    <TR><TD BGCOLOR="#ca8a04"><FONT COLOR="white"><B>classes 班级</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>class_name</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">grade_year / advisor</TD></TR>
  </TABLE>>];
  students [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#fdf2f8">
    <TR><TD BGCOLOR="#db2777"><FONT COLOR="white"><B>students 学生</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>student_id</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">name / class_name (FK)</TD></TR>
    <TR><TD ALIGN="LEFT">user_id (FK)</TD></TR>
  </TABLE>>];
  subjects [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#fff7ed">
    <TR><TD BGCOLOR="#ea580c"><FONT COLOR="white"><B>subjects 课程</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>subject_id</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">subject_name / credit</TD></TR>
    <TR><TD ALIGN="LEFT">teacher_id (FK)</TD></TR>
  </TABLE>>];
  grades [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#f5f3ff">
    <TR><TD BGCOLOR="#7c3aed"><FONT COLOR="white"><B>grades 成绩</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>id</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">student_id / subject_id (FK)</TD></TR>
    <TR><TD ALIGN="LEFT">semester / score</TD></TR>
  </TABLE>>];
  audit [label=<<TABLE BORDER="1" CELLBORDER="0" CELLSPACING="0" CELLPADDING="5" BGCOLOR="#f8fafc">
    <TR><TD BGCOLOR="#64748b"><FONT COLOR="white"><B>audit_log 审计</B></FONT></TD></TR>
    <TR><TD ALIGN="LEFT"><B>id</B> (PK)</TD></TR>
    <TR><TD ALIGN="LEFT">operation / operator</TD></TR>
  </TABLE>>];

  users -> teachers [dir=both, arrowtail=crow, arrowhead=none, label="1:1"];
  users -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:0..1"];
  classes -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  teachers -> subjects [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  students -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  subjects -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  grades -> audit [style=dashed, arrowhead=vee, label="触发器"];
}}
"""


DIAGRAMS["schema_compact"] = f"""
digraph SchemaSimple {{
  graph [rankdir=LR, nodesep=0.5, ranksep=0.8];
  graph [fontname="Microsoft YaHei", fontsize=15, bgcolor="white", pad="0.3"];
  node [fontname="Microsoft YaHei", fontsize=15, color="#334155", fontcolor="#0f172a"];
  edge [fontname="Microsoft YaHei", fontsize=13, color="#475569", fontcolor="#334155"];
  node [shape=box, style="rounded,filled", height=0.9, width=1.9];

  users [label="用户 users\nPK user_id\nusername / user_type", fillcolor="#dbeafe"];
  teachers [label="教师 teachers\nPK teacher_id (FK)\nname / title", fillcolor="#dcfce7"];
  classes [label="班级 classes\nPK class_name\ngrade_year / advisor", fillcolor="#fef9c3"];
  students [label="学生 students\nPK student_id\nclass_name (FK) / user_id (FK)", fillcolor="#fce7f3"];
  subjects [label="课程 subjects\nPK subject_id\nteacher_id (FK) / credit", fillcolor="#ffedd5"];
  grades [label="成绩 grades\nPK id\nUNIQUE(student,subject,semester)", fillcolor="#ede9fe"];
  audit [label="审计 audit_log\nPK id\noperation / operator", fillcolor="#f1f5f9"];

  users -> teachers [dir=both, arrowtail=crow, arrowhead=none, label="1:1"];
  users -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:0..1"];
  classes -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  teachers -> subjects [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  students -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  subjects -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  grades -> audit [style=dashed, arrowhead=vee, label="触发器"];
}}
"""


DIAGRAMS["schema_compact"] = f"""
digraph SchemaSimple {{
  graph [bgcolor="white", pad="0.2", splines=true];
  node [shape=box, style="rounded,filled", fontname="Microsoft YaHei", fontsize=15,
        color="#334155", fontcolor="#0f172a", width=2.6, height=1.05, fixedsize=true];
  edge [fontname="Microsoft YaHei", fontsize=13, color="#475569", fontcolor="#334155", labeldistance=1.6];

  users [pos="0,0!", fillcolor="#dbeafe", label="用户 users\nPK user_id\nusername / user_type"];
  teachers [pos="0,-170!", fillcolor="#dcfce7", label="教师 teachers\nPK teacher_id (FK)\nname / title"];
  subjects [pos="0,-340!", fillcolor="#ffedd5", label="课程 subjects\nPK subject_id\ncredit / teacher_id"];
  students [pos="290,0!", fillcolor="#fce7f3", label="学生 students\nPK student_id\nclass_name / user_id"];
  classes [pos="290,170!", fillcolor="#fef9c3", label="班级 classes\nPK class_name\ngrade_year / advisor"];
  grades [pos="290,-340!", fillcolor="#ede9fe", label="成绩 grades\nPK id\nUNIQUE(学生,课程,学期)"];
  audit [pos="580,-340!", fillcolor="#f1f5f9", label="审计 audit_log\nPK id\noperator / log_time"];

  users -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:1"];
  users -> teachers [dir=both, arrowtail=crow, arrowhead=none, label="1:1"];
  classes -> students [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  teachers -> subjects [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  students -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  subjects -> grades [dir=both, arrowtail=crow, arrowhead=none, label="1:N"];
  grades -> audit [style=dashed, arrowhead=vee, label="触发器"];
}}
"""


DIAGRAMS["modules"] = f"""
digraph Modules {{
  graph [bgcolor="white", pad="0.2"];
  node [shape=box, style="rounded,filled", fontname="Microsoft YaHei", fontsize=16,
        color="#334155", fontcolor="#0f172a", fillcolor="#f8fafc", width=1.7, height=0.5, fixedsize=true];
  edge [color="#475569"];

  root [pos="0,120!", fillcolor="#2563eb", fontcolor="white", label="学生信息管理系统", width=2.0, height=0.65];
  admin [pos="-240,0!", fillcolor="#e0f2fe", label="管理员端"];
  teacher [pos="0,0!", fillcolor="#e0f2fe", label="教师端"];
  student [pos="240,0!", fillcolor="#e0f2fe", label="学生端"];

  a1 [pos="-240,-70!", label="教师账号管理"];
  a2 [pos="-240,-140!", label="数据看板"];
  a3 [pos="-240,-210!", label="审计日志"];
  a4 [pos="-240,-280!", label="备份/恢复"];

  t1 [pos="0,-70!", label="学生管理"];
  t2 [pos="0,-140!", label="班级管理"];
  t3 [pos="0,-210!", label="课程管理"];
  t4 [pos="0,-280!", label="成绩管理"];
  t5 [pos="0,-350!", label="统计报表"];
  t6 [pos="0,-420!", label="账号/密保"];

  s1 [pos="240,-70!", label="个人信息"];
  s2 [pos="240,-140!", label="成绩查询"];
  s3 [pos="240,-210!", label="GPA/排名"];
  s4 [pos="240,-280!", label="修改密码"];

  root -> admin; root -> teacher; root -> student;
  admin -> {{a1 a2 a3 a4}};
  teacher -> {{t1 t2 t3 t4 t5 t6}};
  student -> {{s1 s2 s3 s4}};
}}
"""


def main():
    for name, dot_source in DIAGRAMS.items():
        out_file = OUT / f"{name}.png"
        engine = ENGINES.get(name, "dot")
        result = subprocess.run(
            [str(DOT_DIR / (engine + ".exe")), "-Tpng", "-Gdpi=180"] + EXTRA_ARGS.get(name, []),
            input=dot_source.encode("utf-8"),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        if result.returncode != 0:
            print(f"[FAIL] {name}: {result.stderr.decode('utf-8', 'ignore')[:500]}")
            continue
        out_file.write_bytes(result.stdout)
        print(f"[OK] {out_file} ({len(result.stdout) // 1024} KB)")


if __name__ == "__main__":
    main()
