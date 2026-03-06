#ifndef __CLOCK_MENU_H__
#define __CLOCK_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class ClockMenu : public MenuItemInterface {
public:
    ClockMenu() : MenuItemInterface("时钟") {}

    void optionsMenu(void);
    void showSubMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.clock; }
    String themePath() { return bruceConfig.theme.paths.clock; }
};

#endif
