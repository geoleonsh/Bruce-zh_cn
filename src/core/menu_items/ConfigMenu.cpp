#include "ConfigMenu.h"
#include "core/i18n/zh_CN.h"
#include "core/display.h"
#include "core/i2c_finder.h"
#include "core/main_menu.h"
#include "core/settings.h"
#include "core/utils.h"
#include "core/wifi/wifi_common.h"
#ifdef HAS_RGB_LED
#include "core/led_control.h"
#endif

/*********************************************************************
**  Function: optionsMenu
**  Main Config menu entry point
**********************************************************************/
void ConfigMenu::optionsMenu() {
    returnToMenu = false;
    while (true) {
        // Check if we need to exit to Main Menu (e.g., DevMode disabled)
        if (returnToMenu) {
            returnToMenu = false; // Reset flag
            return;
        }

        std::vector<Option> localOptions = {
            {tr("Display & UI"),  [this]() { displayUIMenu(); }},
#ifdef HAS_RGB_LED
            {tr("LED Config"),    [this]() { ledMenu(); }      },
#endif
            {tr("Audio Config"),  [this]() { audioMenu(); }    },
            {tr("System Config"), [this]() { systemMenu(); }   },
            {tr("Power"),         [this]() { powerMenu(); }    },
        };
#if !defined(LITE_VERSION)
        if (!appStoreInstalled()) {
            localOptions.push_back({tr("Install App Store"), []() { installAppStoreJS(); }});
        }
#endif

        if (bruceConfig.devMode) {
            localOptions.push_back({tr("Dev Mode"), [this]() { devMenu(); }});
        }

        localOptions.push_back({tr("About"), showDeviceInfo});
        localOptions.push_back({tr("Main Menu"), []() {}});

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Config"));

        // Exit to Main Menu only if user pressed Back
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Otherwise rebuild Config menu after submenu returns
    }
}

