#include "../wraith_i.h"
#include <stdio.h>
#include <string.h>

static void wraith_scene_send_result(void* context) {
    WraithApp* app = context;
    if(app->input_buf[0]) {
        char line[WRAITH_CMD_MAX];
        snprintf(line, sizeof(line), "%.62s\n", app->input_buf);
        wraith_link_ensure(app);
        wraith_link_send(app, line);
    }
    // Back to the console (which is still showing the live stream).
    scene_manager_previous_scene(app->scene_manager);
}

void wraith_scene_send_on_enter(void* context) {
    WraithApp* app = context;
    TextInput* ti = app->text_input;

    app->input_buf[0] = '\0';
    text_input_reset(ti);
    text_input_set_header_text(ti, "Marauder command");
    text_input_set_result_callback(
        ti, wraith_scene_send_result, app, app->input_buf, sizeof(app->input_buf), true);

    view_dispatcher_switch_to_view(app->view_dispatcher, WraithViewTextInput);
}

bool wraith_scene_send_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void wraith_scene_send_on_exit(void* context) {
    WraithApp* app = context;
    text_input_reset(app->text_input);
}
