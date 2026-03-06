#include "WifiMenu.h"
#include "core/i18n/zh_CN.h"
#include "core/display.h"
#include "core/settings.h"
#include "core/utils.h"
#include "core/wifi/webInterface.h"
#include "core/wifi/wg.h"
#include "core/wifi/wifi_common.h"
#include "core/wifi/wifi_mac.h"
#include "modules/ethernet/ARPScanner.h"
#include "modules/wifi/ap_info.h"
#include "modules/wifi/clients.h"
#include "modules/wifi/evil_portal.h"
#include "modules/wifi/karma_attack.h"
#include "modules/wifi/responder.h"
#include "modules/wifi/scan_hosts.h"
#include "modules/wifi/sniffer.h"
#include "modules/wifi/wifi_atks.h"



#ifndef LITE_VERSION
#include "modules/wifi/wifi_recover.h"
#include "modules/pwnagotchi/pwnagotchi.h"
#endif

// #include "modules/reverseShell/reverseShell.h"
//  Developed by Fourier (github.com/9dl)
//  Use BruceC2 to interact with the reverse shell server
//  BruceC2: https://github.com/9dl/Bruce-C2
//  To use BruceC2:
//  1. Start Reverse Shell Mode in Bruce
//  2. Start BruceC2 and wait.
//  3. Visit 192.168.4.1 in your browser to access the web interface for shell executing.

// 32bit: https://github.com/9dl/Bruce-C2/releases/download/v1.0/BruceC2_windows_386.exe
// 64bit: https://github.com/9dl/Bruce-C2/releases/download/v1.0/BruceC2_windows_amd64.exe
#include "modules/wifi/tcp_utils.h"

// global toggle - controls whether scanNetworks includes hidden SSIDs
bool showHiddenNetworks = false;

