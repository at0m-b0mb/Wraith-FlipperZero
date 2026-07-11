#include "../wraith_i.h"

typedef enum {
    MacRandAp,
    MacRandSta,
    MacCloneAp,
    MacCloneSta,
} MacIndex;

static void wraith_scene_mac_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// Fire a one-shot command and return to this menu (no console needed).
static void mac_send(WraithApp* app, const char* cmd) {
    wraith_link_ensure(app);
    wraith_link_send(app, cmd);
    wraith_link_send(app, "\n");
}

void wraith_scene_mac_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "MAC");
    submenu_add_item(menu, "Random AP MAC", MacRandAp, wraith_scene_mac_cb, app);
    submenu_add_item(menu, "Random Station MAC", MacRandSta, wraith_scene_mac_cb, app);
    submenu_add_item(menu, "Clone AP MAC #", MacCloneAp, wraith_scene_mac_cb, app);
    submenu_add_item(menu, "Clone Station MAC #", MacCloneSta, wraith_scene_mac_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneMac));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_mac_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneMac, event.event);
        consumed = true;
        switch(event.event) {
        case MacRandAp:
            mac_send(app, MARAUDER_CMD_RANDAPMAC);
            break;
        case MacRandSta:
            mac_send(app, MARAUDER_CMD_RANDSTAMAC);
            break;
        case MacCloneAp:
            wraith_prompt(app, "Clone from AP index", MARAUDER_PFX_CLONEAPMAC, "", "");
            break;
        case MacCloneSta:
            wraith_prompt(app, "Clone from station index", MARAUDER_PFX_CLONESTAMAC, "", "");
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_mac_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
