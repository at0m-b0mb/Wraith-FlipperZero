#include "../wraith_i.h"
#include <stdio.h>
#include <string.h>

static void wraith_scene_console_ok_cb(void* context) {
    WraithApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, WraithCustomEventConsoleSend);
}

void wraith_scene_console_on_enter(void* context) {
    WraithApp* app = context;
    ConsoleView* cv = app->console_view;

    wraith_link_ensure(app);
    console_view_set_ok_callback(cv, wraith_scene_console_ok_cb, app);
    console_view_set_autoscroll(cv, app->settings.autoscroll);
    console_view_set_channel(
        cv, app->settings.uart_channel == WraithUartLpuart ? "15/16" : "13/14");
    app->console_keep_running = false;

    if(app->pending_cmd[0]) {
        // Fresh op launched from a menu: clear the screen and fire the command.
        console_view_clear(cv);
        console_view_set_header(cv, app->pending_title);

        char line[WRAITH_CMD_MAX];
        snprintf(line, sizeof(line), "%.62s\n", app->pending_cmd);
        wraith_link_send(app, line);
        app->pending_cmd[0] = '\0';
    } else {
        console_view_set_header(cv, app->pending_title[0] ? app->pending_title : "Console");
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewConsole);
}

bool wraith_scene_console_on_event(void* context, SceneManagerEvent event) {
    WraithApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeTick) {
        console_view_set_live(app->console_view, wraith_link_is_live(app));
        console_view_tick(app->console_view);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == WraithCustomEventConsoleSend) {
            app->console_keep_running = true; // dip into the sender, keep the op alive
            scene_manager_next_scene(app->scene_manager, WraithSceneSend);
            consumed = true;
        }
    }
    return consumed;
}

void wraith_scene_console_on_exit(void* context) {
    WraithApp* app = context;
    if(app->console_keep_running) {
        app->console_keep_running = false; // just visiting the sender — leave it running
    } else {
        // Leaving back toward the menus: stop the running Marauder op (keep the link).
        wraith_link_send(app, MARAUDER_CMD_STOP "\n");
    }
}
