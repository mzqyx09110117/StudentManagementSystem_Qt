-- ============================================================
-- 学生信息管理系统 数据库模式（SQLite 3.51）
-- 设计要点：
--   1. 单库多表，教师、班级、学生、课程、成绩、审计日志统一管理
--   2. 自然键（学号/课程号/班级名）与业务键一致，便于应用层引用
--   3. 外键启用 + 级联策略 + CHECK 约束 + 触发器，保证数据完整性
--   4. 视图封装常用统计（成绩、班级统计、GPA 排名、教师工作量）
--   5. 审计触发器记录成绩变更，operator 取自 app_context 会话表
-- ============================================================

PRAGMA foreign_keys = ON;

-- ---------- 元数据与会话 ----------
CREATE TABLE IF NOT EXISTS schema_meta (
    id                INTEGER PRIMARY KEY CHECK (id = 1),
    version           INTEGER NOT NULL,
    applied_time      TEXT NOT NULL DEFAULT (datetime('now','localtime')),
    seed_version      INTEGER NOT NULL DEFAULT 0,
    legacy_import_done INTEGER NOT NULL DEFAULT 0 CHECK (legacy_import_done IN (0,1))
);

CREATE TABLE IF NOT EXISTS app_context (
    id           INTEGER PRIMARY KEY CHECK (id = 1),
    current_user TEXT NOT NULL DEFAULT 'system'
);

-- ---------- 用户账号 ----------
-- 密码与两个密保答案各自使用独立盐，避免改密导致密保失效
CREATE TABLE IF NOT EXISTS users (
    user_id        TEXT PRIMARY KEY,
    username       TEXT NOT NULL COLLATE NOCASE UNIQUE,
    password_hash  TEXT NOT NULL,
    password_salt  TEXT NOT NULL DEFAULT '',
    user_type      TEXT NOT NULL CHECK (user_type IN ('admin','teacher','student')),
    question1      TEXT NOT NULL DEFAULT '',
    answer1_hash   TEXT NOT NULL DEFAULT '',
    answer1_salt   TEXT NOT NULL DEFAULT '',
    question2      TEXT NOT NULL DEFAULT '',
    answer2_hash   TEXT NOT NULL DEFAULT '',
    answer2_salt   TEXT NOT NULL DEFAULT '',
    is_active      INTEGER NOT NULL DEFAULT 1 CHECK (is_active IN (0,1)),
    must_change_pw INTEGER NOT NULL DEFAULT 0 CHECK (must_change_pw IN (0,1)),
    created_time   TEXT NOT NULL DEFAULT (datetime('now','localtime')),
    last_login_at  TEXT
);

-- 数据库层保证最多一个管理员
CREATE UNIQUE INDEX IF NOT EXISTS ux_one_admin ON users(user_type) WHERE user_type = 'admin';
CREATE INDEX IF NOT EXISTS idx_users_type ON users(user_type);
CREATE INDEX IF NOT EXISTS idx_users_active ON users(is_active);

-- ---------- 教师档案 ----------
-- 姓名放在教师实体中，users 只保存账号信息，避免冗余
CREATE TABLE IF NOT EXISTS teachers (
    teacher_id  TEXT PRIMARY KEY REFERENCES users(user_id)
                ON UPDATE CASCADE ON DELETE CASCADE,
    name        TEXT NOT NULL,
    title       TEXT,
    phone       TEXT,
    department  TEXT,
    created_time TEXT NOT NULL DEFAULT (datetime('now','localtime'))
);
CREATE INDEX IF NOT EXISTS idx_teachers_name ON teachers(name);

-- ---------- 班级 ----------
-- 独立班级表使“空班级”可以持久化；班级名作为自然主键
CREATE TABLE IF NOT EXISTS classes (
    class_name  TEXT PRIMARY KEY,
    grade_year  INTEGER NOT NULL DEFAULT (CAST(strftime('%Y','now') AS INTEGER))
                CHECK (grade_year BETWEEN 2000 AND 2100),
    advisor     TEXT,
    created_time TEXT NOT NULL DEFAULT (datetime('now','localtime'))
);

