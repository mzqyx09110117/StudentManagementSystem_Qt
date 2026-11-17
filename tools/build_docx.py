#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把 docs/作品说明书.md 转换为排版规范的中文 Word 文档。

- 封面、目录域、页眉页脚、页码
- 宋体小四正文、黑体标题、1.5 倍行距、首行缩进 2 字符
- 图片居中 + 图题，表格加边框与表头底纹，代码块灰色底纹
"""
import re
from pathlib import Path

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.style import WD_STYLE_TYPE
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH, WD_LINE_SPACING
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor

ROOT = Path(__file__).resolve().parent.parent
DOCS = ROOT / "docs"
SOURCE = DOCS / "作品说明书.md"
OUTPUT = DOCS / "作品说明书.docx"

BODY_FONT_CN = "宋体"
BODY_FONT_EN = "Times New Roman"
HEAD_FONT_CN = "黑体"
CODE_FONT = "Consolas"


def set_run_font(run, cn=BODY_FONT_CN, en=BODY_FONT_EN, size=12, bold=None, color=None):
    run.font.name = en
    run.font.size = Pt(size)
    if bold is not None:
        run.font.bold = bold
    if color is not None:
        run.font.color.rgb = color
    rPr = run._element.get_or_add_rPr()
    rFonts = rPr.find(qn("w:rFonts"))
    if rFonts is None:
        rFonts = OxmlElement("w:rFonts")
        rPr.append(rFonts)
    rFonts.set(qn("w:ascii"), en)
    rFonts.set(qn("w:hAnsi"), en)
    rFonts.set(qn("w:eastAsia"), cn)


def set_style_font(style, cn, en, size, bold=False):
    style.font.name = en
    style.font.size = Pt(size)
    style.font.bold = bold
    rPr = style.element.get_or_add_rPr()
    rFonts = rPr.find(qn("w:rFonts"))
    if rFonts is None:
        rFonts = OxmlElement("w:rFonts")
        rPr.append(rFonts)
    rFonts.set(qn("w:ascii"), en)
    rFonts.set(qn("w:hAnsi"), en)
    rFonts.set(qn("w:eastAsia"), cn)


def shade(element_pr, color):
    shd = OxmlElement("w:shd")
    shd.set(qn("w:val"), "clear")
    shd.set(qn("w:color"), "auto")
    shd.set(qn("w:fill"), color)
    element_pr.append(shd)


def shade_cell(cell, color):
    shade(cell._tc.get_or_add_tcPr(), color)


def shade_paragraph(paragraph, color):
    shade(paragraph._p.get_or_add_pPr(), color)


def set_first_line_indent(paragraph, chars=200):
    pPr = paragraph._p.get_or_add_pPr()
    ind = pPr.find(qn("w:ind"))
    if ind is None:
        ind = OxmlElement("w:ind")
        pPr.append(ind)
    ind.set(qn("w:firstLineChars"), str(chars))


def add_field(paragraph, instruction, placeholder="1"):
    run = paragraph.add_run()
    begin = OxmlElement("w:fldChar")
    begin.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = instruction
    separate = OxmlElement("w:fldChar")
    separate.set(qn("w:fldCharType"), "separate")
    text = OxmlElement("w:t")
    text.text = placeholder
    end = OxmlElement("w:fldChar")
    end.set(qn("w:fldCharType"), "end")
    run._r.append(begin)
    run._r.append(instr)
    run._r.append(separate)
    run._r.append(text)
    run._r.append(end)


def add_page_field(paragraph, instruction):
    run = paragraph.add_run()
    begin = OxmlElement("w:fldChar")
    begin.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = f" {instruction} "
    separate = OxmlElement("w:fldChar")
    separate.set(qn("w:fldCharType"), "separate")
    text = OxmlElement("w:t")
    text.text = "1"
    end = OxmlElement("w:fldChar")
    end.set(qn("w:fldCharType"), "end")
    run._r.append(begin)
    run._r.append(instr)
    run._r.append(separate)
    run._r.append(text)
    run._r.append(end)


def set_cell_margins(table, top=40, start=70, bottom=40, end=70):
    tblPr = table._tbl.tblPr
    mar = OxmlElement("w:tblCellMar")
    for tag, value in (("top", top), ("start", start), ("bottom", bottom), ("end", end)):
        node = OxmlElement(f"w:{tag}")
        node.set(qn("w:w"), str(value))
        node.set(qn("w:type"), "dxa")
        mar.append(node)
    tblPr.append(mar)


def set_table_width_pct(table, pct=5000):
    tblPr = table._tbl.tblPr
    width = OxmlElement("w:tblW")
    width.set(qn("w:w"), str(pct))
    width.set(qn("w:type"), "pct")
    tblPr.append(width)


def repeat_header(row):
    trPr = row._tr.get_or_add_trPr()
    header = OxmlElement("w:tblHeader")
    header.set(qn("w:val"), "true")
    trPr.append(header)


def no_split(row):
    trPr = row._tr.get_or_add_trPr()
    cant = OxmlElement("w:cantSplit")
    cant.set(qn("w:val"), "true")
    trPr.append(cant)


def add_inline_runs(paragraph, text, size=12, cn=BODY_FONT_CN, bold=False):
    pattern = re.compile(r"(\*\*[^*]+\*\*|`[^`]+`)")
    pos = 0
    for match in pattern.finditer(text):
        if match.start() > pos:
            run = paragraph.add_run(text[pos:match.start()])
            set_run_font(run, cn=cn, size=size, bold=bold)
        token = match.group(0)
        if token.startswith("**"):
            run = paragraph.add_run(token[2:-2])
            set_run_font(run, cn=cn, size=size, bold=True)
        else:
            run = paragraph.add_run(token[1:-1])
            set_run_font(run, cn=CODE_FONT, en=CODE_FONT, size=size - 1, bold=False)
            run.font.color.rgb = RGBColor(0x1E, 0x40, 0xAF)
        pos = match.end()
    if pos < len(text):
        run = paragraph.add_run(text[pos:])
        set_run_font(run, cn=cn, size=size, bold=bold)


def add_code_block(doc, code):
    paragraph = doc.add_paragraph()
    paragraph.alignment = WD_ALIGN_PARAGRAPH.LEFT
    paragraph.paragraph_format.left_indent = Cm(0.2)
    paragraph.paragraph_format.space_before = Pt(4)
    paragraph.paragraph_format.space_after = Pt(6)
    paragraph.paragraph_format.line_spacing = 1.0
    paragraph.paragraph_format.keep_together = True
    shade_paragraph(paragraph, "F1F5F9")
    pPr = paragraph._p.get_or_add_pPr()
    borders = OxmlElement("w:pBdr")
    for edge in ("top", "left", "bottom", "right"):
        node = OxmlElement(f"w:{edge}")
        node.set(qn("w:val"), "single")
        node.set(qn("w:sz"), "4")
        node.set(qn("w:space"), "4")
        node.set(qn("w:color"), "E2E8F0")
        borders.append(node)
    pPr.append(borders)

    lines = code.splitlines() or [""]
    for index, line in enumerate(lines):
        leading = len(line) - len(line.lstrip(" "))
        text = " " * leading + line[leading:]
        run = paragraph.add_run(text)
        set_run_font(run, cn=CODE_FONT, en=CODE_FONT, size=8)
        if index != len(lines) - 1:
            run.add_break()


def image_width(path):
    name = Path(path).name
    widths = {
        "er_compact.png": 11.0,
        "schema_compact.png": 13.2,
        "modules.png": 12.0,
        "architecture.png": 9.0,
        "flow_login.png": 9.2,
        "flow_grade.png": 9.2,
        "index_benchmark.png": 14.0,
    }
    if name in widths:
        return widths[name]
    if name in ("04_teacher_main.png", "09_student.png", "10_admin_main.png",
                "11_dashboard.png", "12_audit_log.png"):
        return 15.0
    if name.startswith(("01_", "02_", "03_", "05_", "06_", "07_", "08_",
                        "13_", "14_", "15_", "16_")):
        return 11.0
    return 13.0


def add_image(doc, alt, rel_path):
    image_path = DOCS / rel_path
    if not image_path.exists():
        paragraph = doc.add_paragraph(f"[缺少图片：{rel_path}]")
        paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
        return
    paragraph = doc.add_paragraph()
    paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
    paragraph.paragraph_format.space_before = Pt(6)
    paragraph.paragraph_format.space_after = Pt(2)
    run = paragraph.add_run()
    run.add_picture(str(image_path), width=Cm(image_width(image_path)))

    caption = doc.add_paragraph()
    caption.alignment = WD_ALIGN_PARAGRAPH.CENTER
    caption.paragraph_format.space_after = Pt(10)
    run = caption.add_run(alt)
    set_run_font(run, size=10.5, bold=False)


def parse_table(lines):
    rows = []
    for line in lines:
        stripped = line.strip().strip("|")
        cells = [cell.strip() for cell in stripped.split("|")]
        if all(re.fullmatch(r":?-{2,}:?", cell) for cell in cells if cell):
            continue
        rows.append(cells)
    return rows


def add_table(doc, rows):
    if not rows:
        return
    column_count = max(len(row) for row in rows)
    table = doc.add_table(rows=0, cols=column_count)
    table.style = "Table Grid"
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    table.autofit = True
    set_table_width_pct(table)
    set_cell_margins(table)

    for row_index, values in enumerate(rows):
        row = table.add_row()
        if row_index == 0:
            repeat_header(row)
        no_split(row)
        for col in range(column_count):
            cell = row.cells[col]
            text = values[col] if col < len(values) else ""
            cell.text = ""
            paragraph = cell.paragraphs[0]
            paragraph.paragraph_format.space_before = Pt(0)
            paragraph.paragraph_format.space_after = Pt(0)
            paragraph.paragraph_format.line_spacing = 1.0
            if row_index == 0:
                paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
            elif re.fullmatch(r"[-+]?\d+(\.\d+)?%?×?", text.strip()):
                paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
            run = paragraph.add_run(text)
            set_run_font(run, size=9, bold=(row_index == 0))
            if row_index == 0:
                shade_cell(cell, "E8EEF7")
    doc.add_paragraph().paragraph_format.space_after = Pt(2)


def setup_styles(doc):
    normal = doc.styles["Normal"]
    set_style_font(normal, BODY_FONT_CN, BODY_FONT_EN, 12)
    normal.paragraph_format.line_spacing_rule = WD_LINE_SPACING.ONE_POINT_FIVE
    normal.paragraph_format.space_before = Pt(0)
    normal.paragraph_format.space_after = Pt(0)
    normal.paragraph_format.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY

    specs = {
        "Heading 1": (16, 14, 10),
        "Heading 2": (14, 10, 6),
        "Heading 3": (12, 8, 4),
    }
    for name, (size, before, after) in specs.items():
        style = doc.styles[name]
        set_style_font(style, HEAD_FONT_CN, BODY_FONT_EN, size, bold=True)
        style.font.color.rgb = RGBColor(0, 0, 0)
        style.paragraph_format.space_before = Pt(before)
        style.paragraph_format.space_after = Pt(after)
        style.paragraph_format.keep_with_next = True
        style.paragraph_format.line_spacing = 1.3

    for name in ("List Bullet",):
        style = doc.styles[name]
        set_style_font(style, BODY_FONT_CN, BODY_FONT_EN, 12)

    # 目录条目：单倍行距，避免目录占三页
    for name, size, indent in (("TOC 1", 11, 0.0), ("TOC 2", 10.5, 0.5), ("TOC 3", 10, 1.0)):
        try:
            style = doc.styles[name]
        except KeyError:
            style = doc.styles.add_style(name, WD_STYLE_TYPE.PARAGRAPH)
        set_style_font(style, BODY_FONT_CN, BODY_FONT_EN, size)
        style.paragraph_format.line_spacing = 1.0
        style.paragraph_format.space_before = Pt(0)
        style.paragraph_format.space_after = Pt(1)
        style.paragraph_format.left_indent = Cm(indent)


def add_cover(doc):
    for _ in range(2):
        doc.add_paragraph()
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run("计算机科学与工程学院")
    set_run_font(run, cn=HEAD_FONT_CN, size=18, bold=True)

    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_before = Pt(40)
    run = p.add_run("学生信息管理系统")
    set_run_font(run, cn=HEAD_FONT_CN, size=30, bold=True)

    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_before = Pt(10)
    run = p.add_run("数据库应用程序设计与开发 · 作品说明书")
    set_run_font(run, cn=HEAD_FONT_CN, size=16)

    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_before = Pt(6)
    run = p.add_run("（课程：数据库实验　项目：选做项目 2）")
    set_run_font(run, size=12)

    for _ in range(3):
        doc.add_paragraph()

    info = [
        ("项目名称", "学生信息管理系统的设计与实现"),
        ("专业", "人工智能"),
        ("班级", "2403 班"),
        ("学号", "20245627"),
        ("姓名", "祁震淼"),
        ("指导教师", "　　　　　　　　"),
        ("完成日期", "2026 年 11 月"),
    ]
    table = doc.add_table(rows=0, cols=2)
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    table.autofit = False
    for label, value in info:
        row = table.add_row()
        row.cells[0].width = Cm(3.6)
        row.cells[1].width = Cm(8.4)
        for cell in row.cells:
            cell.text = ""
        p0 = row.cells[0].paragraphs[0]
        p0.alignment = WD_ALIGN_PARAGRAPH.RIGHT
        r0 = p0.add_run(label + "：")
        set_run_font(r0, cn=HEAD_FONT_CN, size=12, bold=True)
        p1 = row.cells[1].paragraphs[0]
        r1 = p1.add_run(value)
        set_run_font(r1, size=12)
        p1.paragraph_format.space_after = Pt(6)
        p0.paragraph_format.space_after = Pt(6)

    # 封面信息表不显示边框
    tblPr = table._tbl.tblPr
    borders = OxmlElement("w:tblBorders")
    for edge in ("top", "left", "bottom", "right", "insideH", "insideV"):
        node = OxmlElement(f"w:{edge}")
        node.set(qn("w:val"), "nil")
        borders.append(node)
    tblPr.append(borders)

    for _ in range(2):
        doc.add_paragraph()
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run("2026 年 11 月")
    set_run_font(run, size=14)


def add_header_footer(section):
    section.header.is_linked_to_previous = False
    section.footer.is_linked_to_previous = False

    header = section.header.paragraphs[0]
    header.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = header.add_run("学生信息管理系统 · 作品说明书")
    set_run_font(run, size=9)
    run.font.color.rgb = RGBColor(0x64, 0x74, 0x8B)
    pPr = header._p.get_or_add_pPr()
    borders = OxmlElement("w:pBdr")
    bottom = OxmlElement("w:bottom")
    bottom.set(qn("w:val"), "single")
    bottom.set(qn("w:sz"), "4")
    bottom.set(qn("w:space"), "4")
    bottom.set(qn("w:color"), "CBD5E1")
    borders.append(bottom)
    pPr.append(borders)

    footer = section.footer.paragraphs[0]
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = footer.add_run("第 ")
    set_run_font(run, size=9)
    add_page_field(footer, "PAGE")
    run = footer.add_run(" 页 / 共 ")
    set_run_font(run, size=9)
    add_page_field(footer, "NUMPAGES")
    run = footer.add_run(" 页")
    set_run_font(run, size=9)


def set_page_number_start(section, start=1):
    sectPr = section._sectPr
    pgNumType = sectPr.find(qn("w:pgNumType"))
    if pgNumType is None:
        pgNumType = OxmlElement("w:pgNumType")
        sectPr.append(pgNumType)
    pgNumType.set(qn("w:start"), str(start))


def build():
    doc = Document()
    setup_styles(doc)
    section = doc.sections[0]
    section.page_width = Cm(21.0)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(2.54)
    section.bottom_margin = Cm(2.54)
    section.left_margin = Cm(3.17)
    section.right_margin = Cm(3.17)

    doc.core_properties.title = "学生信息管理系统作品说明书"
    doc.core_properties.author = "祁震淼"
    doc.core_properties.subject = "数据库应用程序设计与开发"

    add_cover(doc)

    body = doc.add_section(WD_SECTION.NEW_PAGE)
    body.top_margin = Cm(2.54)
    body.bottom_margin = Cm(2.54)
    body.left_margin = Cm(3.17)
    body.right_margin = Cm(3.17)
    add_header_footer(body)
    set_page_number_start(body, 1)

    lines = SOURCE.read_text(encoding="utf-8").splitlines()
    index = 0
    first_h1 = True

    while index < len(lines):
        line = lines[index]
        stripped = line.strip()

        if not stripped:
            index += 1
            continue

        if stripped == "<!-- toc -->":
            heading = doc.add_paragraph()
            heading.alignment = WD_ALIGN_PARAGRAPH.LEFT
            heading.paragraph_format.page_break_before = True
            heading.paragraph_format.space_before = Pt(6)
            heading.paragraph_format.space_after = Pt(12)
            run = heading.add_run("目　录")
            set_run_font(run, cn=HEAD_FONT_CN, size=16, bold=True)
            toc_paragraph = doc.add_paragraph()
            add_field(toc_paragraph, r'TOC \o "1-2" \h \z \u', "右键“更新域”生成目录")
            index += 1
            continue

        if stripped == "<!-- pagebreak -->":
            doc.add_page_break()
            index += 1
            continue

        if stripped.startswith("```"):
            index += 1
            code_lines = []
            while index < len(lines) and not lines[index].strip().startswith("```"):
                code_lines.append(lines[index])
                index += 1
            index += 1
            add_code_block(doc, "\n".join(code_lines))
            continue

        image_match = re.fullmatch(r"!\[([^\]]*)\]\(([^)]+)\)", stripped)
        if image_match:
            add_image(doc, image_match.group(1), image_match.group(2))
            index += 1
            continue

        if stripped.startswith("|"):
            table_lines = []
            while index < len(lines) and lines[index].strip().startswith("|"):
                table_lines.append(lines[index])
                index += 1
            add_table(doc, parse_table(table_lines))
            continue

        heading_match = re.match(r"^(#{1,3})\s+(.*)$", stripped)
        if heading_match:
            level = len(heading_match.group(1))
            text = heading_match.group(2).strip()
            paragraph = doc.add_paragraph(style=f"Heading {level}")
            if level == 1 and not first_h1:
                if text.startswith("第") or text.startswith("参考文献") or text.startswith("附录"):
                    paragraph.paragraph_format.page_break_before = True
            first_h1 = False
            run = paragraph.add_run(text)
            set_run_font(run, cn=HEAD_FONT_CN, size={1: 16, 2: 14, 3: 12}[level], bold=True)
            run.font.color.rgb = RGBColor(0, 0, 0)
            index += 1
            continue

        if stripped.startswith("- "):
            while index < len(lines) and lines[index].strip().startswith("- "):
                item = lines[index].strip()[2:]
                paragraph = doc.add_paragraph(style="List Bullet")
                paragraph.paragraph_format.space_after = Pt(2)
                add_inline_runs(paragraph, item, size=12)
                index += 1
            continue

        # 普通段落：连续非空、非特殊行合并
        paragraph_lines = []
        while index < len(lines):
            candidate = lines[index].strip()
            if (not candidate or candidate.startswith("|") or candidate.startswith("```")
                    or candidate.startswith("![") or candidate.startswith("#")
                    or candidate.startswith("- ") or candidate.startswith("<!--")):
                break
            paragraph_lines.append(candidate)
            index += 1

        text = "".join(paragraph_lines)
        if not text:
            continue

        if re.match(r"^表\s*[0-9A-Za-z]", text):
            paragraph = doc.add_paragraph()
            paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
            paragraph.paragraph_format.space_before = Pt(8)
            paragraph.paragraph_format.space_after = Pt(3)
            paragraph.paragraph_format.keep_with_next = True
            add_inline_runs(paragraph, text, size=10.5)
            continue

        paragraph = doc.add_paragraph()
        paragraph.paragraph_format.space_after = Pt(3)
        set_first_line_indent(paragraph, 200)
        add_inline_runs(paragraph, text, size=12)

    # 让 Word 打开时自动更新目录与页码域
    settings = doc.settings.element
    update = OxmlElement("w:updateFields")
    update.set(qn("w:val"), "true")
    settings.append(update)

    doc.save(OUTPUT)
    print(f"已生成 {OUTPUT}")


if __name__ == "__main__":
    build()