void WifiMenu::optionsMenu() {
    returnToMenu = false;
    options.clear();
    if (isWebUIActive) {
        drawMainBorderWithTitle("WiFi", true);
        padprintln(tr(""));
        padprintln(tr("Starting a Wifi function will probably make the WebUI stop working"));
        padprintln(tr(""));
        padprintln(tr("Sel: to continue"));
        padprintln(tr("Any key: to Menu"));
        while (1) {
            if (check(SelPress)) { break; }
            if (check(AnyKeyPress)) { return; }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    if (WiFi.status() != WL_CONNECTED) {
        options = {
            {tr("Connect to Wifi"), lambdaHelper(wifiConnectMenu, WIFI_STA)},
            {tr("Start WiFi AP"), [=]() {
                 wifiConnectMenu(WIFI_AP);
                 displayInfo(tr("pwd: ") + bruceConfig.wifiAp.pwd, true);
             }},
        };
    }
    if (WiFi.getMode() != WIFI_MODE_NULL) { options.push_back({tr("Turn Off WiFi"), wifiDisconnect}); }
    if (WiFi.getMode() == WIFI_MODE_STA || WiFi.getMode() == WIFI_MODE_APSTA) {
        options.push_back({tr("AP info"), displayAPInfo});
    }
    options.push_back({tr("Wifi Atks"), wifi_atk_menu});
    options.push_back({tr("Evil Portal"), [=]() {
                           if (isWebUIActive || server) {
                               stopWebUi();
                               wifiDisconnect();
                           }
                           EvilPortal();
                       }});
    // options.push_back({tr("ReverseShell"), [=]()       { ReverseShell(); }});
#ifndef LITE_VERSION
    options.push_back({tr("Listen TCP"), listenTcpPort});
    options.push_back({tr("Client TCP"), clientTCP});
    options.push_back({tr("TelNET"), telnet_setup});
    options.push_back({tr("SSH"), lambdaHelper(ssh_setup, String(""))});
    options.push_back({tr("Sniffers"), [this]() {
                           std::vector<Option> snifferOptions;
                           snifferOptions.push_back({tr("Raw Sniffer"), sniffer_setup});
                           snifferOptions.push_back({tr("Probe Sniffer"), karma_setup});
                           snifferOptions.push_back({tr("Back"), [this]() { optionsMenu(); }});

                           loopOptions(snifferOptions, MENU_TYPE_SUBMENU, tr("Sniffers"));
                       }});
    options.push_back({tr("Scan Hosts"), [=]() {
                           bool doScan = true;
                           if (!wifiConnected) doScan = wifiConnectMenu();

                           if (doScan) {
                               esp_netif_t *esp_netinterface =
                                   esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
                               if (esp_netinterface == nullptr) {
                                   Serial.println(tr("Failed to get netif handle"));
                                   return;
                               }
                               ARPScanner{esp_netinterface};
                           }
                       }});
    options.push_back({tr("Wireguard"), wg_setup});
    options.push_back({tr("Responder"), responder});
    options.push_back({tr("Brucegotchi"), brucegotchi_start});
    options.push_back({tr("WiFi Pass Recovery"), wifi_recover_menu});
#endif
    
    options.push_back({tr("Config"), [this]() { configMenu(); }});

    addOptionToMainMenu();

    loopOptions(options, MENU_TYPE_SUBMENU, tr("WiFi"));

    options.clear();
}

void WifiMenu::configMenu() {
    std::vector<Option> wifiOptions;

    wifiOptions.push_back({tr("Change MAC"), wifiMACMenu});
    wifiOptions.push_back({tr("Add Evil Wifi"), addEvilWifiMenu});
    wifiOptions.push_back({tr("Remove Evil Wifi"), removeEvilWifiMenu});

    // Evil Wifi Settings submenu (unchanged)
    wifiOptions.push_back({tr("Evil Wifi Settings"), [this]() {
                               std::vector<Option> evilOptions;

                               evilOptions.push_back({tr("Password Mode"), setEvilPasswordMode});
                               evilOptions.push_back({tr("Rename /creds"), setEvilEndpointCreds});
                               evilOptions.push_back({tr("Allow /creds access"), setEvilAllowGetCreds});
                               evilOptions.push_back({tr("Rename /ssid"), setEvilEndpointSsid});
                               evilOptions.push_back({tr("Allow /ssid access"), setEvilAllowSetSsid});
                               evilOptions.push_back({tr("Display endpoints"), setEvilAllowEndpointDisplay});
                               evilOptions.push_back({tr("Back"), [this]() { configMenu(); }});
                               loopOptions(evilOptions, MENU_TYPE_SUBMENU, tr("Evil Wifi Settings"));
                           }});

    {

        String hidden__wifi_option = String("Hidden Networks:") + (showHiddenNetworks ? "ON" : "OFF");

        // construct Option explicitly using char* label
        Option opt(hidden__wifi_option.c_str(), [this]() {
            showHiddenNetworks = !showHiddenNetworks;
            displayInfo(String("Hidden Networks:") + (showHiddenNetworks ? "ON" : "OFF"), true);
            configMenu();
        });

        wifiOptions.push_back(opt);
    }
    wifiOptions.push_back({tr("Back"), [this]() { optionsMenu(); }});
    loopOptions(wifiOptions, MENU_TYPE_SUBMENU, tr("WiFi Config"));
}

void WifiMenu::drawIcon(float scale) {
    clearIconArea();
    int deltaY = scale * 20;
    int radius = scale * 6;

    tft.fillCircle(iconCenterX, iconCenterY + deltaY, radius, bruceConfig.priColor);
    tft.drawArc(
        iconCenterX,
        iconCenterY + deltaY,
        deltaY + radius,
        deltaY,
        130,
        230,
        bruceConfig.priColor,
        bruceConfig.bgColor
    );
    tft.drawArc(
        iconCenterX,
        iconCenterY + deltaY,
        2 * deltaY + radius,
        2 * deltaY,
        130,
        230,
        bruceConfig.priColor,
        bruceConfig.bgColor
    );
}
