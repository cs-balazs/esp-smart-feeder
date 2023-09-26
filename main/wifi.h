#ifndef BIRD_CAM_WIFI_HEADER
#define BIRD_CAM_WIFI_HEADER

#include "esp_wifi.h"

void wifi_connection();
void await_wifi_connection();

#endif