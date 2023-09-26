# [WIP] ESP Smart Feeder

This is a hobby project, which uses an ESP-32 CAM for a smart bird feeder, meaning it takes pictures when it detects motion (motion detection isn't implemented yet, it takes a picture every 5 seconds)

## Setup

1. Set up [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started)
2. Clone this repo with the `--recurse-submodules` flas, as this repo includes [esp32-camera](https://github.com/espressif/esp32-camera) as a submodule
3. Run `get_idf` in the root of this repo
4. Make a copy of the `secrets.h.template` file with just `secrets.h` as the filename, and fill it with your wifi, and telegram credentials
   - Note that Telegram bots can't send you direct messages unless you interact with them first. So if you create a fresh Telegram bot don't forget to send it a message before trying to send pictures with it (you can create a Telegram bot by contacting `@BotFather` on Telegram and using the `/newbot` command)
   - You can delete the template afterwards
5. In `config.h` make sure that only the correct pin layout is defined for your board
   - Optionally the form-data payload's boundary is also configureble, but I don't think it is necessary to change that
6. Run `openssl s_client -showcerts -connect api.telegram.org:443 </dev/null | sed -n -e '/-.BEGIN/,/-.END/ p' > ./main/cert.pem` to generate the `cert.pem` file with the ssl certificates
7. Build the app with `idf.py build`
8. Connect your ESP-32 CAM, and flash the binary with `idf.py -p PORT build` where PORT is the USB port's identifier where the ESP is connected
   - Check [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html) if you have trouble with usb connection

Credit to the [esp32-cam example](https://github.com/espressif/esp32-camera/blob/master/examples/main/take_picture.c)
