#include "../wraith_i.h"

typedef enum {
    BleSpamApple,
    BleSpamSamsung,
    BleSpamGoogle,
    BleSpamWindows,
    BleSpamAll,
    BleSourApple,
} BleSpamIndex;

static void wraith_scene_blespam_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_blespam_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "BLE Spam");
    submenu_add_item(menu, "Apple", BleSpamApple, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Samsung", BleSpamSamsung, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Google", BleSpamGoogle, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Windows", BleSpamWindows, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "All Brands", BleSpamAll, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Sour Apple", BleSourApple, wraith_scene_blespam_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneBlespam));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_blespam_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneBlespam, event.event);
        consumed = true;
        switch(event.event) {
        case BleSpamApple:
            wraith_launch(app, "BLE Spam Apple", MARAUDER_CMD_BLE_SPAM_APPLE, true);
            break;
        case BleSpamSamsung:
            wraith_launch(app, "BLE Spam Samsung", MARAUDER_CMD_BLE_SPAM_SAMSUNG, true);
            break;
        case BleSpamGoogle:
            wraith_launch(app, "BLE Spam Google", MARAUDER_CMD_BLE_SPAM_GOOGLE, true);
            break;
        case BleSpamWindows:
            wraith_launch(app, "BLE Spam Windows", MARAUDER_CMD_BLE_SPAM_WINDOWS, true);
            break;
        case BleSpamAll:
            wraith_launch(app, "BLE Spam All", MARAUDER_CMD_BLE_SPAM_ALL, true);
            break;
        case BleSourApple:
            wraith_launch(app, "Sour Apple", MARAUDER_CMD_BLE_SOURAPPLE, true);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_blespam_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
