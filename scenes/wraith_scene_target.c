#include "../wraith_i.h"
#include <stdio.h>

typedef enum {
    TgtListAp,
    TgtSelectAp,
    TgtSelectAllAp,
    TgtClearAp,
    TgtSaveAp,
    TgtLoadAp,
    TgtListSta,
    TgtSelectSta,
    TgtClearSta,
    TgtSaveSta,
    TgtLoadSta,
    TgtMac,
} TgtIndex;

static void wraith_scene_target_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// Fire a one-shot command, then drop into the console showing the refreshed list.
static void quick(WraithApp* app, const char* cmd, const char* list_title, const char* list_cmd) {
    char line[WRAITH_CMD_MAX];
    wraith_link_ensure(app);
    snprintf(line, sizeof(line), "%.60s\n", cmd);
    wraith_link_send(app, line);
    wraith_launch(app, list_title, list_cmd, false);
}

void wraith_scene_target_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Targets");
    submenu_add_item(menu, "List APs", TgtListAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Select AP by #", TgtSelectAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Select ALL APs", TgtSelectAllAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Clear AP list", TgtClearAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Save APs (SD)", TgtSaveAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Load APs (SD)", TgtLoadAp, wraith_scene_target_cb, app);
    submenu_add_item(menu, "List Stations", TgtListSta, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Select Station by #", TgtSelectSta, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Clear Station list", TgtClearSta, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Save Stations (SD)", TgtSaveSta, wraith_scene_target_cb, app);
    submenu_add_item(menu, "Load Stations (SD)", TgtLoadSta, wraith_scene_target_cb, app);
    submenu_add_item(menu, "MAC Spoofing", TgtMac, wraith_scene_target_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneTarget));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_target_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneTarget, event.event);
        consumed = true;
        switch(event.event) {
        case TgtListAp:
            wraith_launch(app, "AP List", MARAUDER_CMD_LIST_AP, false);
            break;
        case TgtSelectAp:
            app->select_kind = 'a';
            scene_manager_next_scene(app->scene_manager, WraithSceneSelect);
            break;
        case TgtSelectAllAp:
            quick(app, MARAUDER_CMD_SELECT_AP_ALL, "AP List", MARAUDER_CMD_LIST_AP);
            break;
        case TgtClearAp:
            quick(app, MARAUDER_CMD_CLEAR_AP, "AP List", MARAUDER_CMD_LIST_AP);
            break;
        case TgtSaveAp:
            quick(app, MARAUDER_CMD_SAVE_AP, "AP List", MARAUDER_CMD_LIST_AP);
            break;
        case TgtLoadAp:
            quick(app, MARAUDER_CMD_LOAD_AP, "AP List", MARAUDER_CMD_LIST_AP);
            break;
        case TgtListSta:
            wraith_launch(app, "Station List", MARAUDER_CMD_LIST_STA, false);
            break;
        case TgtSelectSta:
            app->select_kind = 's';
            scene_manager_next_scene(app->scene_manager, WraithSceneSelect);
            break;
        case TgtClearSta:
            quick(app, MARAUDER_CMD_CLEAR_STA, "Station List", MARAUDER_CMD_LIST_STA);
            break;
        case TgtSaveSta:
            quick(app, MARAUDER_CMD_SAVE_STA, "Station List", MARAUDER_CMD_LIST_STA);
            break;
        case TgtLoadSta:
            quick(app, MARAUDER_CMD_LOAD_STA, "Station List", MARAUDER_CMD_LIST_STA);
            break;
        case TgtMac:
            scene_manager_next_scene(app->scene_manager, WraithSceneMac);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_target_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
