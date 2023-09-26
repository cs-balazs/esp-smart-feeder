#ifndef BIRD_CAM_CONFIG_HEADER
#define BIRD_CAM_CONFIG_HEADER

// support IDF 5.x
#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

// #define BOARD_WROVER_KIT 1
#define BOARD_ESP32CAM_AITHINKER 1

#define FORM_DATA_BOUNDARY "----------esp-bird-cam-boundary"
#define FORM_DATA_BOUNDARY_LENGTH 31
#define TELEGRAM_TOKEN_LENGTH 46
#define TELEGRAM_CHAT_ID_LENGTH 9

#endif