#ifndef __CONFIG_MENU_H__
#define __CONFIG_MENU_H__

#include <MenuItemInterface.h>
#include "core/i18n/zh_CN.h"

class ConfigMenu : public MenuItemInterface {
public:
    ConfigMenu() : MenuItemInterface("配置") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.config; }
    String themePath() { return bruceConfig.theme.paths.config; }

private:
    // Submenus
    void displayUIMenu(void);
    void ledMenu(void);
    void audioMenu(void);
    void systemMenu(void);
    void advancedMenu(void);
    void powerMenu(void);
    void devMenu(void);

    // Helper methods for complex operations
    void switchToUSBSerial(void);
    void switchToUARTSerial(void);
};

#endif
