#pragma once

/*
 * Command catalogue for the ESP32 Marauder serial CLI.
 *
 * These are the stable, documented commands from the upstream ESP32 Marauder
 * firmware (github.com/justcallmekoko/ESP32Marauder). Wraith sends them over
 * UART with a trailing newline; the board streams its output back, which the
 * console view renders live. Anything not covered by a menu can still be typed
 * by hand in the Console (OK -> Send command), so Wraith stays useful across
 * firmware revisions.
 *
 * Newlines are intentionally omitted here — wraith_link_send() /
 * wraith_launch() append "\n" so a single command constant can be reused.
 */

/* ---- general ---- */
#define MARAUDER_CMD_STOP      "stop"
#define MARAUDER_CMD_REBOOT    "reboot"
#define MARAUDER_CMD_UPDATE    "update"
#define MARAUDER_CMD_HELP      "help"

/* ---- scan / discovery ---- */
#define MARAUDER_CMD_SCAN_AP   "scanap"
#define MARAUDER_CMD_SCAN_STA  "scansta"
#define MARAUDER_CMD_LIST_AP   "list -a"
#define MARAUDER_CMD_LIST_STA  "list -s"
#define MARAUDER_CMD_LIST_SSID "list -c"

/* ---- sniffers ---- */
#define MARAUDER_CMD_SNIFF_BEACON "sniffbeacon"
#define MARAUDER_CMD_SNIFF_PROBE  "sniffprobe"
#define MARAUDER_CMD_SNIFF_DEAUTH "sniffdeauth"
#define MARAUDER_CMD_SNIFF_PMKID  "sniffpmkid"
#define MARAUDER_CMD_SNIFF_PWN    "sniffpwn"
#define MARAUDER_CMD_SNIFF_ESP    "sniffesp"
#define MARAUDER_CMD_SNIFF_RAW    "sniffraw"

/* ---- attacks (gated behind the confirmation prompt) ---- */
#define MARAUDER_CMD_ATTACK_DEAUTH     "attack -t deauth"
#define MARAUDER_CMD_ATTACK_BEACON_L   "attack -t beacon -l"
#define MARAUDER_CMD_ATTACK_BEACON_R   "attack -t beacon -r"
#define MARAUDER_CMD_ATTACK_BEACON_AP  "attack -t beacon -a"
#define MARAUDER_CMD_ATTACK_PROBE      "attack -t probe"
#define MARAUDER_CMD_ATTACK_RICKROLL   "attack -t rickroll"

/* ---- targeting ---- */
#define MARAUDER_CMD_SELECT_AP_ALL "select -a all"
#define MARAUDER_CMD_CLEAR_AP      "clearlist -a"
#define MARAUDER_CMD_CLEAR_STA     "clearlist -s"
/* select a single index: printf(MARAUDER_FMT_SELECT, 'a'|'s', index) */
#define MARAUDER_FMT_SELECT        "select -%c %s"

/* ---- bluetooth ---- */
#define MARAUDER_CMD_BT_SNIFF      "sniffbt"
#define MARAUDER_CMD_BT_SKIMMER    "sniffskim"

/* ---- gps (needs the onboard GPS antenna + a fix) ---- */
#define MARAUDER_CMD_GPS_DATA      "gpsdata"
#define MARAUDER_CMD_WARDRIVE      "wardrive"
#define MARAUDER_CMD_WARDRIVE_STA  "stationwardrive"
