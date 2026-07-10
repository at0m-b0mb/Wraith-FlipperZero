#include "../wraith_i.h"

typedef enum {
    DevHelp,
    DevSettings,
    DevClearAll,
    DevUpdate,
    DevReboot,
} DevIndex;

static void wraith_scene_device_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_device_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Device");
    submenu_add_item(menu, "Show Commands", DevHelp, wraith_scene_device_cb, app);
    submenu_add_item(menu, "Board Settings", DevSettings, wraith_scene_device_cb, app);
    submenu_add_item(menu, "Clear All Lists", DevClearAll, wraith_scene_device_cb, app);
    submenu_add_item(menu, "Update (SD)", DevUpdate, wraith_scene_device_cb, app);
    submenu_add_item(menu, "Reboot Board", DevReboot, wraith_scene_device_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneDevice));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_device_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneDevice, event.event);
        consumed = true;
        switch(event.event) {
        case DevHelp:
            wraith_launch(app, "Commands", MARAUDER_CMD_HELP, false);
            break;
        case DevSettings:
            wraith_launch(app, "Board Settings", MARAUDER_CMD_SETTINGS, false);
            break;
        case DevClearAll:
            wraith_link_ensure(app);
            wraith_link_send(app, MARAUDER_CMD_CLEAR_AP "\n");
            wraith_link_send(app, MARAUDER_CMD_CLEAR_STA "\n");
            wraith_link_send(app, MARAUDER_CMD_CLEAR_SSID "\n");
            wraith_launch(app, "Lists Cleared", MARAUDER_CMD_LIST_AP, false);
            break;
        case DevUpdate:
            wraith_launch(app, "Update (SD)", MARAUDER_CMD_UPDATE, false);
            break;
        case DevReboot:
            wraith_launch(app, "Reboot", MARAUDER_CMD_REBOOT, false);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_device_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
