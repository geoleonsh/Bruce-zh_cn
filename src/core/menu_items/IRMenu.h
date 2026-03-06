#ifndef __IR_MENU_H__
#define __IR_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class IRMenu : public MenuItemInterface {
public:
    IRMenu() : MenuItemInterface("红外") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.ir; }
    String themePath() { return bruceConfig.theme.paths.ir; }

private:
    void configMenu(void);
};

#endif
