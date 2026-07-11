#include "../wraith_i.h"

typedef enum {
    AtkDeauth,
    AtkBadmsg,
    AtkBeaconList,
    AtkBeaconRandom,
    AtkBeaconAp,
    AtkProbe,
    AtkRickroll,
    AtkFunny,
    AtkSae,
    AtkCsa,
    AtkQuiet,
} AtkIndex;

static void wraith_scene_attacks_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_attacks_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Attacks");
    submenu_add_item(menu, "Deauth Flood", AtkDeauth, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Bad Msg", AtkBadmsg, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Beacon Spam (list)", AtkBeaconList, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Beacon Spam (random)", AtkBeaconRandom, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Beacon Spam (AP clone)", AtkBeaconAp, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Probe Flood", AtkProbe, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Rickroll Beacon", AtkRickroll, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Funny Beacon", AtkFunny, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "WPA3 SAE Flood", AtkSae, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Channel Switch (CSA)", AtkCsa, wraith_scene_attacks_cb, app);
    submenu_add_item(menu, "Quiet Flood", AtkQuiet, wraith_scene_attacks_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneAttacks));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_attacks_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneAttacks, event.event);
        consumed = true;
        switch(event.event) {
        case AtkDeauth:
            wraith_launch(app, "Deauth Flood", MARAUDER_CMD_ATTACK_DEAUTH, true);
            break;
        case AtkBadmsg:
            wraith_launch(app, "Bad Msg", MARAUDER_CMD_ATTACK_BADMSG, true);
            break;
        case AtkBeaconList:
            wraith_launch(app, "Beacon (list)", MARAUDER_CMD_ATTACK_BEACON_L, true);
            break;
        case AtkBeaconRandom:
            wraith_launch(app, "Beacon (random)", MARAUDER_CMD_ATTACK_BEACON_R, true);
            break;
        case AtkBeaconAp:
            wraith_launch(app, "Beacon (AP clone)", MARAUDER_CMD_ATTACK_BEACON_AP, true);
            break;
        case AtkProbe:
            wraith_launch(app, "Probe Flood", MARAUDER_CMD_ATTACK_PROBE, true);
            break;
        case AtkRickroll:
            wraith_launch(app, "Rickroll Beacon", MARAUDER_CMD_ATTACK_RICKROLL, true);
            break;
        case AtkFunny:
            wraith_launch(app, "Funny Beacon", MARAUDER_CMD_ATTACK_FUNNY, true);
            break;
        case AtkSae:
            wraith_launch(app, "WPA3 SAE Flood", MARAUDER_CMD_ATTACK_SAE, true);
            break;
        case AtkCsa:
            wraith_launch(app, "Channel Switch", MARAUDER_CMD_ATTACK_CSA, true);
            break;
        case AtkQuiet:
            wraith_launch(app, "Quiet Flood", MARAUDER_CMD_ATTACK_QUIET, true);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_attacks_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
