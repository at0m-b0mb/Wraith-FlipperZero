#pragma once

/*
 * Command catalogue for the ESP32 Marauder serial CLI.
 *
 * Verified against the upstream firmware source
 * (github.com/justcallmekoko/ESP32Marauder, esp32_marauder/CommandLine.*).
 * Wraith sends these over UART with a trailing newline; the board streams its
 * output back, which the console view renders live. Anything not covered by a
 * menu can still be typed by hand in the Console (OK -> Send command).
 *
 * Newlines are intentionally omitted — wraith_link_send()/wraith_launch()/
 * wraith_prompt() append "\n", so a single constant can be reused.
 */

/* ---- general / admin ---- */
#define MARAUDER_CMD_STOP      "stopscan" // halts any running scan/attack/sniff
#define MARAUDER_CMD_REBOOT    "reboot"
#define MARAUDER_CMD_UPDATE_SD "update -s" // firmware update from SD card
#define MARAUDER_CMD_HELP      "help"
#define MARAUDER_CMD_SETTINGS  "settings" // print current board settings
#define MARAUDER_CMD_LS        "ls" // list SD card contents

/* ---- channel ---- */
#define MARAUDER_CMD_CHANNEL   "channel" // print the current channel
#define MARAUDER_PFX_CHANNEL   "channel -s " // + <n>  (2.4 GHz 1-14, 5 GHz 36..165)

/* ---- LED / display ---- */
#define MARAUDER_CMD_LED_RAINBOW "led -p rainbow"
#define MARAUDER_PFX_LED         "led -s " // + <hex color e.g. FF0000>
#define MARAUDER_PFX_BRIGHTNESS  "brightness -s " // + <0-9>

/* ---- scan / discovery ---- */
#define MARAUDER_CMD_SCAN_AP   "scanap"
#define MARAUDER_CMD_SCAN_STA  "scansta"
#define MARAUDER_CMD_SCAN_ALL  "scanall" // APs + stations in one pass
#define MARAUDER_CMD_LIST_AP   "list -a"
#define MARAUDER_CMD_LIST_STA  "list -s"
#define MARAUDER_CMD_LIST_SSID "list -c"

/* ---- sniffers ---- */
#define MARAUDER_CMD_SNIFF_BEACON    "sniffbeacon"
#define MARAUDER_CMD_SNIFF_PROBE     "sniffprobe"
#define MARAUDER_CMD_SNIFF_DEAUTH    "sniffdeauth"
#define MARAUDER_CMD_SNIFF_PMKID     "sniffpmkid"
#define MARAUDER_CMD_SNIFF_PMKID_D   "sniffpmkid -d" // + active deauth to force PMKID
#define MARAUDER_CMD_SNIFF_PWN       "sniffpwn" // pwnagotchi beacons
#define MARAUDER_CMD_SNIFF_RAW       "sniffraw"
#define MARAUDER_CMD_SNIFF_PINESCAN  "sniffpinescan" // detect Wi-Fi Pineapple
#define MARAUDER_CMD_SNIFF_MULTISSID "sniffmultissid" // detect Karma/multi-SSID APs
#define MARAUDER_CMD_SNIFF_SAE       "sniffsae" // WPA3 SAE

/* ---- analysis ---- */
#define MARAUDER_CMD_PACKETCOUNT "packetcount" // packets/sec on current channel
#define MARAUDER_CMD_MACTRACK    "mactrack" // track selected MACs
#define MARAUDER_PFX_FOXHUNT_W   "foxhunt -w " // + <AP index>   RSSI fox-hunt
#define MARAUDER_PFX_KARMA       "karma -p " // + <SSID index>  karma attack

/* ---- attacks (gated behind the confirmation prompt) ---- */
#define MARAUDER_CMD_ATTACK_DEAUTH    "attack -t deauth"
#define MARAUDER_CMD_ATTACK_BADMSG    "attack -t badmsg"
#define MARAUDER_CMD_ATTACK_BEACON_L  "attack -t beacon -l"
#define MARAUDER_CMD_ATTACK_BEACON_R  "attack -t beacon -r"
#define MARAUDER_CMD_ATTACK_BEACON_AP "attack -t beacon -a"
#define MARAUDER_CMD_ATTACK_PROBE     "attack -t probe"
#define MARAUDER_CMD_ATTACK_RICKROLL  "attack -t rickroll"
#define MARAUDER_CMD_ATTACK_FUNNY     "attack -t funny"
#define MARAUDER_CMD_ATTACK_SAE       "attack -t sae"
#define MARAUDER_CMD_ATTACK_CSA       "attack -t csa"
#define MARAUDER_CMD_ATTACK_QUIET     "attack -t quiet"

