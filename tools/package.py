#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""组装课程提交包：源代码 + 作品说明书 + 可直接运行的演示程序。

不包含 PDF、截图、图表、报告源稿等额外材料。
"""
import shutil
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STAGE = ROOT / "提交" / "StudentManagementSystem_20245627"
ZIP = ROOT / "提交" / "学生信息管理系统_祁震淼_20245627.zip"

SOURCE_PATTERNS = ("*.cpp", "*.h", "*.ui", "*.pro", "*.qrc")


def copy_file(src: Path, dst: Path):
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)


def main():
    if STAGE.exists():
        shutil.rmtree(STAGE)
    STAGE.mkdir(parents=True)
    if ZIP.exists():
        ZIP.unlink()

    docx = ROOT / "docs" / "作品说明书.docx"
    if not docx.exists():
        raise SystemExit("缺少 docs/作品说明书.docx")

    # 01 作品说明书（只放 Word，不放 PDF）
    copy_file(docx, STAGE / docx.name)

    # 02 源代码
    source_dir = STAGE / "源代码"
    source_dir.mkdir()
    for pattern in SOURCE_PATTERNS:
        for file in ROOT.glob(pattern):
            copy_file(file, source_dir / file.name)
    for folder in ("resources", "sql"):
        shutil.copytree(ROOT / folder, source_dir / folder)
    tools_dir = source_dir / "tools"
    tools_dir.mkdir()
    for file in (ROOT / "tools").iterdir():
        if file.is_file() and not file.name.startswith("_") and file.suffix in (".py", ".ps1", ".bat"):
            copy_file(file, tools_dir / file.name)
    copy_file(ROOT / "README.md", source_dir / "README.md")

    # 03 演示程序（windeployqt 部署目录）
    deploy = ROOT / "deploy"
    exe = deploy / "StudentManagementSystem_Qt.exe"
    if not exe.exists():
        raise SystemExit("缺少 deploy 目录或可执行文件，请先运行 tools/build_release.bat")
    for required in ("sqldrivers/qsqlite.dll", "platforms/qwindows.dll", "Qt6Charts.dll"):
        if not (deploy / required).exists():
            raise SystemExit(f"部署目录缺少：{required}")
    shutil.copytree(deploy, STAGE / "演示程序")

    # 使用说明
    readme = """学生信息管理系统 提交包

1. 直接运行：进入“演示程序”目录，双击 StudentManagementSystem_Qt.exe。
   首次运行会在 data 目录创建 StudentSystem.db，并自动写入演示数据。

2. 演示账号：
   管理员 admin / admin123
   教师   teacher01 / 123456
   学生   2024001 / 123456

3. 作品说明书：根目录下的 作品说明书.docx。
4. 源代码：源代码目录，包含 SQL 脚本、Qt 工程与构建说明。
5. 若需重新初始化演示数据：关闭程序后删除“演示程序/data”目录，再次启动即可。
"""
    (STAGE / "使用说明.txt").write_text(readme, encoding="utf-8")

    # 打包
    with zipfile.ZipFile(ZIP, "w", zipfile.ZIP_DEFLATED, allowZip64=True) as zf:
        for file in sorted(STAGE.rglob("*")):
            if file.is_file():
                zf.write(file, file.relative_to(STAGE.parent))

    size_mb = ZIP.stat().st_size / (1024 * 1024)
    print(f"提交包：{ZIP}")
    print(f"大小：{size_mb:.2f} MB")
    if size_mb > 200:
        raise SystemExit("提交包超过 200MB！")


if __name__ == "__main__":
    main()
