#include "../wraith_i.h"

typedef enum {
    WifiScanAp,
    WifiScanSta,
    WifiScanAll,
    WifiSetChannel,
    WifiTarget,
    WifiSniffers,
    WifiAnalysis,
    WifiSsidList,
    WifiAttacks,
    WifiEvilPortal,
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
    submenu_add_item(menu, "Scan All", WifiScanAll, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Set Channel", WifiSetChannel, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Targets / Select", WifiTarget, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Sniffers", WifiSniffers, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Analysis", WifiAnalysis, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "SSID List", WifiSsidList, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Attacks", WifiAttacks, wraith_scene_wifi_cb, app);
    submenu_add_item(menu, "Evil Portal", WifiEvilPortal, wraith_scene_wifi_cb, app);

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
        case WifiScanAll:
            wraith_launch(app, "Scan All", MARAUDER_CMD_SCAN_ALL, false);
            break;
        case WifiSetChannel:
            wraith_prompt(
                app, "Wi-Fi channel (e.g. 6, 36)", MARAUDER_PFX_CHANNEL, "Channel",
                MARAUDER_CMD_CHANNEL);
            break;
        case WifiTarget:
            scene_manager_next_scene(app->scene_manager, WraithSceneTarget);
            break;
        case WifiSniffers:
            scene_manager_next_scene(app->scene_manager, WraithSceneSniffers);
            break;
        case WifiAnalysis:
            scene_manager_next_scene(app->scene_manager, WraithSceneAnalysis);
            break;
        case WifiSsidList:
            scene_manager_next_scene(app->scene_manager, WraithSceneSsidlist);
            break;
        case WifiAttacks:
            scene_manager_next_scene(app->scene_manager, WraithSceneAttacks);
            break;
        case WifiEvilPortal:
            scene_manager_next_scene(app->scene_manager, WraithSceneEvilportal);
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
