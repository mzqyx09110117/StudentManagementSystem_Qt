#ifndef SCREENSHOTTOOL_H
#define SCREENSHOTTOOL_H

#include <QString>

/**
 * @brief 无人工交互的截图模式：使用独立演示库渲染各角色窗口与关键对话框。
 *
 * 用法：StudentManagementSystem_Qt.exe --capture <目录>
 */
class ScreenshotTool
{
public:
    static bool run(const QString &outputDir);
};

#endif // SCREENSHOTTOOL_H
