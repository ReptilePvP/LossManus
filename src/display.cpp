/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Display Implementation
 */

#include "display.h"

// Static member initialization
uint8_t DisplayHAL::_brightness = SCREEN_BRIGHTNESS_NORMAL;
bool DisplayHAL::_powerSaveMode = false;
uint32_t DisplayHAL::_lastActivityTime = 0;

bool DisplayHAL::init() {
    DEBUG_PRINT("Initializing display...");
    
    // Initialize M5 with default configuration
    auto cfg = M5.config();
    M5.begin(cfg);
    
    // Initialize power management
    M5.Power.begin();
    
    // Enable external bus power
    M5.Power.setExtOutput(true);
    DEBUG_PRINTF("External bus power enabled: %s\n", M5.Power.getExtOutput() ? "Yes" : "No");
    
    // Configure ALDO3 to 3.3V
    M5.Power.Axp2101.writeRegister8(0x94, 28); // Set ALDO3 to 3.3V
    uint8_t reg90 = M5.Power.Axp2101.readRegister8(0x90);
    M5.Power.Axp2101.writeRegister8(0x90, reg90 | 0x08); // Enable ALDO3
    DEBUG_PRINT("ALDO3 configured to 3.3V");
    
    // Set initial brightness
    setBrightness(_brightness);
    
    // Reset activity timer
    _lastActivityTime = millis();
    
    DEBUG_PRINT("Display initialization complete");
    return true;
}

void DisplayHAL::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    M5.Display.setBrightness(_brightness);
    DEBUG_PRINTF("Display brightness set to %d\n", _brightness);
}

uint8_t DisplayHAL::getBrightness() {
    return _brightness;
}

void DisplayHAL::enterPowerSaveMode() {
    if (!_powerSaveMode) {
        _powerSaveMode = true;
        uint8_t prevBrightness = _brightness;
        setBrightness(SCREEN_BRIGHTNESS_POWER_SAVE);
        DEBUG_PRINTF("Entered power save mode (brightness reduced from %d to %d)\n", 
                    prevBrightness, SCREEN_BRIGHTNESS_POWER_SAVE);
    }
}

void DisplayHAL::exitPowerSaveMode() {
    if (_powerSaveMode) {
        _powerSaveMode = false;
        setBrightness(SCREEN_BRIGHTNESS_NORMAL);
        DEBUG_PRINT("Exited power save mode");
    }
}

void DisplayHAL::lvglFlushCb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    M5.Display.startWrite();
    M5.Display.setAddrWindow(area->x1, area->y1, w, h);
    M5.Display.writePixels((lgfx::rgb565_t*)&color_p->full, w * h);
    M5.Display.endWrite();
    
    lv_disp_flush_ready(disp);
}

void DisplayHAL::lvglTouchReadCb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = M5.Display.getTouch(&touchX, &touchY);
    
    if (touched) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
        
        // Reset power save timer on touch
        _lastActivityTime = millis();
        if (_powerSaveMode) {
            exitPowerSaveMode();
        }
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void DisplayHAL::update() {
    // Check for power save mode
    if (!_powerSaveMode && (millis() - _lastActivityTime > POWER_SAVE_TIMEOUT)) {
        enterPowerSaveMode();
    }
}
