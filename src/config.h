/**
 * Enhanced Loss Prevention Log
 * Configuration file
 * 
 * This file contains all configurable parameters for the application
 */

#ifndef CONFIG_H
#define CONFIG_H

// Debug configuration
#define DEBUG_ENABLED true
#define DEBUG_PRINT(x) if(DEBUG_ENABLED) { Serial.print(millis()); Serial.print(": "); Serial.println(x); }
#define DEBUG_PRINTF(x, ...) if(DEBUG_ENABLED) { Serial.print(millis()); Serial.print(": "); Serial.printf(x, __VA_ARGS__); }

// Hardware configuration - M5Stack CoreS3
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// SD Card configuration
#define SD_SPI_SCK_PIN  36
#define SD_SPI_MISO_PIN 35
#define SD_SPI_MOSI_PIN 37
#define SD_SPI_CS_PIN   4
#define TFT_DC 35

// Power management configuration
#define AXP2101_ADDR 0x34
#define AW9523_ADDR 0x58
#define POWER_SAVE_TIMEOUT 60000  // Time in ms before entering power save mode
#define SCREEN_BRIGHTNESS_NORMAL 128
#define SCREEN_BRIGHTNESS_POWER_SAVE 32

// Data management configuration
#define DATABASE_FILENAME "/loss_prevention.db"
#define LOG_FILENAME "/loss_prevention_log.txt"
#define MAX_LOG_ENTRIES 1000
#define BACKUP_INTERVAL 86400000  // 24 hours in ms

// WiFi configuration
#define MAX_WIFI_NETWORKS 5
#define WIFI_CONNECT_TIMEOUT 10000  // 10 seconds
#define MAX_WIFI_CONNECTION_ATTEMPTS 5
#define WIFI_RECONNECT_INTERVAL 300000  // 5 minutes

// UI configuration
#define UI_ANIMATION_ENABLED true
#define UI_ANIMATION_SPEED 300  // ms
#define UI_THEME_DEFAULT "default"
#define UI_HAPTIC_FEEDBACK_ENABLED true
#define LV_TICK_PERIOD_MS 10

// Application configuration
#define APP_VERSION "2.0.0"
#define APP_NAME "Enhanced Loss Prevention Log"
#define APP_AUTHOR "Manus"

// Webhook configuration
#define WEBHOOK_ENABLED true
#define WEBHOOK_RETRY_INTERVAL 60000  // 1 minute
#define WEBHOOK_MAX_RETRIES 5

#endif // CONFIG_H
