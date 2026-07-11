#include "../wraith_i.h"

typedef enum {
    StartIndexWifi,
    StartIndexBluetooth,
    StartIndexGps,
    StartIndexNetwork,
    StartIndexDevice,
    StartIndexConsole,
    StartIndexSettings,
    StartIndexAbout,
} StartIndex;

static void wraith_scene_start_submenu_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_start_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_reset(submenu);
    submenu_set_header(submenu, "Wraith");
    submenu_add_item(submenu, "Wi-Fi", StartIndexWifi, wraith_scene_start_submenu_cb, app);
    submenu_add_item(
        submenu, "Bluetooth", StartIndexBluetooth, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "GPS / Wardrive", StartIndexGps, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "Network", StartIndexNetwork, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "Device", StartIndexDevice, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "Console", StartIndexConsole, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "Settings", StartIndexSettings, wraith_scene_start_submenu_cb, app);
    submenu_add_item(submenu, "About", StartIndexAbout, wraith_scene_start_submenu_cb, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, WraithSceneStart));

    // Back at the top menu = idle. Drop the radio link so the UART is free.
    wraith_link_disarm(app);

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_start_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneStart, event.event);
        switch(event.event) {
        case StartIndexWifi:
            scene_manager_next_scene(app->scene_manager, WraithSceneWifi);
            consumed = true;
            break;
        case StartIndexBluetooth:
            scene_manager_next_scene(app->scene_manager, WraithSceneBluetooth);
            consumed = true;
            break;
        case StartIndexGps:
            scene_manager_next_scene(app->scene_manager, WraithSceneGps);
            consumed = true;
            break;
        case StartIndexNetwork:
            scene_manager_next_scene(app->scene_manager, WraithSceneNetwork);
            consumed = true;
            break;
        case StartIndexDevice:
            scene_manager_next_scene(app->scene_manager, WraithSceneDevice);
            consumed = true;
            break;
        case StartIndexConsole:
            // Open the console with no staged command: a free serial terminal.
            app->pending_cmd[0] = '\0';
            strncpy(app->pending_title, "Console", sizeof(app->pending_title) - 1);
            scene_manager_next_scene(app->scene_manager, WraithSceneConsole);
            consumed = true;
            break;
        case StartIndexSettings:
            scene_manager_next_scene(app->scene_manager, WraithSceneSettings);
            consumed = true;
            break;
        case StartIndexAbout:
            scene_manager_next_scene(app->scene_manager, WraithSceneAbout);
            consumed = true;
            break;
        default:
            break;
        }
    }
    return consumed;
}

void wraith_scene_start_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
