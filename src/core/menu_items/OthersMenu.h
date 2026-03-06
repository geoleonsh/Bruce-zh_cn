#ifndef __OTHERS_MENU_H__
#define __OTHERS_MENU_H__

#include "MenuItemInterface.h"
#include "core/i18n/zh_CN.h"

class OthersMenu : public MenuItemInterface {

public:
    OthersMenu() : MenuItemInterface("其他") {}

    void micMenu();
    void badUsbHidMenu(); // New submenu for BadUSB & HID tools
    void optionsMenu(void);
    void drawIcon(float scale);

    bool hasTheme() { return bruceConfig.theme.others; }
    String themePath() { return bruceConfig.theme.paths.others; }
};

#endif
