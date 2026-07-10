#include "../wraith_i.h"
#include <stdio.h>

typedef enum {
    SsidList,
    SsidGen,
    SsidNamed,
    SsidRemove,
    SsidClear,
} SsidIndex;

static void wraith_scene_ssidlist_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_ssidlist_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "SSID List");
    submenu_add_item(menu, "List SSIDs", SsidList, wraith_scene_ssidlist_cb, app);
    submenu_add_item(menu, "Add Random SSIDs", SsidGen, wraith_scene_ssidlist_cb, app);
    submenu_add_item(menu, "Add Named SSID", SsidNamed, wraith_scene_ssidlist_cb, app);
    submenu_add_item(menu, "Remove SSID by #", SsidRemove, wraith_scene_ssidlist_cb, app);
    submenu_add_item(menu, "Clear SSIDs", SsidClear, wraith_scene_ssidlist_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneSsidlist));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_ssidlist_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneSsidlist, event.event);
        consumed = true;
        switch(event.event) {
        case SsidList:
            wraith_launch(app, "SSID List", MARAUDER_CMD_LIST_SSID, false);
            break;
        case SsidGen:
            wraith_prompt(app, "How many random SSIDs?", MARAUDER_PFX_SSID_GEN, "SSID List",
                          MARAUDER_CMD_LIST_SSID);
            break;
        case SsidNamed:
            wraith_prompt(app, "SSID name", MARAUDER_PFX_SSID_NAME, "SSID List",
                          MARAUDER_CMD_LIST_SSID);
            break;
        case SsidRemove:
            wraith_prompt(app, "Remove SSID index", MARAUDER_PFX_SSID_REMOVE, "SSID List",
                          MARAUDER_CMD_LIST_SSID);
            break;
        case SsidClear: {
            wraith_link_ensure(app);
            wraith_link_send(app, MARAUDER_CMD_CLEAR_SSID "\n");
            wraith_launch(app, "SSID List", MARAUDER_CMD_LIST_SSID, false);
            break;
        }
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_ssidlist_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
