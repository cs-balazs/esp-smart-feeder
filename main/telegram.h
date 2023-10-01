#ifndef BIRD_CAM_TELEGRAM_HEADER
#define BIRD_CAM_TELEGRAM_HEADER

#include "stdint.h"
#include "stddef.h"

void send_photo(uint8_t *body, size_t body_len);

#endif