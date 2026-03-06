#pragma once
/*
 * zh_CN.h - Simplified Chinese (UTF-8) localization for Bruce Firmware
 *
 * HOW IT WORKS:
 *   TFT_eSPI supports UTF-8 rendering when a Unicode-compatible font is loaded.
 *   This header provides:
 *     1. A compile-time string-to-Chinese mapping via #defines (ZH_ prefixed macros).
 *     2. A runtime helper  tr(const char*)  that looks up the map and falls back to
 *        the original English string when no translation exists.
 *
 *   To activate Chinese UI:
 *     a) Add  #define LANG_ZH_CN  to your platformio.ini build_flags (or in config).
 *     b) Include this header in display.cpp / main_menu.cpp / menu_items/*.cpp.
 *     c) Wrap every user-visible string literal with  tr("English text") .
 *        e.g.   tft.drawCentreString(tr("WiFi"), x, y, font);
 *     d) Make sure a CJK-capable font is compiled in (see note below).
 *
 * FONT NOTE:
 *   The default TFT_eSPI built-in fonts do NOT contain CJK glyphs.
 *   You must add a Unicode font.  Two practical options:
 *
 *   Option A – NotoSansCJK subset via lvgl/tft_espi converter:
 *     Place  NotoSansSC_Regular_16.h  (pre-converted) in src/core/i18n/ and include it.
 *     Then call  tft.loadFont(NotoSansSC_Regular_16);  once in setup().
 *
 *   Option B – Smooth font from SPIFFS/SD at runtime:
 *     Copy a .vlw font file to the filesystem and call:
 *       tft.loadFont("NotoSansSC16", SD);
 *     This is the most flexible approach and does NOT inflate flash.
 *
 * This file only provides the translation table.
 * Font loading is handled in startup_app.cpp / display.cpp.
 */

#ifdef LANG_ZH_CN

#include <Arduino.h>
#include <map>

