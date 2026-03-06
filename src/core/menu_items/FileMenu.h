#ifndef __FILE_MENU_H__
#define __FILE_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class FileMenu : public MenuItemInterface {
public:
    FileMenu() : MenuItemInterface("文件") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.files; }
    String themePath() { return bruceConfig.theme.paths.files; }
};

#endif