-- ---------- 学生 ----------
-- 未分班统一存 NULL；删除班级时置 NULL，不产生空字符串脏数据
CREATE TABLE IF NOT EXISTS students (
    student_id  TEXT PRIMARY KEY,
    name        TEXT NOT NULL,
    gender      TEXT NOT NULL DEFAULT '男' CHECK (gender IN ('男','女','其他')),
    age         INTEGER CHECK (age IS NULL OR (age BETWEEN 5 AND 100)),
    class_name  TEXT REFERENCES classes(class_name)
                ON UPDATE CASCADE ON DELETE SET NULL,
    phone       TEXT,
    address     TEXT,
    status      TEXT NOT NULL DEFAULT '在读'
                CHECK (status IN ('在读','休学','毕业','退学')),
    user_id     TEXT UNIQUE REFERENCES users(user_id)
                ON UPDATE CASCADE ON DELETE SET NULL,
    created_time TEXT NOT NULL DEFAULT (datetime('now','localtime')),
    updated_at  TEXT
);
CREATE INDEX IF NOT EXISTS idx_students_class ON students(class_name);
CREATE INDEX IF NOT EXISTS idx_students_name ON students(name);
CREATE INDEX IF NOT EXISTS idx_students_status ON students(status);

-- ---------- 课程 ----------
-- 课程名为“教师内唯一”，两位教师可各自开设同名课程
CREATE TABLE IF NOT EXISTS subjects (
    subject_id   TEXT PRIMARY KEY,
    subject_name TEXT NOT NULL,
    credit       REAL NOT NULL DEFAULT 1.0 CHECK (credit > 0 AND credit <= 20),
    teacher_id   TEXT REFERENCES teachers(teacher_id)
                 ON UPDATE CASCADE ON DELETE SET NULL,
    description  TEXT,
    created_time TEXT NOT NULL DEFAULT (datetime('now','localtime')),
    UNIQUE (teacher_id, subject_name)
);
CREATE INDEX IF NOT EXISTS idx_subjects_teacher ON subjects(teacher_id);
CREATE INDEX IF NOT EXISTS idx_subjects_name ON subjects(subject_name);

