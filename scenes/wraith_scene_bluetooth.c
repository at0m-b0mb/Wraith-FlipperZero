#include "../wraith_i.h"

typedef enum {
    BtSniff,
    BtSkimmer,
} BtIndex;

static void wraith_scene_bluetooth_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_bluetooth_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Bluetooth");
    submenu_add_item(menu, "Sniff Bluetooth", BtSniff, wraith_scene_bluetooth_cb, app);
    submenu_add_item(menu, "Detect Skimmers", BtSkimmer, wraith_scene_bluetooth_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneBluetooth));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_bluetooth_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneBluetooth, event.event);
        consumed = true;
        switch(event.event) {
        case BtSniff:
            wraith_launch(app, "Sniff Bluetooth", MARAUDER_CMD_BT_SNIFF, false);
            break;
        case BtSkimmer:
            wraith_launch(app, "Detect Skimmers", MARAUDER_CMD_BT_SKIMMER, false);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_bluetooth_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
