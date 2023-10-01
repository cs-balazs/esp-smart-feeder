#include "telegram.h"
#include "http.h"
#include "config.h"
#include "esp_log.h"
#include <stdlib.h>
#include "memory.h"
#include "secrets.h"

extern const uint8_t cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t cert_pem_end[] asm("_binary_cert_pem_end");

static const char *CONTENT_TYPE_HEADER = "multipart/form-data; boundary=" FORM_DATA_BOUNDARY;
static const char *URL = "https://api.telegram.org/bot" TELEGRAM_TOKEN "/sendMediaGroup";
static const char *FORM_DATA_PREFIX = "--" FORM_DATA_BOUNDARY "\r\nContent-Disposition: form-data; name=\"media\"\r\n\r\n[{\"type\":\"photo\",\"media\":\"attach://photo\"}]\r\n--" FORM_DATA_BOUNDARY "\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"image.jpg\"\r\nContent-Type: application/octet-stream\r\n\r\n";
static const char *FORM_DATA_SUFFIX = "\r\n--" FORM_DATA_BOUNDARY "\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n" TELEGRAM_CHAT_ID "\r\n--" FORM_DATA_BOUNDARY "--\r\n";

void construct_payload(uint8_t *body, size_t body_len, char **out, size_t *out_len)
{
  size_t payload_len = strlen(FORM_DATA_PREFIX) + body_len + strlen(FORM_DATA_SUFFIX);

  char *payload = malloc(payload_len);
  size_t offset = 0;
  memcpy(payload + offset, FORM_DATA_PREFIX, strlen(FORM_DATA_PREFIX));
  offset += strlen(FORM_DATA_PREFIX);
  memcpy(payload + offset, body, body_len);
  offset += body_len;
  memcpy(payload + offset, FORM_DATA_SUFFIX, strlen(FORM_DATA_SUFFIX));

  *out = payload;
  *out_len = payload_len;
}

void send_photo(uint8_t *body, size_t body_len)
{

  esp_http_client_config_t config_post = {
      .url = URL,
      .method = HTTP_METHOD_POST,
      .cert_pem = (const char *)cert_pem_start,
      .event_handler = client_event_get_handler};

  esp_http_client_handle_t client = esp_http_client_init(&config_post);

  char *payload;
  size_t payload_len;
  construct_payload(body, body_len, &payload, &payload_len);

  esp_http_client_set_post_field(client, payload, payload_len);
  esp_http_client_set_header(client, "Content-Type", CONTENT_TYPE_HEADER);

  esp_http_client_perform(client);
  esp_http_client_cleanup(client);
  free(payload);
}