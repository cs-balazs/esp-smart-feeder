#include "telegram.h"
#include "http.h"
#include "config.h"
#include "esp_log.h"
#include <stdlib.h>
#include "memory.h"
#include "secrets.h"

extern const uint8_t cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t cert_pem_end[] asm("_binary_cert_pem_end");

#define FORM_DATA_PREFIX_LENGTH 2 + FORM_DATA_BOUNDARY_LENGTH + 2 + 37 + 1 + 5 + 1 + 11 + 1 + 9 + 3 + 38 + 4
#define FORM_DATA_SUFFIX_LENGTH 4 + FORM_DATA_BOUNDARY_LENGTH + 2 + 37 + 1 + 7 + 5 + TELEGRAM_CHAT_ID_LENGTH + 2 + 2 + FORM_DATA_BOUNDARY_LENGTH + 4
#define URL_LENGTH 28 + TELEGRAM_TOKEN_LENGTH + 10

char content_type_header[30 + FORM_DATA_BOUNDARY_LENGTH + 1] = "multipart/form-data; boundary=\0";
char url[URL_LENGTH + 1] = "https://api.telegram.org/bot\0";
char form_data_prefix[FORM_DATA_PREFIX_LENGTH + 1] = "--\0";
char form_data_suffix[FORM_DATA_SUFFIX_LENGTH + 1] = "\r\n--\0";

void init_telegram()
{
  // Construct Content-Type header with boundary
  size_t initial_length = strlen(content_type_header);

  memcpy(content_type_header + initial_length, FORM_DATA_BOUNDARY, FORM_DATA_BOUNDARY_LENGTH);
  content_type_header[initial_length + FORM_DATA_BOUNDARY_LENGTH] = '\0';

  // Construct URL
  initial_length = strlen(url);
  const char *url_after_token = "/sendPhoto";
  const size_t url_after_token_length = strlen(url_after_token);

  memcpy(url + initial_length, TELEGRAM_TOKEN, TELEGRAM_TOKEN_LENGTH);
  memcpy(url + initial_length + TELEGRAM_TOKEN_LENGTH, url_after_token, url_after_token_length);
  url[initial_length + TELEGRAM_TOKEN_LENGTH + url_after_token_length] = '\0';

  // Construct form-data prefix
  initial_length = strlen(form_data_prefix);
  const char *prefix_after_boundary = "\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"image.jpg\"\r\nContent-Type: application/octet-stream\r\n\r\n";
  const size_t prefix_after_boundary_length = strlen(prefix_after_boundary);

  memcpy(form_data_prefix + initial_length, FORM_DATA_BOUNDARY, FORM_DATA_BOUNDARY_LENGTH);
  memcpy(form_data_prefix + initial_length + FORM_DATA_BOUNDARY_LENGTH, prefix_after_boundary, prefix_after_boundary_length);
  form_data_prefix[initial_length + FORM_DATA_BOUNDARY_LENGTH + prefix_after_boundary_length] = '\0';

  // Construct form-data suffix
  initial_length = strlen(form_data_suffix);
  const char *suffix_before_chat_id = "\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
  const size_t suffix_before_chat_id_length = strlen(suffix_before_chat_id);
  const char *suffix_after_chat_id = "\r\n--";
  const size_t suffix_after_chat_id_length = strlen(suffix_after_chat_id);
  const char *sufix_ending = "--\r\n";
  size_t suffix_ending_length = strlen(sufix_ending);

  memcpy(form_data_suffix + initial_length, FORM_DATA_BOUNDARY, FORM_DATA_BOUNDARY_LENGTH);
  memcpy(form_data_suffix + initial_length + FORM_DATA_BOUNDARY_LENGTH, suffix_before_chat_id, suffix_before_chat_id_length);
  memcpy(form_data_suffix + initial_length + FORM_DATA_BOUNDARY_LENGTH + suffix_before_chat_id_length, TELEGRAM_CHAT_ID, TELEGRAM_CHAT_ID_LENGTH);
  memcpy(form_data_suffix + initial_length + FORM_DATA_BOUNDARY_LENGTH + suffix_before_chat_id_length + TELEGRAM_CHAT_ID_LENGTH, suffix_after_chat_id, suffix_after_chat_id_length);
  memcpy(form_data_suffix + initial_length + FORM_DATA_BOUNDARY_LENGTH + suffix_before_chat_id_length + TELEGRAM_CHAT_ID_LENGTH + suffix_after_chat_id_length, FORM_DATA_BOUNDARY, FORM_DATA_BOUNDARY_LENGTH);
  memcpy(form_data_suffix + initial_length + FORM_DATA_BOUNDARY_LENGTH + suffix_before_chat_id_length + TELEGRAM_CHAT_ID_LENGTH + suffix_after_chat_id_length + FORM_DATA_BOUNDARY_LENGTH, sufix_ending, suffix_ending_length);
  form_data_suffix[initial_length + FORM_DATA_BOUNDARY_LENGTH + suffix_before_chat_id_length + TELEGRAM_CHAT_ID_LENGTH + suffix_after_chat_id_length + FORM_DATA_BOUNDARY_LENGTH + suffix_ending_length] = '\0';
}

void construct_payload(uint8_t *body, size_t body_len, char **out, size_t *out_len)
{
  size_t payload_len = FORM_DATA_PREFIX_LENGTH + body_len + FORM_DATA_SUFFIX_LENGTH;

  char *payload = malloc(payload_len + 1);
  memcpy(payload, form_data_prefix, FORM_DATA_PREFIX_LENGTH);
  memcpy(payload + FORM_DATA_PREFIX_LENGTH, body, body_len);
  memcpy(payload + FORM_DATA_PREFIX_LENGTH + body_len, form_data_suffix, FORM_DATA_SUFFIX_LENGTH);
  payload[payload_len] = '\0';

  *out = payload;
  *out_len = payload_len;
}

void send_photo(uint8_t *body, size_t body_len)
{

  esp_http_client_config_t config_post = {
      .url = url,
      .method = HTTP_METHOD_POST,
      .cert_pem = (const char *)cert_pem_start,
      .event_handler = client_event_get_handler};

  esp_http_client_handle_t client = esp_http_client_init(&config_post);

  char *payload;
  size_t payload_len;
  construct_payload(body, body_len, &payload, &payload_len);

  esp_http_client_set_post_field(client, payload, payload_len);
  esp_http_client_set_header(client, "Content-Type", content_type_header);

  esp_http_client_perform(client);
  esp_http_client_cleanup(client);
  free(payload);
}