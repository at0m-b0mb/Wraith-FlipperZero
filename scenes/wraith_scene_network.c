#include "../wraith_i.h"

typedef enum {
    NetJoin,
    NetPing,
    NetArp,
} NetIndex;

static void wraith_scene_network_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_network_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Network");
    submenu_add_item(menu, "Join AP", NetJoin, wraith_scene_network_cb, app);
    submenu_add_item(menu, "Ping Scan", NetPing, wraith_scene_network_cb, app);
    submenu_add_item(menu, "ARP Scan", NetArp, wraith_scene_network_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneNetwork));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_network_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneNetwork, event.event);
        consumed = true;
        switch(event.event) {
        case NetJoin:
            // Scan APs first; type e.g. "0 -p mypassword" to join AP index 0.
            wraith_prompt(app, "AP index -p <password>", MARAUDER_PFX_JOIN, "Join", "");
            break;
        case NetPing:
            wraith_launch(app, "Ping Scan", MARAUDER_CMD_PINGSCAN, false);
            break;
        case NetArp:
            wraith_launch(app, "ARP Scan", MARAUDER_CMD_ARPSCAN, false);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_network_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
