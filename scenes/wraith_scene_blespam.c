#include "../wraith_i.h"

typedef enum {
    BleSourApple,
    BleAppleJuice,
    BleSamsung,
    BleGoogle,
    BleWindows,
    BleFlipper,
    BleAll,
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
    submenu_add_item(menu, "Sour Apple", BleSourApple, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Apple Juice", BleAppleJuice, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Samsung", BleSamsung, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Google", BleGoogle, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Windows", BleWindows, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "Flipper", BleFlipper, wraith_scene_blespam_cb, app);
    submenu_add_item(menu, "All Brands", BleAll, wraith_scene_blespam_cb, app);

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
        case BleSourApple:
            wraith_launch(app, "Sour Apple", MARAUDER_CMD_BLE_SOURAPPLE, true);
            break;
        case BleAppleJuice:
            wraith_launch(app, "Apple Juice", MARAUDER_CMD_BLE_APPLEJUICE, true);
            break;
        case BleSamsung:
            wraith_launch(app, "BLE Spam Samsung", MARAUDER_CMD_BLE_SAMSUNG, true);
            break;
        case BleGoogle:
            wraith_launch(app, "BLE Spam Google", MARAUDER_CMD_BLE_GOOGLE, true);
            break;
        case BleWindows:
            wraith_launch(app, "BLE Spam Windows", MARAUDER_CMD_BLE_WINDOWS, true);
            break;
        case BleFlipper:
            wraith_launch(app, "BLE Spam Flipper", MARAUDER_CMD_BLE_FLIPPER, true);
            break;
        case BleAll:
            wraith_launch(app, "BLE Spam All", MARAUDER_CMD_BLE_ALL, true);
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
