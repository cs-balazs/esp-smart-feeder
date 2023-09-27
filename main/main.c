#include "config.h"
#include "telegram.h"
#include "wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "camera.h"

void app_main(void)
{
  nvs_flash_init();
  wifi_connection();
  ESP_ERROR_CHECK(init_camera());
  init_telegram();

  while (1)
  {
    await_wifi_connection();

    camera_fb_t *pic = take_photo();

    send_photo(pic->buf, pic->len);
    esp_camera_fb_return(pic);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
