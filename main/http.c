#include "http.h"
#include "esp_log.h"

static const char *HTTPS_TAG = "feeder-cam:https-request";

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
  switch (evt->event_id)
  {
  case HTTP_EVENT_ON_DATA:
    ESP_LOGI(HTTPS_TAG, "Response body: %s", (char *)evt->data);
    break;

  default:
    break;
  }
  return ESP_OK;
}