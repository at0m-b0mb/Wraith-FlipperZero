#include "../wraith_i.h"

typedef enum {
    AnPacket,
    AnMacTrack,
    AnFoxhunt,
    AnKarma,
} AnIndex;

static void wraith_scene_analysis_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_analysis_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Analysis");
    submenu_add_item(menu, "Packet Count", AnPacket, wraith_scene_analysis_cb, app);
    submenu_add_item(menu, "MAC Track", AnMacTrack, wraith_scene_analysis_cb, app);
    submenu_add_item(menu, "Fox Hunt (AP #)", AnFoxhunt, wraith_scene_analysis_cb, app);
    submenu_add_item(menu, "Karma (SSID #)", AnKarma, wraith_scene_analysis_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneAnalysis));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_analysis_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneAnalysis, event.event);
        consumed = true;
        switch(event.event) {
        case AnPacket:
            wraith_launch(app, "Packet Count", MARAUDER_CMD_PACKETCOUNT, false);
            break;
        case AnMacTrack:
            wraith_launch(app, "MAC Track", MARAUDER_CMD_MACTRACK, false);
            break;
        case AnFoxhunt:
            wraith_prompt_run(app, "Fox-hunt AP index", MARAUDER_PFX_FOXHUNT_W, "Fox Hunt", false);
            break;
        case AnKarma:
            wraith_prompt_run(app, "Karma SSID index", MARAUDER_PFX_KARMA, "Karma", true);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_analysis_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
