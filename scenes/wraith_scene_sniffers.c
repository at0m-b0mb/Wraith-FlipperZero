#include "../wraith_i.h"

typedef enum {
    SnBeacon,
    SnProbe,
    SnDeauth,
    SnPmkid,
    SnPmkidD,
    SnPwn,
    SnRaw,
    SnPine,
    SnMulti,
    SnSae,
} SnIndex;

static void wraith_scene_sniffers_cb(void* context, uint32_t index) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void wraith_scene_sniffers_on_enter(void* context) {
    WraithApp* app = context;
    Submenu* menu = app->submenu;

    submenu_reset(menu);
    submenu_set_header(menu, "Sniffers");
    submenu_add_item(menu, "Beacons", SnBeacon, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Probes", SnProbe, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Deauth", SnDeauth, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "PMKID", SnPmkid, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "PMKID + Deauth", SnPmkidD, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Pwnagotchi", SnPwn, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Raw", SnRaw, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Detect Pineapple", SnPine, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "Detect Multi-SSID", SnMulti, wraith_scene_sniffers_cb, app);
    submenu_add_item(menu, "WPA3 SAE", SnSae, wraith_scene_sniffers_cb, app);

    submenu_set_selected_item(
        menu, scene_manager_get_scene_state(app->scene_manager, WraithSceneSniffers));

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewSubmenu);
}

bool wraith_scene_sniffers_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, WraithSceneSniffers, event.event);
        consumed = true;
        switch(event.event) {
        case SnBeacon:
            wraith_launch(app, "Sniff Beacons", MARAUDER_CMD_SNIFF_BEACON, false);
            break;
        case SnProbe:
            wraith_launch(app, "Sniff Probes", MARAUDER_CMD_SNIFF_PROBE, false);
            break;
        case SnDeauth:
            wraith_launch(app, "Sniff Deauth", MARAUDER_CMD_SNIFF_DEAUTH, false);
            break;
        case SnPmkid:
            wraith_launch(app, "Sniff PMKID", MARAUDER_CMD_SNIFF_PMKID, false);
            break;
        case SnPmkidD:
            wraith_launch(app, "PMKID + Deauth", MARAUDER_CMD_SNIFF_PMKID_D, true);
            break;
        case SnPwn:
            wraith_launch(app, "Sniff Pwnagotchi", MARAUDER_CMD_SNIFF_PWN, false);
            break;
        case SnRaw:
            wraith_launch(app, "Sniff Raw", MARAUDER_CMD_SNIFF_RAW, false);
            break;
        case SnPine:
            wraith_launch(app, "Detect Pineapple", MARAUDER_CMD_SNIFF_PINESCAN, false);
            break;
        case SnMulti:
            wraith_launch(app, "Detect Multi-SSID", MARAUDER_CMD_SNIFF_MULTISSID, false);
            break;
        case SnSae:
            wraith_launch(app, "Sniff WPA3 SAE", MARAUDER_CMD_SNIFF_SAE, false);
            break;
        default:
            consumed = false;
            break;
        }
    }
    return consumed;
}

void wraith_scene_sniffers_on_exit(void* context) {
    WraithApp* app = context;
    submenu_reset(app->submenu);
}
