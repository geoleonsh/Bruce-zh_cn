#ifndef __BLE_MENU_H__
#define __BLE_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class BleMenu : public MenuItemInterface {
public:
    BleMenu() : MenuItemInterface("蓝牙") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.ble; }
    String themePath() { return bruceConfig.theme.paths.ble; }

private:
    void configMenu(void);
    void setBleNameMenu(void);
};

#endif