/* ---- targeting ---- */
#define MARAUDER_CMD_SELECT_AP_ALL "select -a all"
#define MARAUDER_CMD_CLEAR_AP      "clearlist -a"
#define MARAUDER_CMD_CLEAR_STA     "clearlist -s"
#define MARAUDER_CMD_CLEAR_SSID    "clearlist -c"
#define MARAUDER_PFX_SELECT_AP     "select -a " // + <indices|all>
#define MARAUDER_PFX_SELECT_STA    "select -s " // + <indices|all>
/* select a single index: printf(MARAUDER_FMT_SELECT, 'a'|'s', index) */
#define MARAUDER_FMT_SELECT        "select -%c %s"

/* ---- list persistence (SD card) ---- */
#define MARAUDER_CMD_SAVE_AP  "save -a"
#define MARAUDER_CMD_SAVE_STA "save -s"
#define MARAUDER_CMD_LOAD_AP  "load -a"
#define MARAUDER_CMD_LOAD_STA "load -s"

/* ---- MAC address ---- */
#define MARAUDER_CMD_RANDAPMAC   "randapmac"
#define MARAUDER_CMD_RANDSTAMAC  "randstamac"
#define MARAUDER_PFX_CLONEAPMAC  "cloneapmac -a " // + <AP index>
#define MARAUDER_PFX_CLONESTAMAC "clonestamac -s " // + <station index>

/* ---- SSID list (feeds "Beacon Spam (list)") ---- */
#define MARAUDER_PFX_SSID_GEN    "ssid -a -g " // + <n>     add n random SSIDs
#define MARAUDER_PFX_SSID_NAME   "ssid -a -n " // + <name>  add a named SSID
#define MARAUDER_PFX_SSID_REMOVE "ssid -r " // + <n>     remove SSID at index n

/* ---- bluetooth ---- */
#define MARAUDER_CMD_BT_SNIFF     "sniffbt"
#define MARAUDER_CMD_BT_AIRTAG    "sniffbt -t airtag" // detect Apple AirTags
#define MARAUDER_CMD_BT_FLIPPER   "sniffbt -t flipper" // detect nearby Flippers
#define MARAUDER_CMD_BT_SKIMMER   "sniffskim"
#define MARAUDER_PFX_BT_SPOOFAT   "spoofat -t " // + <index>  spoof a scanned AirTag

/* ---- bluetooth spam (attacks) ---- */
#define MARAUDER_CMD_BLE_SOURAPPLE  "blespam -t sourapple"
#define MARAUDER_CMD_BLE_APPLEJUICE "blespam -t applejuice"
#define MARAUDER_CMD_BLE_SAMSUNG    "blespam -t samsung"
#define MARAUDER_CMD_BLE_GOOGLE     "blespam -t google"
#define MARAUDER_CMD_BLE_WINDOWS    "blespam -t windows"
#define MARAUDER_CMD_BLE_FLIPPER    "blespam -t flipper"
#define MARAUDER_CMD_BLE_ALL        "blespam -t all"

/* ---- evil portal ---- */
#define MARAUDER_CMD_EVILPORTAL_START "evilportal -c start"
#define MARAUDER_PFX_EVILPORTAL_SETAP "evilportal -c setap " // + <AP index>

/* ---- network (needs a join first for the scans) ---- */
#define MARAUDER_PFX_JOIN     "join -a " // + "<AP index> -p <password>"
#define MARAUDER_CMD_PINGSCAN "pingscan"
#define MARAUDER_CMD_ARPSCAN  "arpscan"

/* ---- gps / wardrive ---- */
#define MARAUDER_CMD_GPS_DATA     "gpsdata"
#define MARAUDER_CMD_NMEA         "nmea" // raw NMEA stream
#define MARAUDER_CMD_GPS_TRACKER  "gpstracker"
#define MARAUDER_CMD_WARDRIVE     "wardrive"
#define MARAUDER_CMD_WARDRIVE_POI "wardrivepoi"
