#ifndef __WIFI_MENU_H__
#define __WIFI_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class WifiMenu : public MenuItemInterface {
public:
    WifiMenu() : MenuItemInterface("无线网络") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.wifi; }
    String themePath() { return bruceConfig.theme.paths.wifi; }

private:
    void configMenu(void);
};

#endif
