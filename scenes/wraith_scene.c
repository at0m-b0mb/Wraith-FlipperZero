#include "wraith_scene.h"

// on_enter handlers
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const wraith_scene_on_enter_handlers[])(void*) = {
#include "wraith_scene_config.h"
};
#undef ADD_SCENE

// on_event handlers
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const wraith_scene_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "wraith_scene_config.h"
};
#undef ADD_SCENE

// on_exit handlers
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const wraith_scene_on_exit_handlers[])(void* context) = {
#include "wraith_scene_config.h"
};
#undef ADD_SCENE

const SceneManagerHandlers wraith_scene_handlers = {
    .on_enter_handlers = wraith_scene_on_enter_handlers,
    .on_event_handlers = wraith_scene_on_event_handlers,
    .on_exit_handlers = wraith_scene_on_exit_handlers,
    .scene_num = WraithSceneNum,
};
