#include "../wraith_i.h"

typedef enum {
    GpsData,
    GpsWardrive,
    GpsWardriveSta,
} GpsIndex;

static void wraith_scene_gps_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_gps_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "GPS / Wardrive");
    submenu_add_item(menu, "GPS Data", GpsData, wraith_scene_gps_cb, app);
    submenu_add_item(menu, "Wardrive (AP)", GpsWardrive, wraith_scene_gps_cb, app);
    submenu_add_item(menu, "Wardrive (Station)", GpsWardriveSta, wraith_scene_gps_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneGps));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_gps_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneGps, event.event);
        consumed = true;
        switch(event.event) {
        case GpsData:
            wraith_launch(app, "GPS Data", MARAUDER_CMD_GPS_DATA, false);
            break;
        case GpsWardrive:
            wraith_launch(app, "Wardrive AP", MARAUDER_CMD_WARDRIVE, false);
            break;
        case GpsWardriveSta:
            wraith_launch(app, "Wardrive STA", MARAUDER_CMD_WARDRIVE_STA, false);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_gps_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
