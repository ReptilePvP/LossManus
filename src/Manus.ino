/**
 * Enhanced Loss Prevention Log for M5Stack CoreS3
 * Final Arduino Sketch
 * 
 * This sketch implements a loss prevention logging system with LVGL 8.4.0 UI
 * Version 1.0.0
 */

 #include <M5Unified.h>
 #include <M5GFX.h>
 #include "lv_conf.h"
 #include <lvgl.h>
 #include <WiFi.h>
 #include <Preferences.h>
 #include <SPI.h>
 #include <SD.h>
 #include <time.h>
 
 // Include project headers
 #include "config.h"
 #include "version.h"
 #include "app_controller.h"
 #include "ui_manager.h"
 #include "display.h"
 #include "touch.h"
 #include "power.h"
 #include "rtc.h"
 #include "storage.h"
 #include "wifi_manager.h"
 #include "database.h"
 
 // LVGL display buffer
 static lv_disp_draw_buf_t draw_buf;
 static lv_disp_drv_t disp_drv;
 static lv_indev_drv_t indev_drv;
 
 // LVGL semaphore for thread safety
 static SemaphoreHandle_t xGuiSemaphore;
 
 // Display flush callback
 void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
     uint32_t w = (area->x2 - area->x1 + 1);
     uint32_t h = (area->y2 - area->y1 + 1);
 
     M5.Display.startWrite();
     M5.Display.setAddrWindow(area->x1, area->y1, w, h);
     M5.Display.writePixels((uint16_t *)color_p, w * h);
     M5.Display.endWrite();
 
     lv_disp_flush_ready(disp);
 }
 
 // Touchpad read callback
 void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
     TouchPoint_t pos = M5.Touch.getDetail();
     if (M5.Touch.ispressed()) {
         data->state = LV_INDEV_STATE_PR;
         data->point.x = pos.x;
         data->point.y = pos.y;
     } else {
         data->state = LV_INDEV_STATE_REL;
     }
 }
 
 // Set system time from RTC
 void setSystemTimeFromRTC() {
     m5::rtc_datetime_t dt;
     M5.Rtc.getDateTime(&dt);
     
     struct tm timeinfo;
     timeinfo.tm_year = dt.date.year - 1900;
     timeinfo.tm_mon = dt.date.month - 1;
     timeinfo.tm_mday = dt.date.date;
     timeinfo.tm_hour = dt.time.hours;
     timeinfo.tm_min = dt.time.minutes;
     timeinfo.tm_sec = dt.time.seconds;
     
     time_t t = mktime(&timeinfo);
     struct timeval now = { .tv_sec = t };
     settimeofday(&now, NULL);
 }
 
 // Update time display
 void updateTimeDisplay() {
     // This will be called from the main loop to update time displays
     // Implementation depends on which screen is active
 }
 
 // Initialize file system
 void initFileSystem() {
     if (!SD.begin()) {
         // If SD card is not available, use SPIFFS
         if (!SPIFFS.begin(true)) {
             // Handle storage initialization failure
         }
     }
     
     // Initialize database
     Database::init();
 }
 
 // Initialize styles
 void initStyles() {
     // Initialize LVGL styles for consistent UI appearance
 }
 
 // Setup function
 void setup() {
     Serial.begin(115200);
     Serial.println("Starting Loss Prevention Log...");
 
     auto cfg = M5.config();
     M5.begin(cfg);
     M5.Power.begin();
 
     // Enable external bus power
     M5.Power.setExtOutput(true);
     Serial.println("External bus power enabled: " + String(M5.Power.getExtOutput() ? "Yes" : "No"));
 
     // Configure ALDO3 to 3.3V
     M5.Power.Axp2101.writeRegister8(0x94, 28); // Set ALDO3 to 3.3V
     uint8_t reg90 = M5.Power.Axp2101.readRegister8(0x90);
     M5.Power.Axp2101.writeRegister8(0x90, reg90 | 0x08); // Enable ALDO3
     Serial.printf("ALDO3 configured to 3.3V\n");
 
     // Check AW9523 PORT0/PORT1
     uint8_t port0_val = M5.In_I2C.readRegister8(0x58, 0x02, 100000);
     Serial.printf("AW9523 PORT0: 0x%02X (BUS_EN: %d)\n", port0_val, (port0_val & 0x02) >> 1);
     uint8_t port1_val = M5.In_I2C.readRegister8(0x58, 0x03, 100000);
     Serial.printf("AW9523 PORT1: 0x%02X (BOOST_EN: %d)\n", port1_val, (port1_val & 0x80) >> 7);
 
     // Monitor battery and VBUS
     Serial.printf("Battery Voltage: %d mV\n", M5.Power.getBatteryVoltage());
     Serial.printf("VBUS Voltage: %d mV\n", M5.Power.getVBUSVoltage());
 
     // Initialize speaker
     M5.Speaker.begin();
     Serial.println("Speaker initialized");
 
     // Load persistent sound settings
     Preferences prefs;
     prefs.begin("settings", false);
     uint8_t saved_volume = prefs.getUChar("volume", 128);
     bool sound_enabled = prefs.getBool("sound_enabled", true);
     M5.Speaker.setVolume(sound_enabled ? saved_volume : 0);
     Serial.printf("Loaded sound settings - Enabled: %d, Volume: %d\n", sound_enabled, saved_volume);
 
     // Load persistent brightness settings
     uint8_t displayBrightness = prefs.getUChar("brightness", 128);
     M5.Display.setBrightness(displayBrightness);
     Serial.printf("Loaded Brightness settings - Brightness: %d\n", displayBrightness);
     prefs.end();
 
     // Initialize LVGL
     Serial.println("Initializing LVGL");
     lv_init();
 
     // Initialize LVGL buffer
     static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(SCREEN_WIDTH * 40 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
     static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(SCREEN_WIDTH * 40 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
     lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SCREEN_WIDTH * 40);
     Serial.println("Display buffer initialized");
 
     lv_disp_drv_init(&disp_drv);
     disp_drv.hor_res = M5.Display.width();
     disp_drv.ver_res = M5.Display.height();
     disp_drv.flush_cb = my_disp_flush;
     disp_drv.draw_buf = &draw_buf;
     lv_disp_drv_register(&disp_drv);
     Serial.println("Display driver registered");
 
     lv_indev_drv_init(&indev_drv);
     indev_drv.type = LV_INDEV_TYPE_POINTER;
     indev_drv.read_cb = my_touchpad_read;
     lv_indev_drv_register(&indev_drv);
     Serial.println("Input driver registered");
 
     // Create semaphore for LVGL thread safety
     xGuiSemaphore = xSemaphoreCreateMutex();
 
     // Set initial RTC time if unset
     m5::rtc_date_t DateStruct;
     M5.Rtc.getDate(&DateStruct);
     if (DateStruct.year < 2020) {
         // Set default date/time if RTC is unset
         DateStruct.year = 2025;
         DateStruct.month = 3;
         DateStruct.date = 17; // Today's date
         DateStruct.weekDay = 1; // Monday
         M5.Rtc.setDate(&DateStruct);
 
         m5::rtc_time_t TimeStruct;
         TimeStruct.hours = 12;
         TimeStruct.minutes = 0;
         TimeStruct.seconds = 0;
         M5.Rtc.setTime(&TimeStruct);
         Serial.println("RTC set to default: 2025-03-17 12:00:00");
     }
     setSystemTimeFromRTC();
 
     // Initialize WiFi Manager
     WiFiManager::init();
     Serial.println("WiFi Manager initialized");
 
     // Initialize file system
     initFileSystem();
     Serial.println("File system initialized");
 
     // Initialize application controller
     AppController::init();
     Serial.println("App controller initialized");
 
     // Initialize UI manager
     UIManager::init();
     Serial.println("UI manager initialized");
 
     // Initialize styles
     initStyles();
     Serial.println("Styles initialized");
 
     // Show main menu
     UIManager::setScreen(SCREEN_MAIN_MENU);
     Serial.println("Setup complete!");
 }
 
 // Main loop
 void loop() {
     M5.update();
     
     // Update time display every second
     static unsigned long lastTimeUpdate = 0;
     if (millis() - lastTimeUpdate > 1000) {
         updateTimeDisplay();
         lastTimeUpdate = millis();
     }
 
     // Handle LVGL timing
     static uint32_t lastLvglTick = 0;
     uint32_t currentMillis = millis();
     if (currentMillis - lastLvglTick > 10) { // 10ms refresh rate
         if (xSemaphoreTake(xGuiSemaphore, (TickType_t)10) == pdTRUE) {
             lv_task_handler();
             xSemaphoreGive(xGuiSemaphore);
             lastLvglTick = currentMillis;
         }
     }
 
     // Process WiFi events
     WiFiManager::update();
     
     // Process application events
     AppController::update();
     
     // Update UI
     UIManager::update();
 }
 