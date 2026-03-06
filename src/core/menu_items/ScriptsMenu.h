#ifndef __SCRIPTS_MENU_H__
#define __SCRIPTS_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class ScriptsMenu : public MenuItemInterface {
public:
    ScriptsMenu() : MenuItemInterface("JS 解释器") {}

    void optionsMenu();
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.interpreter; }
    String themePath() { return bruceConfig.theme.paths.interpreter; }
};

#endif
