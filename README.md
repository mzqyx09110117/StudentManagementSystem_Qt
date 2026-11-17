# 学生信息管理系统（Qt + SQLite）

《数据库原理》选做项目 2「数据库应用程序的设计与开发」作品。

## 功能概览

- **管理员**：教师账号管理、数据看板、审计日志、数据库备份与恢复、个人密码/密保。
- **教师**：学生增删改查、分页与按字段搜索、班级管理（含空班级）、本人课程管理、成绩录入/修改/删除、班级/单科排名报表、CSV 导出与打印、开通学生账号。
- **学生**：个人信息、成绩明细、GPA、班级/年级排名、修改密码与密保（只读）。
- **数据库**：单库 `data/StudentSystem.db`，9 张表、5 个视图、8 个触发器、15 个显式索引，启用外键、CHECK/UNIQUE 约束、事务、WAL、审计与一致性备份。

## 技术栈

- Qt 6.11.0（Widgets / SQL / Charts / PrintSupport），C++17
- SQLite 3.51（Qt QSQLITE 驱动）
- MinGW GCC 13.1.0 + qmake

## 目录结构

```
*.cpp / *.h / *.ui        应用源码与界面
sql/schema.sql            建表、索引、视图、触发器脚本
sql/seed.sql              演示数据
tools/gen_seed.py         演示数据生成器（固定随机种子）
tools/test_database.py    数据库自动化测试（36 项）
tools/benchmark_index.py  索引性能测试
tools/gen_diagrams.py     E-R/架构/流程图生成
tools/build_docx.py       作品说明书 Word 生成
tools/update_docx.ps1     用 Word 更新目录并导出 PDF
tools/build_release.bat   Release 构建与部署
tools/package.py          生成提交 zip
selftest.cpp              应用自检（43 项）
screenshottool.cpp        自动截图
resources/style.qss       界面主题
docs/                     作品说明书、截图、图表
deploy/                   windeployqt 部署目录
```

## 构建（Windows）

需要 Qt 6.11.0 MinGW 64-bit，默认路径为 `D:\Qt\6.11.0\mingw_64` 与 `D:\Qt\Tools\mingw1310_64`。

```bat
cd build-qt611
D:\Qt\6.11.0\mingw_64\bin\qmake.exe ..\StudentManagementSystem_Qt.pro -spec win32-g++ CONFIG+=release
D:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe -j8
```

或直接运行 `tools\build_release.bat`，脚本会构建并调用 `windeployqt` 生成 `deploy/`。

## 运行与演示账号

双击 `deploy\StudentManagementSystem_Qt.exe`（或构建目录 `release\StudentManagementSystem_Qt.exe`）。首次运行会自动创建 `data/StudentSystem.db` 并写入演示数据。

| 角色 | 登录名 | 密码 |
|---|---|---|
| 管理员 | admin | admin123 |
| 教师 | teacher01 | 123456 |
| 教师 | teacher02 | 123456 |
| 学生 | 2024001 | 123456 |

演示版密码仅用于课程展示，实际部署应修改。

## 自动化测试

```bat
python -I tools\test_database.py
StudentManagementSystem_Qt.exe --db C:\temp\selftest.db --self-test
```

`--self-test` 会在当前目录写入 `selftest.log`，返回码 0 表示全部通过。

## 自动截图

```bat
StudentManagementSystem_Qt.exe --capture docs\screenshots
```

程序会在指定目录创建独立演示库并生成 16 张 PNG，不影响正式数据库。

## 作品说明书

- `docs/作品说明书.docx`（主交付）
- `docs/作品说明书.pdf`
- `docs/作品说明书.md`（源稿）
- 生成：`python -I tools\build_docx.py`，随后用 `tools\update_docx.ps1` 更新目录并导出 PDF。

## 说明

- 数据库文件位置：程序目录 `data/`；若不可写则回退到 `%APPDATA%/学生信息管理系统`。
- 可用环境变量 `SMS_DB_PATH` 指定数据库路径，便于测试。
- 课程管理只显示当前教师负责的课程；学生与班级为全校共享数据。
- 删除学生会同时删除其成绩并注销学生账号；删除课程会先确认并删除相关成绩；删除最后一个管理员会被数据库触发器拒绝。
