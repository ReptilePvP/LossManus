/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Touch Input Interface
 * 
 * This file contains the interface for touch input operations
 */

#ifndef HAL_TOUCH_H
#define HAL_TOUCH_H

#include <M5Unified.h>
#include "../config.h"

// Touch event types
enum TouchEventType {
    TOUCH_NONE,
    TOUCH_DOWN,
    TOUCH_UP,
    TOUCH_MOVE,
    TOUCH_HOLD
};

// Touch event structure
struct TouchEvent {
    TouchEventType type;
    uint16_t x;
    uint16_t y;
    uint32_t timestamp;
    uint16_t duration; // For hold events
};

// Touch gesture types
enum TouchGestureType {
    GESTURE_NONE,
    GESTURE_TAP,
    GESTURE_DOUBLE_TAP,
    GESTURE_SWIPE_LEFT,
    GESTURE_SWIPE_RIGHT,
    GESTURE_SWIPE_UP,
    GESTURE_SWIPE_DOWN,
    GESTURE_PINCH_IN,
    GESTURE_PINCH_OUT
};

// Touch gesture structure
struct TouchGesture {
    TouchGestureType type;
    uint16_t x;
    uint16_t y;
    uint32_t timestamp;
    int16_t deltaX; // For swipe gestures
    int16_t deltaY; // For swipe gestures
    float scale;    // For pinch gestures
};

// Touch callback function type
typedef void (*TouchEventCallback)(const TouchEvent& event);
typedef void (*TouchGestureCallback)(const TouchGesture& gesture);

class TouchHAL {
public:
    /**
     * Initialize the touch input system
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Update touch state (call in loop)
     */
    static void update();
    
    /**
     * Check if screen is currently touched
     * @return true if touched, false otherwise
     */
    static bool isTouched();
    
    /**
     * Get current touch coordinates
     * @param x pointer to store x coordinate
     * @param y pointer to store y coordinate
     * @return true if touched, false otherwise
     */
    static bool getTouchCoordinates(uint16_t* x, uint16_t* y);
    
    /**
     * Get last touch event
     * @return last touch event
     */
    static TouchEvent getLastTouchEvent();
    
    /**
     * Get last touch gesture
     * @return last touch gesture
     */
    static TouchGesture getLastTouchGesture();
    
    /**
     * Register touch event callback
     * @param callback function to call on touch events
     */
    static void registerTouchEventCallback(TouchEventCallback callback);
    
    /**
     * Register touch gesture callback
     * @param callback function to call on touch gestures
     */
    static void registerTouchGestureCallback(TouchGestureCallback callback);
    
    /**
     * Set touch sensitivity
     * @param sensitivity value (1-10)
     */
    static void setSensitivity(uint8_t sensitivity);
    
    /**
     * Get touch sensitivity
     * @return sensitivity value (1-10)
     */
    static uint8_t getSensitivity();
    
    /**
     * Calibrate touch input
     * @return true if successful, false otherwise
     */
    static bool calibrate();

private:
    static bool _initialized;
    static TouchEvent _lastEvent;
    static TouchGesture _lastGesture;
    static TouchEventCallback _eventCallback;
    static TouchGestureCallback _gestureCallback;
    static uint8_t _sensitivity;
    static uint16_t _lastX, _lastY;
    static uint32_t _touchStartTime;
    static bool _isTouched;
    
    static void detectGesture();
};

#endif // HAL_TOUCH_H
