#include "../wraith_i.h"

void wraith_scene_about_on_enter(void* context) {
    WraithApp* app = context;
    Widget* widget = app->widget;

    widget_reset(widget);
    widget_add_icon_element(widget, 2, 2, &I_wraith_10px);
    widget_add_string_element(
        widget, 16, 2, AlignLeft, AlignTop, FontPrimary, "Wraith v" WRAITH_VERSION);
    widget_add_string_element(
        widget, 16, 14, AlignLeft, AlignTop, FontSecondary, "Marauder controller");

    widget_add_text_scroll_element(
        widget,
        0,
        26,
        128,
        38,
        "A clean front-end for an ESP32\n"
        "Marauder board: dual-band\n"
        "2.4/5 GHz Wi-Fi, BLE, GPS, 433.\n"
        " \n"
        "The Flipper is the UI; the board\n"
        "is the radio. They talk over the\n"
        "GPIO UART at 115200.\n"
        " \n"
        "Wiring (Settings > UART pins):\n"
        "  13/14 - standard dev boards\n"
        "  15/16 - dual-band / GPS boards\n"
        " \n"
        "Typical flow: Scan APs, then\n"
        "Targets to select, then Attacks.\n"
        "Set Channel to lock a band.\n"
        " \n"
        "SSID List builds names for\n"
        "beacon spam; Device has help,\n"
        "clear-all, update and reboot.\n"
        " \n"
        "Bluetooth: sniff, skimmer and\n"
        "AirTag scan, plus BLE Spam\n"
        "(Apple/Samsung/Google/Win).\n"
        " \n"
        "Console sends any raw command.\n"
        " \n"
        "Note: the 433 MHz module runs\n"
        "through the Flipper's own Sub-\n"
        "GHz app (External), not Wraith.\n"
        " \n"
        "Speaks the upstream ESP32\n"
        "Marauder CLI by justcallmekoko.\n"
        " \n"
        "For authorised testing only - use\n"
        "it on networks you own or have\n"
        "explicit permission to assess.\n"
        " \n"
        "by at0m-b0mb\n"
        "github.com/at0m-b0mb/Wraith-FlipperZero");

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewWidget);
}

bool wraith_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void wraith_scene_about_on_exit(void* context) {
    WraithApp* app = context;
    widget_reset(app->widget);
}
