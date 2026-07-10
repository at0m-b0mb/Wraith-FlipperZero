#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "wraith_icons.h" // generated from icons/ by fbt

#include "helpers/marauder.h"
#include "helpers/marauder_uart.h"
#include "views/console_view.h"
#include "scenes/wraith_scene.h"

#define WRAITH_VERSION   "1.0"
#define WRAITH_CMD_MAX   64
#define WRAITH_TITLE_MAX 24
#define WRAITH_LINK_TIMEOUT_MS \
    1500u // no serial byte for this long -> link shown as idle, not live

typedef enum {
    WraithViewSubmenu, // start + every sub-menu
    WraithViewConsole, // live serial console (the workhorse)
    WraithViewVarList, // settings
    WraithViewTextInput, // send raw command / enter a target index
    WraithViewWidget, // about + attack confirmation
} WraithViewId;

typedef enum {
    // submenu item ids are sent verbatim as custom events, so keep app events high
    WraithCustomEventConfirmStart = 1000,
    WraithCustomEventConsoleSend,
    WraithCustomEventSelectApplied,
    WraithCustomEventInputDone,
} WraithCustomEvent;

typedef enum {
    WraithUartUsart = 0, // GPIO 13 TX / 14 RX  (standard WiFi dev board)
    WraithUartLpuart = 1, // GPIO 15 TX / 16 RX  (dual-band / GPS boards)
} WraithUartChannel;

typedef struct {
    uint8_t uart_channel; // WraithUartChannel
    bool autoscroll; // console follows newest output
    bool confirm_attacks; // gate deauth/beacon/probe behind a confirmation
    bool sound;
    bool vibro;
    bool led;
} WraithSettings;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    NotificationApp* notifications;

    // shared GUI modules
    Submenu* submenu;
    VariableItemList* var_item_list;
    TextInput* text_input;
    Widget* widget;

    // custom views
    ConsoleView* console_view;

    // radio link to the Marauder board
    MarauderUart* uart;

    WraithSettings settings;

    // command staged for the console scene to run on entry
    char pending_cmd[WRAITH_CMD_MAX];
    char pending_title[WRAITH_TITLE_MAX];
    bool pending_is_attack;

    // text-input scratch (raw command sender / target index)
    char input_buf[WRAITH_CMD_MAX];
    char select_kind; // 'a' = AP, 's' = station (for the Select scene)

    // generic input-prompt scene: build "<prefix><typed value>", then optionally
    // drop into the console running <after_cmd>
    char input_prefix[24];
    char input_header[32];
    char input_after_cmd[24];
    char input_after_title[WRAITH_TITLE_MAX];

    // when true, leaving the console scene must NOT stop the running op
    // (used when we dip into the raw-command sender and come straight back)
    bool console_keep_running;

    volatile uint32_t last_rx_tick; // last byte received from the board
} WraithApp;

/* ---- settings.c helpers ---- */
const char* wraith_uart_channel_label(uint8_t index);

/* ---- wraith.c helpers ---- */
// Stage a command and route to the console (through the confirm gate for attacks).
void wraith_launch(WraithApp* app, const char* title, const char* cmd, bool is_attack);
// Open the input-prompt scene: on commit, send "<prefix><value>"; if after_cmd is
// non-empty, then open the console running after_cmd (titled after_title).
void wraith_prompt(
    WraithApp* app,
    const char* header,
    const char* prefix,
    const char* after_title,
    const char* after_cmd);
void wraith_link_ensure(WraithApp* app); // acquire UART + worker if not already up
void wraith_link_disarm(WraithApp* app); // stop the current op and release the UART
void wraith_link_send(WraithApp* app, const char* cmd); // raw send (adds nothing)
void wraith_notify_start(WraithApp* app); // short feedback when an attack starts
bool wraith_link_is_live(WraithApp* app); // received data recently?
