#ifndef __RFID_MENU_H__
#define __RFID_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class RFIDMenu : public MenuItemInterface {
public:
    RFIDMenu() : MenuItemInterface("射频识别") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.rfid; }
    String themePath() { return bruceConfig.theme.paths.rfid; }

private:
    void configMenu(void);
};

#endif
