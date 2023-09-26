idf_component_register(
  SRCS "main.c" "http.c" "telegram.c" "wifi.c" "camera.c"
  INCLUDE_DIRS ""
  EMBED_TXTFILES cert.pem
)