-- ---------- 成绩 ----------
-- 同一学生、同一课程、同一学期只允许一条成绩；唯一约束支撑 UPSERT
CREATE TABLE IF NOT EXISTS grades (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id  TEXT NOT NULL REFERENCES students(student_id)
                ON UPDATE CASCADE ON DELETE CASCADE,
    subject_id  TEXT NOT NULL REFERENCES subjects(subject_id)
                ON UPDATE CASCADE ON DELETE RESTRICT,
    semester    TEXT NOT NULL CHECK (semester GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[12]'),
    score       REAL NOT NULL CHECK (score >= 0 AND score <= 100),
    exam_date   TEXT,
    entered_by  TEXT REFERENCES teachers(teacher_id)
                ON UPDATE CASCADE ON DELETE SET NULL,
    remark      TEXT,
    created_time TEXT NOT NULL DEFAULT (datetime('now','localtime')),
    updated_at  TEXT,
    UNIQUE (student_id, subject_id, semester)
);
CREATE INDEX IF NOT EXISTS idx_grades_student_sem ON grades(student_id, semester);
CREATE INDEX IF NOT EXISTS idx_grades_subject_sem ON grades(subject_id, semester);
CREATE INDEX IF NOT EXISTS idx_grades_entered_by ON grades(entered_by);

-- ---------- 审计日志 ----------
CREATE TABLE IF NOT EXISTS audit_log (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    table_name TEXT NOT NULL,
    operation  TEXT NOT NULL CHECK (operation IN ('INSERT','UPDATE','DELETE')),
    record_key TEXT,
    old_value  TEXT,
    new_value  TEXT,
    operator   TEXT NOT NULL DEFAULT 'system',
    log_time   TEXT NOT NULL DEFAULT (datetime('now','localtime'))
);
CREATE INDEX IF NOT EXISTS idx_audit_time ON audit_log(log_time);
CREATE INDEX IF NOT EXISTS idx_audit_operator ON audit_log(operator);
CREATE INDEX IF NOT EXISTS idx_audit_table ON audit_log(table_name, operation);

-- ---------- 触发器：成绩合法性校验 ----------
-- CHECK 约束已兜底；触发器提供更友好的中文错误并校验学期格式
CREATE TRIGGER IF NOT EXISTS trg_grades_validate_ai
BEFORE INSERT ON grades
BEGIN
    SELECT RAISE(ABORT, '成绩必须在 0 到 100 之间')
    WHERE NEW.score < 0 OR NEW.score > 100;
    SELECT RAISE(ABORT, '学期格式应为 2024-2025-1')
    WHERE NEW.semester NOT GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[12]';
END;

CREATE TRIGGER IF NOT EXISTS trg_grades_validate_au
BEFORE UPDATE OF score, semester ON grades
BEGIN
    SELECT RAISE(ABORT, '成绩必须在 0 到 100 之间')
    WHERE NEW.score < 0 OR NEW.score > 100;
    SELECT RAISE(ABORT, '学期格式应为 2024-2025-1')
    WHERE NEW.semester NOT GLOB '[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[12]';
END;

-- ---------- 触发器：成绩审计 ----------
CREATE TRIGGER IF NOT EXISTS trg_grades_audit_ai
AFTER INSERT ON grades
BEGIN
    INSERT INTO audit_log(table_name, operation, record_key, old_value, new_value, operator)
    VALUES ('grades', 'INSERT',
            NEW.student_id || '/' || NEW.subject_id || '/' || NEW.semester,
            NULL,
            json_object('student_id', NEW.student_id,
                        'subject_id', NEW.subject_id,
                        'semester', NEW.semester,
                        'score', NEW.score),
            COALESCE((SELECT current_user FROM app_context WHERE id = 1), 'system'));
END;

CREATE TRIGGER IF NOT EXISTS trg_grades_audit_au
AFTER UPDATE ON grades
BEGIN
    INSERT INTO audit_log(table_name, operation, record_key, old_value, new_value, operator)
    VALUES ('grades', 'UPDATE',
            OLD.student_id || '/' || OLD.subject_id || '/' || OLD.semester,
            json_object('score', OLD.score,
                        'semester', OLD.semester,
                        'exam_date', OLD.exam_date),
            json_object('score', NEW.score,
                        'semester', NEW.semester,
                        'exam_date', NEW.exam_date),
            COALESCE((SELECT current_user FROM app_context WHERE id = 1), 'system'));
END;

CREATE TRIGGER IF NOT EXISTS trg_grades_audit_ad
AFTER DELETE ON grades
BEGIN
    INSERT INTO audit_log(table_name, operation, record_key, old_value, new_value, operator)
    VALUES ('grades', 'DELETE',
            OLD.student_id || '/' || OLD.subject_id || '/' || OLD.semester,
            json_object('score', OLD.score,
                        'semester', OLD.semester,
                        'exam_date', OLD.exam_date),
            NULL,
            COALESCE((SELECT current_user FROM app_context WHERE id = 1), 'system'));
END;

-- ---------- 触发器：学生更新时间 ----------
CREATE TRIGGER IF NOT EXISTS trg_students_touch
AFTER UPDATE ON students
BEGIN
    UPDATE students SET updated_at = datetime('now','localtime')
    WHERE student_id = NEW.student_id;
END;

-- ---------- 触发器：保留最后一个管理员 ----------
CREATE TRIGGER IF NOT EXISTS trg_last_admin_delete
BEFORE DELETE ON users
WHEN OLD.user_type = 'admin'
     AND (SELECT COUNT(*) FROM users WHERE user_type = 'admin') <= 1
BEGIN
    SELECT RAISE(ABORT, '系统必须至少保留一个管理员');
END;

CREATE TRIGGER IF NOT EXISTS trg_last_admin_update
BEFORE UPDATE OF user_type ON users
WHEN OLD.user_type = 'admin'
     AND NEW.user_type <> 'admin'
     AND (SELECT COUNT(*) FROM users WHERE user_type = 'admin') <= 1
BEGIN
    SELECT RAISE(ABORT, '系统必须至少保留一个管理员');
END;

-- ============================================================
-- 视图
-- ============================================================

-- 成绩明细视图：学生、班级、课程、等级、任课教师
CREATE VIEW IF NOT EXISTS v_student_grades AS
SELECT g.id            AS grade_id,
       s.student_id,
       s.name          AS student_name,
       s.class_name,
       sub.subject_id,
       sub.subject_name,
       sub.credit,
       g.semester,
       g.score,
       CASE WHEN g.score >= 90 THEN '优秀'
            WHEN g.score >= 80 THEN '良好'
            WHEN g.score >= 70 THEN '中等'
            WHEN g.score >= 60 THEN '及格'
            ELSE '不及格' END AS grade_level,
       g.exam_date,
       COALESCE(t.name, '未分配') AS teacher_name
FROM grades g
JOIN students s   ON s.student_id = g.student_id
JOIN subjects sub ON sub.subject_id = g.subject_id
LEFT JOIN teachers t ON t.teacher_id = sub.teacher_id;

-- 班级-课程-学期统计视图
CREATE VIEW IF NOT EXISTS v_class_subject_stats AS
SELECT s.class_name,
       sub.subject_id,
       sub.subject_name,
       g.semester,
       COUNT(*)                                                     AS student_count,
       ROUND(AVG(g.score), 2)                                       AS avg_score,
       MAX(g.score)                                                 AS max_score,
       MIN(g.score)                                                 AS min_score,
       ROUND(100.0 * SUM(CASE WHEN g.score >= 60 THEN 1 ELSE 0 END) / COUNT(*), 2) AS pass_rate,
       ROUND(100.0 * SUM(CASE WHEN g.score >= 90 THEN 1 ELSE 0 END) / COUNT(*), 2) AS excellent_rate
FROM grades g
JOIN students s   ON s.student_id = g.student_id
JOIN subjects sub ON sub.subject_id = g.subject_id
WHERE s.class_name IS NOT NULL
GROUP BY s.class_name, sub.subject_id, sub.subject_name, g.semester;

-- 学生总览视图：按课程取最新一次成绩，学分加权平均分与 GPA，含班级/年级排名
CREATE VIEW IF NOT EXISTS v_student_overview AS
WITH latest AS (
    SELECT g.*,
           ROW_NUMBER() OVER (PARTITION BY g.student_id, g.subject_id
                              ORDER BY g.semester DESC, g.id DESC) AS rn
    FROM grades g
),
calc AS (
    SELECT l.student_id,
           COUNT(*)                                                       AS subject_count,
           SUM(sub.credit)                                                AS total_credit,
           ROUND(SUM(l.score * sub.credit) / SUM(sub.credit), 2)          AS weighted_avg,
           ROUND(SUM((CASE WHEN l.score >= 90 THEN 4.0
                           WHEN l.score >= 85 THEN 3.7
                           WHEN l.score >= 82 THEN 3.3
                           WHEN l.score >= 78 THEN 3.0
                           WHEN l.score >= 75 THEN 2.7
                           WHEN l.score >= 72 THEN 2.3
                           WHEN l.score >= 68 THEN 2.0
                           WHEN l.score >= 64 THEN 1.5
                           WHEN l.score >= 60 THEN 1.0
                           ELSE 0.0 END) * sub.credit) / SUM(sub.credit), 3) AS gpa
    FROM latest l
    JOIN subjects sub ON sub.subject_id = l.subject_id
    WHERE l.rn = 1
    GROUP BY l.student_id
)
SELECT s.student_id,
       s.name,
       s.class_name,
       s.status,
       COALESCE(c.subject_count, 0) AS subject_count,
       COALESCE(c.total_credit, 0)  AS total_credit,
       COALESCE(c.weighted_avg, 0)  AS weighted_avg,
       COALESCE(c.gpa, 0)           AS gpa,
       RANK() OVER (PARTITION BY s.class_name
                    ORDER BY COALESCE(c.gpa, 0) DESC) AS class_rank,
       RANK() OVER (ORDER BY COALESCE(c.gpa, 0) DESC)  AS grade_rank
FROM students s
LEFT JOIN calc c ON c.student_id = s.student_id;

-- 教师工作量视图
CREATE VIEW IF NOT EXISTS v_teacher_workload AS
SELECT t.teacher_id,
       t.name,
       COUNT(DISTINCT sub.subject_id) AS subject_count,
       COUNT(g.id)                    AS grade_count,
       COUNT(DISTINCT g.student_id)   AS student_count,
       ROUND(AVG(g.score), 2)         AS avg_score
FROM teachers t
LEFT JOIN subjects sub ON sub.teacher_id = t.teacher_id
LEFT JOIN grades g     ON g.subject_id = sub.subject_id
GROUP BY t.teacher_id, t.name;

-- 审计日志视图：把 operator（user_id）显示为登录名
CREATE VIEW IF NOT EXISTS v_audit_display AS
SELECT a.id,
       a.log_time,
       COALESCE(u.username, a.operator) AS operator_name,
       a.operator,
       a.table_name,
       a.operation,
       a.record_key,
       a.old_value,
       a.new_value
FROM audit_log a
LEFT JOIN users u ON u.user_id = a.operator;

-- ---------- 元数据初始化 ----------
INSERT OR IGNORE INTO schema_meta (id, version) VALUES (1, 1);
INSERT OR IGNORE INTO app_context (id, current_user) VALUES (1, 'system');
