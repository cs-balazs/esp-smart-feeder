#include "camera.h"
#include "esp_log.h"

static const char *CAMERA_TAG = "feeder-cam:camera";

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    // XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG, // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_HD,     // QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

    .jpeg_quality = 3, // 0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,     // When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

camera_fb_t *take_photo()
{
  ESP_LOGI(CAMERA_TAG, "Taking picture...");
  camera_fb_t *pic = esp_camera_fb_get();

  ESP_LOGI(CAMERA_TAG, "Picture taken!");
  // ESP_LOGI(CAMERA_TAG, "Length: %zu", pic->len);
  // ESP_LOGI(CAMERA_TAG, "Width: %zu", pic->width);
  // ESP_LOGI(CAMERA_TAG, "Height: %zu", pic->height);
  // ESP_LOGI(CAMERA_TAG, "Format: %d", pic->format);

  return pic;
}

esp_err_t init_camera()
{
  return esp_camera_init(&camera_config);
}