// ─────────────────────────────────────────────
//  Translation table  (English → Simplified Chinese)
// ─────────────────────────────────────────────
static const std::map<String, String> _zh_translations = {
    // ── Main menu items ──────────────────────────────────────────────
    {"WiFi",              "无线网络"},
    {"BLE",               "蓝牙"},
    {"Ethernet",          "以太网"},
    {"RF",                "射频"},
    {"RFID",              "射频识别"},
    {"IR",                "红外"},
    {"FM",                "调频"},
    {"Files",             "文件"},
    {"GPS",               "定位"},
    {"NRF24",             "NRF24"},
    {"JS Interpreter",    "JS 解释器"},
    {"LoRa",              "LoRa"},
    {"Others",            "其他"},
    {"Clock",             "时钟"},
    {"Connect",           "连接"},
    {"Config",            "配置"},
    {"Main Menu",         "主菜单"},

    // ── Common buttons / labels ──────────────────────────────────────
    {"Back",              "返回"},
    {"Exit",              "退出"},
    {"PREV",              "上一"},
    {"SEL",               "选择"},
    {"NEXT",              "下一"},
    {"UP",                "向上"},
    {"DOWN",              "向下"},
    {"OK",                "确定"},
    {"Cancel",            "取消"},
    {"Yes",               "是"},
    {"No",                "否"},
    {"Default",           "默认"},
    {"Custom",            "自定义"},
    {"Information",       "信息"},
    {"About",             "关于"},
    {"Show All",          "显示全部"},

    // ── WiFi submenu ─────────────────────────────────────────────────
    {"Connect to Wifi",   "连接 Wi-Fi"},
    {"Start WiFi AP",     "开启热点"},
    {"Turn Off WiFi",     "关闭 Wi-Fi"},
    {"AP info",           "热点信息"},
    {"Wifi Atks",         "Wi-Fi 攻击"},
    {"Evil Portal",       "钓鱼门户"},
    {"Listen TCP",        "监听 TCP"},
    {"Client TCP",        "TCP 客户端"},
    {"TelNET",            "Telnet"},
    {"SSH",               "SSH"},
    {"Sniffers",          "嗅探器"},
    {"Raw Sniffer",       "原始嗅探"},
    {"Probe Sniffer",     "探针嗅探"},
    {"Scan Hosts",        "扫描主机"},
    {"Scan WiFi Networks","扫描 Wi-Fi"},
    {"Wireguard",         "WireGuard"},
    {"Responder",         "应答器"},
    {"Brucegotchi",       "Brucegotchi"},
    {"WiFi Pass Recovery","Wi-Fi 密码恢复"},
    {"Change MAC",        "更改 MAC"},
    {"Add Evil Wifi",     "添加恶意热点"},
    {"Remove Evil Wifi",  "删除恶意热点"},
    {"Evil Wifi Settings","恶意热点设置"},
    {"Password Mode",     "密码模式"},
    {"Rename /creds",     "重命名 /creds"},
    {"Allow /creds access","允许 /creds 访问"},
    {"Rename /ssid",      "重命名 /ssid"},
    {"Allow /ssid access","允许 /ssid 访问"},
    {"Display endpoints", "显示端点"},
    {"WebUI",             "网页界面"},
    {"Wardriving",        "无线战驾"},
    {"Deauther",          "取消认证"},
    {"Scan Both",         "全部扫描"},
    {"Karma Attack",      "Karma 攻击"},
    {"Disconnect",        "断开连接"},

    // ── BLE submenu ──────────────────────────────────────────────────
    {"BLE Scan",          "蓝牙扫描"},
    {"Scan BLE Devices",  "扫描蓝牙设备"},
    {"BLE Spam",          "蓝牙垃圾"},
    {"Bad BLE",           "恶意蓝牙"},
    {"BLE Keyboard",      "蓝牙键盘"},
    {"BLE Name",          "蓝牙名称"},
    {"iBeacon",           "iBeacon"},
    {"Ninebot",           "Ninebot"},
    {"Toggle BLE API",    "切换蓝牙 API"},
    {"BadUSB/BLE",        "BadUSB/蓝牙"},
    {"Change username",   "更改用户名"},
    {"Chat",              "聊天"},
    {"Brdcast std",       "标准广播"},
    {"Brdcast rsvd",      "保留广播"},
    {"Brdcast stop",      "停止广播"},
    {"Send Cmds",         "发送命令"},
    {"Recv Cmds",         "接收命令"},
    {"Send File",         "发送文件"},
    {"Recv File",         "接收文件"},

    // ── RF submenu ───────────────────────────────────────────────────
    {"Record",            "录制"},
    {"Record RAW",        "录制原始"},
    {"Bruteforce",        "暴力破解"},
    {"RF Frequency",      "射频频率"},
    {"RF Module",         "射频模块"},
    {"RF RX Pin",         "射频接收引脚"},
    {"RF TX Pin",         "射频发送引脚"},
    {"Custom SubGhz",     "自定义亚GHz"},
    {"Spectrum",          "频谱"},
    {"RSSI Spectrum",     "RSSI 频谱"},
    {"SquareWave Spec",   "方波频谱"},
    {"Change Frequency",  "更改频率"},
    {"Jammer Full",       "全频干扰"},
    {"Jammer Itmt",       "间歇干扰"},
    {"Listen",            "监听"},
    {"Scan/copy",         "扫描/复制"},
    {"Save",              "保存"},
    {"Emit",              "发射"},
    {"RF Waterfall",      "射频瀑布图"},

    // ── RFID submenu ─────────────────────────────────────────────────
    {"Read tag",          "读取标签"},
    {"Scan tags",         "扫描标签"},
    {"Write NDEF",        "写入 NDEF"},
    {"Chameleon",         "变色龙"},
    {"Amiibolink",        "Amiibolink"},
    {"PN532 UART",        "PN532 串口"},
    {"PN532 BLE",         "PN532 蓝牙"},
    {"RFID Module",       "RFID 模块"},
    {"Read 125kHz",       "读取 125kHz"},
    {"Read EMV",          "读取 EMV"},
    {"SRIX Tool",         "SRIX 工具"},
    {"Add MIF Key",       "添加 MIFARE 密钥"},

    // ── IR submenu ───────────────────────────────────────────────────
    {"IR Read",           "红外读取"},
    {"IR Jammer",         "红外干扰"},
    {"TV-B-Gone",         "TV-B-Gone"},
    {"Custom IR",         "自定义红外"},
    {"Ir RX Pin",         "红外接收引脚"},
    {"Ir TX Pin",         "红外发送引脚"},
    {"Ir TX Repeats",     "红外发送重复次数"},

    // ── GPS submenu ──────────────────────────────────────────────────
    {"GPS Tracker",       "GPS 追踪"},
    {"GPS Pins",          "GPS 引脚"},

    // ── NRF24 submenu ────────────────────────────────────────────────
    {"CH Jammer",         "信道干扰"},
    {"CH hopper",         "信道跳变"},
    {"NRF Jammer",        "NRF 干扰"},
    {"Spectogram",        "声谱图"},
    {"NRF24 (legacy)",    "NRF24 (传统)"},
    {"NRF24 (shared SPI)","NRF24 (共享 SPI)"},
    {"NRF24  Pins",       "NRF24 引脚"},

    // ── LoRa submenu ─────────────────────────────────────────────────
    {"LoRa Pins",         "LoRa 引脚"},

    // ── Others submenu ───────────────────────────────────────────────
    {"iButton",           "iButton"},
    {"QRCodes",           "二维码"},
    {"Microphone",        "麦克风"},
    {"USB Clicker",       "USB 点击器"},
    {"USB Keyboard",      "USB 键盘"},
    {"Timer",             "计时器"},
    {"Megalodon",         "Megalodon"},
    {"BadUSB",            "BadUSB"},
    {"BadUSB & HID",      "BadUSB & HID"},
    {"Media Cmds",        "媒体命令"},
    {"Presenter",         "演示器"},

    // ── Files submenu ────────────────────────────────────────────────
    {"SD Card",           "SD 卡"},
    {"LittleFS",          "LittleFS"},
    {"Erase data",        "清除数据"},

    // ── Clock submenu ────────────────────────────────────────────────
    {"Back to Clock",     "返回时钟"},

    // ── Connect submenu ──────────────────────────────────────────────
    {"Serial UART",       "串口 UART"},
    {"Serial USB",        "USB 串口"},
    {"Baudrate",          "波特率"},
    {"UART Pins",         "UART 引脚"},
    {"Hijack TA",         "劫持 TA"},

    // ── Config submenu ───────────────────────────────────────────────
    {"Display & UI",      "显示与界面"},
    {"LED Config",        "LED 配置"},
    {"Audio Config",      "音频配置"},
    {"System Config",     "系统配置"},
    {"Power",             "电源"},
    {"Dev Mode",          "开发者模式"},
    {"Disable DevMode",   "关闭开发者模式"},
    {"Install App Store", "安装应用商店"},
    {"Brightness",        "亮度"},
    {"Dim Time",          "熄屏时间"},
    {"Orientation",       "屏幕方向"},
    {"UI Color",          "界面颜色"},
    {"UI Theme",          "界面主题"},
    {"LED Color",         "LED 颜色"},
    {"LED Effect",        "LED 效果"},
    {"LED Brightness",    "LED 亮度"},
    {"Sound Volume",      "音量"},
    {"Startup App",       "启动应用"},
    {"Hide/Show Apps",    "隐藏/显示应用"},
    {"Advanced",          "高级"},
    {"Toggle BLE API",    "切换蓝牙 API"},
    {"Network Creds",     "网络凭据"},
    {"Factory Reset",     "恢复出厂设置"},
    {"Deep Sleep",        "深度休眠"},
    {"Sleep",             "休眠"},
    {"Restart",           "重启"},
    {"Power Off",         "关机"},
    {"I2C Finder",        "I2C 扫描"},
    {"CC1101 Pins",       "CC1101 引脚"},
    {"SDCard Pins",       "SD 卡引脚"},
    {"W5500 Pins",        "W5500 引脚"},
    {"I2C Pins",          "I2C 引脚"},
    {"Config pins",       "配置引脚"},

    // ── Ethernet submenu ─────────────────────────────────────────────
    {"Scan Hosts",        "扫描主机"},
    {"DHCP Starvation",   "DHCP 耗尽"},
    {"ARP Poisoner",      "ARP 投毒"},
    {"ARP Scanner",       "ARP 扫描"},
    {"ARP Spoofer",       "ARP 欺骗"},
    {"MAC Flooding",      "MAC 泛洪"},

    // ── Scripts submenu ──────────────────────────────────────────────
    {"ReverseShell",      "反向 Shell"},

    // ── Status / notification strings ────────────────────────────────
    {"Running, Wait",     "运行中，请稍候"},
    {"File not found",    "文件未找到"},
    {"Image not supported","图像格式不支持"},
    {"Dev Mode Enabled",  "开发者模式已开启"},
    {"Selected: ",        "已选择: "},
    {"SD",                "SD"},
    {"Mass Storage",      "大容量存储"},
    {"Sniffer",           "嗅探器"},
    {"Invalid startup app: ","无效启动应用: "},
};

/*
 * tr() – runtime translation lookup.
 * Returns the Chinese string if found, otherwise returns the original.
 */
inline const char* tr(const char* key) {
    if (!key) return key;
    auto it = _zh_translations.find(String(key));
    if (it != _zh_translations.end()) {
        return it->second.c_str();
    }
    return key;
}

inline String tr(const String& key) {
    auto it = _zh_translations.find(key);
    if (it != _zh_translations.end()) {
        return it->second;
    }
    return key;
}

#else
// When LANG_ZH_CN is not defined, tr() is a no-op pass-through
inline const char* tr(const char* key) { return key; }
inline String tr(const String& key) { return key; }
#endif // LANG_ZH_CN
