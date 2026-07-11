#include "../wraith_i.h"

typedef enum {
    EpStart,
    EpSetAp,
} EpIndex;

static void wraith_scene_evilportal_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_evilportal_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Evil Portal");
    submenu_add_item(menu, "Set Portal AP #", EpSetAp, wraith_scene_evilportal_cb, app);
    submenu_add_item(menu, "Start Portal", EpStart, wraith_scene_evilportal_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneEvilportal));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_evilportal_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneEvilportal, event.event);
        consumed = true;
        switch(event.event) {
        case EpSetAp:
            // Point the portal at a scanned AP (run Scan APs first). Fire-and-return.
            wraith_prompt(
                app, "Clone AP index", MARAUDER_PFX_EVILPORTAL_SETAP, "", "");
            break;
        case EpStart:
            // Needs an index.html / portal set up on the SD card.
            wraith_launch(app, "Evil Portal", MARAUDER_CMD_EVILPORTAL_START, true);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_evilportal_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
