#ifndef __Ethernet_MENU_H__
#define __Ethernet_MENU_H__

#include "modules/ethernet/EthernetHelper.h"
#include "core/i18n/zh_CN.h"
#include <MenuItemInterface.h>
#if !defined(LITE_VERSION)
class EthernetMenu : public MenuItemInterface {
private:
    EthernetHelper *eth;
    void start_ethernet();

public:
    EthernetMenu() : MenuItemInterface("以太网") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.ethernet; }
    String themePath() { return bruceConfig.theme.paths.ethernet; }
};

#endif
#endif
