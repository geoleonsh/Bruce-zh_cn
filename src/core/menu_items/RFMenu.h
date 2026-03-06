#ifndef __RF_MENU_H__
#define __RF_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class RFMenu : public MenuItemInterface {
public:
    RFMenu() : MenuItemInterface("射频") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.rf; }
    String themePath() { return bruceConfig.theme.paths.rf; }

private:
    void configMenu(void);
};

#endif
