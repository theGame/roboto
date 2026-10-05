/*
 * ESP32 Camera Surveillance Car
 *
 * Автоматично підбирає робочий RGB565-розмір кадру:
 *   1) QVGA  = 320x240  (старі модулі)
 *   2) HQVGA = 240x176  (новий модуль)
 *   3) QQVGA = 160x120  (резервний режим)
 *
 * Важливо: розмір кадру задається ДО esp_camera_init().
 * Після ініціалізації set_framesize() не викликається.
 */

#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_M5STACK_PSRAM
#define CAMERA_MODEL_AI_THINKER

const char *ssid = "TP-Link_0D14";
const char *password = "35429012";

// const char *ssid = "SoftServe-Guest";
// const char *password = "Welcome!";

#if defined(CAMERA_MODEL_WROVER_KIT)

#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  21
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    19
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM     5
#define Y2_GPIO_NUM     4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

#elif defined(CAMERA_MODEL_AI_THINKER)

#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM   0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM     5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

#else
#error "Camera model not selected"
#endif

// GPIO / shared variables used by app_httpd.cpp
int gpLed = 4;
String WiFiAddr = "";

// Information about the format selected automatically
framesize_t gCameraFrameSize = FRAMESIZE_QQVGA;
bool gCameraFrameSizeValid = false;
size_t gCameraWidth = 0;
size_t gCameraHeight = 0;

void startCameraServer();

struct FrameSizeCandidate
{
    framesize_t frameSize;
    const char *name;
    uint16_t width;
    uint16_t height;
};

static const FrameSizeCandidate CAMERA_FORMATS[] =
{
    {FRAMESIZE_QVGA,  "QVGA",  320, 240}, // old camera modules
    {FRAMESIZE_HQVGA, "HQVGA", 240, 176}, // new camera module
    {FRAMESIZE_QQVGA, "QQVGA", 160, 120}  // fallback
};

static camera_config_t makeCameraConfig(framesize_t frameSize)
{
    // Zero initialization is important because newer camera_config_t
    // versions contain additional fields.
    camera_config_t config = {};

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    /*
     * The old misspelled names pin_sscb_* are retained here intentionally:
     * old esp32-camera versions use them directly, while newer versions keep
     * them as compatibility aliases.
     */
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB565;
		//config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = frameSize;

    // jpeg_quality is ignored for RGB565, but initialize it anyway.
    config.jpeg_quality = 12;
    config.fb_count = 1;

    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = psramFound()
        ? CAMERA_FB_IN_PSRAM
        : CAMERA_FB_IN_DRAM;

    return config;
}

static bool captureTestFrame(const FrameSizeCandidate &candidate)
{
    Serial.printf("Testing %s (%u x %u)...\n",
                  candidate.name,
                  candidate.width,
                  candidate.height);

    // Give the sensor a moment to stabilize.
    delay(250);

    camera_fb_t *fb = esp_camera_fb_get();

    if (fb == nullptr)
    {
        Serial.printf("%s: frame capture failed\n", candidate.name);
        return false;
    }

    const bool valid =
        fb->buf != nullptr &&
        fb->len > 0 &&
        fb->width == candidate.width &&
        fb->height == candidate.height;

    if (valid)
    {
        gCameraFrameSize = candidate.frameSize;
        gCameraFrameSizeValid = true;
        gCameraWidth = fb->width;
        gCameraHeight = fb->height;

        Serial.printf(
            "%s works: actual frame %u x %u, %u bytes, format=%d\n",
            candidate.name,
            static_cast<unsigned>(fb->width),
            static_cast<unsigned>(fb->height),
            static_cast<unsigned>(fb->len),
            static_cast<int>(fb->format)
        );
    }
    else
    {
        Serial.printf("%s: invalid frame\n", candidate.name);
    }

    esp_camera_fb_return(fb);
    return valid;
}

static bool initCameraAutomatically()
{
    const size_t formatCount =
        sizeof(CAMERA_FORMATS) / sizeof(CAMERA_FORMATS[0]);

    Serial.printf("PSRAM: %s\n", psramFound() ? "found" : "not found");

    for (size_t i = 0; i < formatCount; ++i)
    {
        const FrameSizeCandidate &candidate = CAMERA_FORMATS[i];

        Serial.printf(
            "\nInitializing camera in %s RGB565 mode (%u x %u)...\n",
            candidate.name,
            candidate.width,
            candidate.height
        );

        camera_config_t config = makeCameraConfig(candidate.frameSize);

        const esp_err_t err = esp_camera_init(&config);

        if (err != ESP_OK)
        {
            Serial.printf(
                "%s initialization failed: 0x%x\n",
                candidate.name,
                static_cast<unsigned>(err)
            );

            // Safe even if initialization was only partially completed.
            esp_camera_deinit();
            delay(200);
            continue;
        }

        if (captureTestFrame(candidate))
        {
            Serial.printf(
                "\nSelected camera mode: %s, actual %u x %u\n",
                candidate.name,
                static_cast<unsigned>(gCameraWidth),
                static_cast<unsigned>(gCameraHeight)
            );

            /*
             * Do not call sensor->set_framesize() here for RGB565.
             * The allocated framebuffer must remain consistent with
             * the resolution selected before esp_camera_init().
             */
            return true;
        }

        Serial.printf("%s rejected; trying the next format...\n",
                      candidate.name);

        esp_camera_deinit();
        delay(300);
    }

    Serial.println("\nNo compatible RGB565 camera format was found.");
    return false;
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();
    Serial.println("ESP32 camera startup");

    pinMode(gpLed, OUTPUT);
    digitalWrite(gpLed, LOW);

    if (!initCameraAutomatically())
    {
        Serial.println("Camera startup stopped.");
        return;
    }



    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setSleep(false);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    startCameraServer();

    WiFiAddr = WiFi.localIP().toString();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFiAddr);
    Serial.println("' to connect");
}

void loop()
{
    delay(10000);
}
