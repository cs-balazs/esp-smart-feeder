#include "wifi.h"
#include "config.h"
#include "esp_netif_types.h"
#include "esp_log.h"
#include "secrets.h"

// TODO: Implement reconnect

uint8_t static wifi_connected = 0;
uint8_t static try_to_reconnect = 0;

static const char *WIFI_TAG = "feeder-cam:wifi-connection";

static void
wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  switch (event_id)
  {
  case WIFI_EVENT_STA_START:
    ESP_LOGI(WIFI_TAG, "Connecting...");
    break;
  case WIFI_EVENT_STA_CONNECTED:
    ESP_LOGI(WIFI_TAG, "Connected!");
    try_to_reconnect = 0;
    break;
  case WIFI_EVENT_STA_DISCONNECTED:
    ESP_LOGW(WIFI_TAG, "Lost connection!");
    wifi_connected = 0;
    try_to_reconnect = 1;
    break;
  case IP_EVENT_STA_GOT_IP:
    ESP_LOGW(WIFI_TAG, "Got IP address!");
    wifi_connected = 1;
    break;
  default:
    break;
  }
}

void wifi_connection()
{
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_initiation);

  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
  wifi_config_t wifi_configuration = {
      .sta = {
          .ssid = SSID,
          .password = PASSWORD}};

  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
  esp_wifi_start();
  esp_wifi_connect();
}

void await_wifi_connection()
{
  while (!wifi_connected)
  {
    if (try_to_reconnect)
    {
      esp_wifi_connect();
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ESP_LOGI(WIFI_TAG, "Awaiting connection...");
  }
}