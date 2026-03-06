#ifndef __CONNECT_MENU_H__
#define __CONNECT_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class ConnectMenu : public MenuItemInterface {
public:
    ConnectMenu() : MenuItemInterface("连接") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.connect; }
    String themePath() { return bruceConfig.theme.paths.connect; }
};

#endif
