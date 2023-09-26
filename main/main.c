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

  await_wifi_connection();

  while (1)
  {
    camera_fb_t *pic = take_photo();

    size_t out_len;
    uint8_t *out;

    frame2jpg(pic, 100, &out, &out_len);
    send_photo(out, out_len);

    free(out);
    esp_camera_fb_return(pic);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