/*********************************************************************
**  Function: displayUIMenu
**  Display & UI configuration submenu with auto-rebuild
**********************************************************************/
void ConfigMenu::displayUIMenu() {
    while (true) {
        std::vector<Option> localOptions = {
            {tr("Brightness"),  [this]() { setBrightnessMenu(); }               },
            {tr("Dim Time"),    [this]() { setDimmerTimeMenu(); }               },
            {tr("Orientation"), [this]() { lambdaHelper(gsetRotation, true)(); }},
            {tr("UI Color"),    [this]() { setUIColor(); }                      },
            {tr("UI Theme"),    [this]() { setTheme(); }                        },
            {tr("Back"),        []() {}                                         },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Display & UI"));

        // Exit only if user pressed Back or ESC
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Otherwise loop continues and menu rebuilds
    }
}

/*********************************************************************
**  Function: ledMenu
**  LED configuration submenu with auto-rebuild for toggles
**********************************************************************/
#ifdef HAS_RGB_LED
void ConfigMenu::ledMenu() {
    while (true) {
        std::vector<Option> localOptions = {
            {tr("LED Color"),
             [this]() {
                 beginLed();
                 setLedColorConfig();
             }                                                                            },
            {tr("LED Effect"),
             [this]() {
                 beginLed();
                 setLedEffectConfig();
             }                                                                            },
            {tr("LED Brightness"),
             [this]() {
                 beginLed();
                 setLedBrightnessConfig();
             }                                                                            },
            {String("LED Blink: ") + (bruceConfig.ledBlinkEnabled ? "ON" : "OFF"),
             [this]() {
                 // Toggle LED blink setting
                 bruceConfig.ledBlinkEnabled = !bruceConfig.ledBlinkEnabled;
                 bruceConfig.saveFile();
             }                                                                            },
            {tr("Back"),                                                               []() {}},
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("LED Config"));

        // Exit only if user pressed Back or ESC
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds to update toggle label
    }
}
#endif
/*********************************************************************
**  Function: audioMenu
**  Audio configuration submenu with auto-rebuild for toggles
**********************************************************************/
void ConfigMenu::audioMenu() {
    while (true) {
        std::vector<Option> localOptions = {
#if !defined(LITE_VERSION)
#if defined(BUZZ_PIN) || defined(HAS_NS4168_SPKR)

            {String("Sound: ") + (bruceConfig.soundEnabled ? "ON" : "OFF"),
                                                             [this]() {
                 // Toggle sound setting
                 bruceConfig.soundEnabled = !bruceConfig.soundEnabled;
                 bruceConfig.saveFile();
             }                                                                                                                                            },
#if defined(HAS_NS4168_SPKR)
            {tr("Sound Volume"),                                                [this]() { setSoundVolume(); }},
#endif  // BUZZ_PIN || HAS_NS4168_SPKR
#endif  //  HAS_NS4168_SPKR
#endif  //  LITE_VERSION
            {tr("Back"),                                                        []() {}                       },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Audio Config"));

        // Exit only if user pressed Back or ESC
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds to update toggle label
    }
}

/*********************************************************************
**  Function: systemMenu
**  System configuration submenu with auto-rebuild for toggles
**********************************************************************/
void ConfigMenu::systemMenu() {
    while (true) {
        std::vector<Option> localOptions = {
            {String("InstaBoot: ") + (bruceConfig.instantBoot ? "ON" : "OFF"),
             [this]() {
                 // Toggle InstaBoot setting
                 bruceConfig.instantBoot = !bruceConfig.instantBoot;
                 bruceConfig.saveFile();
             }                                                                                                           },
            {String("WiFi Startup: ") + (bruceConfig.wifiAtStartup ? "ON" : "OFF"),
             [this]() {
                 // Toggle WiFi at startup setting
                 bruceConfig.wifiAtStartup = !bruceConfig.wifiAtStartup;
                 bruceConfig.saveFile();
             }                                                                                                           },
            {tr("Startup App"),                                                         [this]() { setStartupApp(); }        },
            {tr("Hide/Show Apps"),                                                      [this]() { mainMenu.hideAppsMenu(); }},
            {tr("Clock"),                                                               [this]() { setClock(); }             },
            {tr("Advanced"),                                                            [this]() { advancedMenu(); }         },
            {tr("Back"),                                                                []() {}                              },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("System Config"));

        // Exit only if user pressed Back or ESC
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds to update toggle labels
    }
}

/*********************************************************************
**  Function: advancedMenu
**  Advanced settings submenu (nested under System Config)
**********************************************************************/
void ConfigMenu::advancedMenu() {
    while (true) {
        std::vector<Option> localOptions = {
#if !defined(LITE_VERSION)
            {tr("Toggle BLE API"), [this]() { enableBLEAPI(); }       },
            {tr("BadUSB/BLE"),     [this]() { setBadUSBBLEMenu(); }   },
#endif
            {tr("Network Creds"),  [this]() { setNetworkCredsMenu(); }},
            {tr("Factory Reset"),
                                      []() {
                 // Confirmation dialog for destructive action
                 drawMainBorder(true);
                 int8_t choice = displayMessage(
                     "Are you sure you want\nto Factory Reset?\nAll data will be lost!",
                     "No",
                     nullptr,
                     "Yes",
                     TFT_RED
                 );

                 if (choice == 1) {
                     // User confirmed - perform factory reset
                     bruceConfigPins.factoryReset();
                     bruceConfig.factoryReset(); // Restarts ESP
                 }
                 // If cancelled, loop continues and menu rebuilds
             }                                                                             },
            {tr("Back"),           []() {}                            },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Advanced"));

        // Exit to System Config menu
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds after each action
    }
}
/*********************************************************************
**  Function: powerMenu
**  Power management submenu with auto-rebuild
**********************************************************************/
void ConfigMenu::powerMenu() {
    while (true) {
        std::vector<Option> localOptions = {
            {tr("Deep Sleep"), goToDeepSleep          },
            {tr("Sleep"),      setSleepMode           },
            {tr("Restart"),    []() { ESP.restart(); }},
            {tr("Power Off"),
             []() {
                 // Confirmation dialog for power off
                 drawMainBorder(true);
                 int8_t choice = displayMessage("Power Off Device?", "No", nullptr, "Yes", TFT_RED);

                 if (choice == 1) { powerOff(); }
             }                                    },
            {tr("Back"),       []() {}                },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Power Menu"));

        // Exit to Config menu
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds after each action
    }
}

/*********************************************************************
**  Function: devMenu
**  Developer mode menu for advanced hardware configuration
**********************************************************************/
void ConfigMenu::devMenu() {
    while (true) {
        std::vector<Option> localOptions = {
            {tr("I2C Finder"),      [this]() { find_i2c_addresses(); }                      },
            {tr("CC1101 Pins"),     [this]() { setSPIPinsMenu(bruceConfigPins.CC1101_bus); }},
            {tr("NRF24  Pins"),     [this]() { setSPIPinsMenu(bruceConfigPins.NRF24_bus); } },
#if !defined(LITE_VERSION)
            {tr("LoRa Pins"),       [this]() { setSPIPinsMenu(bruceConfigPins.LoRa_bus); }  },
            {tr("W5500 Pins"),      [this]() { setSPIPinsMenu(bruceConfigPins.W5500_bus); } },
#endif
            {tr("SDCard Pins"),     [this]() { setSPIPinsMenu(bruceConfigPins.SDCARD_bus); }},
            {tr("I2C Pins"),        [this]() { setI2CPinsMenu(bruceConfigPins.i2c_bus); }   },
            {tr("UART Pins"),       [this]() { setUARTPinsMenu(bruceConfigPins.uart_bus); } },
            {tr("GPS Pins"),        [this]() { setUARTPinsMenu(bruceConfigPins.gps_bus); }  },
            {tr("Serial USB"),      [this]() { switchToUSBSerial(); }                       },
            {tr("Serial UART"),     [this]() { switchToUARTSerial(); }                      },
            {tr("Disable DevMode"), [this]() { bruceConfig.setDevMode(false); }             },
            {tr("Back"),            []() {}                                                 },
        };

        int selected = loopOptions(localOptions, MENU_TYPE_SUBMENU, tr("Dev Mode"));

        // Check if "Disable DevMode" was pressed (second-to-last option)
        if (selected == localOptions.size() - 2) {
            returnToMenu = true; // Signal to exit all Config menus
            return;
        }

        // Exit to Config menu on Back or ESC
        if (selected == -1 || selected == localOptions.size() - 1) { return; }
        // Menu rebuilds after each action
    }
}

/*********************************************************************
**  Function: switchToUSBSerial
**  Switch serial output to USB Serial
**********************************************************************/
void ConfigMenu::switchToUSBSerial() {
    USBserial.setSerialOutput(&Serial);
    Serial1.end();
}

/*********************************************************************
**  Function: switchToUARTSerial
**  Switch serial output to UART (handles pin conflicts)
**********************************************************************/
void ConfigMenu::switchToUARTSerial() {
    // Check and resolve SD card pin conflicts
    if (bruceConfigPins.SDCARD_bus.checkConflict(bruceConfigPins.uart_bus.rx) ||
        bruceConfigPins.SDCARD_bus.checkConflict(bruceConfigPins.uart_bus.tx)) {
        sdcardSPI.end();
    }

    // Check and resolve CC1101/NRF24 pin conflicts
    if (bruceConfigPins.CC1101_bus.checkConflict(bruceConfigPins.uart_bus.rx) ||
        bruceConfigPins.CC1101_bus.checkConflict(bruceConfigPins.uart_bus.tx) ||
        bruceConfigPins.NRF24_bus.checkConflict(bruceConfigPins.uart_bus.rx) ||
        bruceConfigPins.NRF24_bus.checkConflict(bruceConfigPins.uart_bus.tx)) {
        CC_NRF_SPI.end();
    }

    // Configure UART pins and switch serial output
    pinMode(bruceConfigPins.uart_bus.rx, INPUT);
    pinMode(bruceConfigPins.uart_bus.tx, OUTPUT);
    Serial1.begin(115200, SERIAL_8N1, bruceConfigPins.uart_bus.rx, bruceConfigPins.uart_bus.tx);
    USBserial.setSerialOutput(&Serial1);
}
/*********************************************************************
**  Function: drawIcon
**  Draw config gear icon
**********************************************************************/
void ConfigMenu::drawIcon(float scale) {
    clearIconArea();
    int radius = scale * 9;

    // Draw 6 gear teeth segments
    for (int i = 0; i < 6; i++) {
        tft.drawArc(
            iconCenterX,
            iconCenterY,
            3.5 * radius,
            2 * radius,
            15 + 60 * i,
            45 + 60 * i,
            bruceConfig.priColor,
            bruceConfig.bgColor,
            true
        );
    }

    // Draw inner circle
    tft.drawArc(
        iconCenterX,
        iconCenterY,
        2.5 * radius,
        radius,
        0,
        360,
        bruceConfig.priColor,
        bruceConfig.bgColor,
        false
    );
}
