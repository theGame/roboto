# ESP32 Camera Car: Project Status

The HTTP server and camera files in this folder were added and tested with the Arduino IDE. The ESP32 camera initialized successfully, captured frames, and worked as expected during testing.

## How it works

1. `esp32-car.ino` starts the serial console, configures the AI-Thinker camera pins, and checks for PSRAM.
2. It tries RGB565 camera modes in order: QVGA (320 x 240), HQVGA (240 x 176), then QQVGA (160 x 120). Each mode is initialized and tested by capturing a frame; the first valid frame size is selected.
3. The ESP32 connects to the configured Wi-Fi network and prints its IP address to the serial console.
4. `http-server.cpp` starts an HTTP control server on port 80 and a camera stream server on port 81. Open the printed IP address in a browser to view the control page; the stream is served at `/stream` on port 81.
5. Browser control requests call the registered handlers. Movement and mode handlers send a three-byte command over `Serial`; the motor controller on the other end must use the matching protocol. The LED handlers control the ESP32 LED directly.

The web interface includes directional controls, stop, LED controls, four operating modes, and left/right motor controls. The server also provides camera capture, stream, status, and camera-setting endpoints.

## Files in this folder

- `esp32-car.ino` is the ESP32 sketch entry point. It configures the camera model and pin mapping, selects a working frame size, connects to Wi-Fi, and starts the server. Update the Wi-Fi credentials in this sketch for the network being used.
- `http-server.cpp` implements the browser page, camera stream and capture handlers, camera status and setting handlers, and the robot/LED control endpoints. Movement commands are sent using the protocol defined by the motor controller code in the parent `motor` folder.
- `http-server.h` contains the HTTP server API declarations and configuration types used by `http-server.cpp`.
- `camera-index.h` contains a gzip-compressed HTML page asset for the camera interface.
- `.vscode/c_cpp_properties.json` configures C/C++ IntelliSense. Its current paths and definitions target Arduino Uno/AVR, so it needs to be updated for the ESP32 board and the installed ESP32 core before it can accurately provide ESP32 IntelliSense.
- `about.md` describes this folder, its current test status, and the remaining setup work.

## VS Code setup still needed

The project has not yet been fully configured or refactored for building from VS Code. Remaining work includes:

- Configure the ESP32 board platform, board selection, and build/upload workflow in VS Code, and replace the Uno/AVR IntelliSense paths and definitions.
- Make sure the required ESP32 Arduino core and its camera, Wi-Fi, HTTP server, timer, and image-conversion components are available to the selected build environment. These headers are provided by the ESP32 framework/core rather than being separate third-party libraries in this sketch.
- Review and refactor the sketch and source-file layout, include paths, and build configuration as needed for VS Code to compile the project reliably.

The existing Arduino IDE test confirms the camera code worked in that environment; the VS Code build and upload workflow still needs to be validated separately.
