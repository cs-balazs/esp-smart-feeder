#ifndef BIRD_CAM_HTTP_HEADER
#define BIRD_CAM_HTTP_HEADER

#include "stdint.h"
#include "esp_http_client.h"

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt);

#endif