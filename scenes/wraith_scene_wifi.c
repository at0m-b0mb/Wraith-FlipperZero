#include "../wraith_i.h"

typedef enum {
    WifiScanAp,
    WifiScanSta,
    WifiTarget,
    WifiSniffBeacon,
    WifiSniffProbe,
    WifiSniffDeauth,
    WifiSniffPmkid,
    WifiSniffPwn,
    WifiSniffEsp,
    WifiAttacks,
} WifiIndex;

static void wraith_scene_wifi_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_wifi_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Wi-Fi");
    submenu_add_item(menu, "Scan APs", WifiScanAp, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Scan Stations", WifiScanSta, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Targets / Select", WifiTarget, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff Beacons", WifiSniffBeacon, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff Probes", WifiSniffProbe, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff Deauth", WifiSniffDeauth, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff PMKID", WifiSniffPmkid, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff Pwnagotchi", WifiSniffPwn, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniff ESP", WifiSniffEsp, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Attacks", WifiAttacks, wraith_scene_wifi_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneWifi));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_wifi_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneWifi, event.event);
        consumed = true;
        switch(event.event) {
        case WifiScanAp:
            wraith_launch(app, "Scan APs", MARAUDER_CMD_SCAN_AP, false);
            break;
        case WifiScanSta:
            wraith_launch(app, "Scan Stations", MARAUDER_CMD_SCAN_STA, false);
            break;
        case WifiTarget:
            scene_manager_next_scene(app->scene_manager, WraithSceneTarget);
            break;
        case WifiSniffBeacon:
            wraith_launch(app, "Sniff Beacons", MARAUDER_CMD_SNIFF_BEACON, false);
            break;
        case WifiSniffProbe:
            wraith_launch(app, "Sniff Probes", MARAUDER_CMD_SNIFF_PROBE, false);
            break;
        case WifiSniffDeauth:
            wraith_launch(app, "Sniff Deauth", MARAUDER_CMD_SNIFF_DEAUTH, false);
            break;
        case WifiSniffPmkid:
            wraith_launch(app, "Sniff PMKID", MARAUDER_CMD_SNIFF_PMKID, false);
            break;
        case WifiSniffPwn:
            wraith_launch(app, "Sniff Pwnagotchi", MARAUDER_CMD_SNIFF_PWN, false);
            break;
        case WifiSniffEsp:
            wraith_launch(app, "Sniff ESP", MARAUDER_CMD_SNIFF_ESP, false);
            break;
        case WifiAttacks:
            scene_manager_next_scene(app->scene_manager, WraithSceneAttacks);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_wifi_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
