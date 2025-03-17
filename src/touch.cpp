/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Touch Input Implementation
 */

#include "touch.h"

// Static member initialization
bool TouchHAL::_initialized = false;
TouchEvent TouchHAL::_lastEvent = {TOUCH_NONE, 0, 0, 0, 0};
TouchGesture TouchHAL::_lastGesture = {GESTURE_NONE, 0, 0, 0, 0, 0, 0};
TouchEventCallback TouchHAL::_eventCallback = nullptr;
TouchGestureCallback TouchHAL::_gestureCallback = nullptr;
uint8_t TouchHAL::_sensitivity = 5;
uint16_t TouchHAL::_lastX = 0, TouchHAL::_lastY = 0;
uint32_t TouchHAL::_touchStartTime = 0;
bool TouchHAL::_isTouched = false;

bool TouchHAL::init() {
    DEBUG_PRINT("Initializing touch input system...");
    
    if (_initialized) {
        DEBUG_PRINT("Touch input system already initialized");
        return true;
    }
    
    // M5Stack CoreS3 touch is initialized with M5.begin() in display.cpp
    // No additional initialization needed here
    
    _initialized = true;
    _lastEvent.type = TOUCH_NONE;
    _lastGesture.type = GESTURE_NONE;
    
    DEBUG_PRINT("Touch input system initialization complete");
    return true;
}

void TouchHAL::update() {
    if (!_initialized) {
        return;
    }
    
    uint16_t x, y;
    bool touched = M5.Display.getTouch(&x, &y);
    uint32_t currentTime = millis();
    
    // Process touch events
    if (touched) {
        if (!_isTouched) {
            // Touch down event
            _isTouched = true;
            _touchStartTime = currentTime;
            _lastX = x;
            _lastY = y;
            
            _lastEvent.type = TOUCH_DOWN;
            _lastEvent.x = x;
            _lastEvent.y = y;
            _lastEvent.timestamp = currentTime;
            _lastEvent.duration = 0;
            
            if (_eventCallback) {
                _eventCallback(_lastEvent);
            }
        } else {
            // Check if touch position has moved
            int16_t deltaX = x - _lastX;
            int16_t deltaY = y - _lastY;
            
            if (abs(deltaX) > _sensitivity || abs(deltaY) > _sensitivity) {
                // Touch move event
                _lastEvent.type = TOUCH_MOVE;
                _lastEvent.x = x;
                _lastEvent.y = y;
                _lastEvent.timestamp = currentTime;
                _lastEvent.duration = currentTime - _touchStartTime;
                
                _lastX = x;
                _lastY = y;
                
                if (_eventCallback) {
                    _eventCallback(_lastEvent);
                }
            } else if (currentTime - _touchStartTime > 500) {
                // Touch hold event (after 500ms)
                _lastEvent.type = TOUCH_HOLD;
                _lastEvent.x = x;
                _lastEvent.y = y;
                _lastEvent.timestamp = currentTime;
                _lastEvent.duration = currentTime - _touchStartTime;
                
                if (_eventCallback) {
                    _eventCallback(_lastEvent);
                }
            }
        }
    } else if (_isTouched) {
        // Touch up event
        _isTouched = false;
        
        _lastEvent.type = TOUCH_UP;
        _lastEvent.x = _lastX;
        _lastEvent.y = _lastY;
        _lastEvent.timestamp = currentTime;
        _lastEvent.duration = currentTime - _touchStartTime;
        
        if (_eventCallback) {
            _eventCallback(_lastEvent);
        }
        
        // Detect gesture
        detectGesture();
    }
}

bool TouchHAL::isTouched() {
    return _isTouched;
}

bool TouchHAL::getTouchCoordinates(uint16_t* x, uint16_t* y) {
    if (!_initialized || !_isTouched) {
        return false;
    }
    
    *x = _lastX;
    *y = _lastY;
    return true;
}

TouchEvent TouchHAL::getLastTouchEvent() {
    return _lastEvent;
}

TouchGesture TouchHAL::getLastTouchGesture() {
    return _lastGesture;
}

void TouchHAL::registerTouchEventCallback(TouchEventCallback callback) {
    _eventCallback = callback;
}

void TouchHAL::registerTouchGestureCallback(TouchGestureCallback callback) {
    _gestureCallback = callback;
}

void TouchHAL::setSensitivity(uint8_t sensitivity) {
    if (sensitivity < 1) sensitivity = 1;
    if (sensitivity > 10) sensitivity = 10;
    _sensitivity = sensitivity;
}

uint8_t TouchHAL::getSensitivity() {
    return _sensitivity;
}

bool TouchHAL::calibrate() {
    // M5Stack CoreS3 doesn't require touch calibration
    return true;
}

void TouchHAL::detectGesture() {
    uint32_t currentTime = millis();
    uint32_t touchDuration = currentTime - _touchStartTime;
    
    // Reset gesture
    _lastGesture.type = GESTURE_NONE;
    
    // Get touch start and end positions
    uint16_t startX = _lastEvent.x;
    uint16_t startY = _lastEvent.y;
    uint16_t endX = _lastX;
    uint16_t endY = _lastY;
    
    // Calculate deltas
    int16_t deltaX = endX - startX;
    int16_t deltaY = endY - startY;
    
    // Detect tap
    if (touchDuration < 200 && abs(deltaX) < 20 && abs(deltaY) < 20) {
        // Simple tap
        _lastGesture.type = GESTURE_TAP;
        _lastGesture.x = endX;
        _lastGesture.y = endY;
        _lastGesture.timestamp = currentTime;
        _lastGesture.deltaX = deltaX;
        _lastGesture.deltaY = deltaY;
        
        // Double tap detection would go here
    }
    // Detect swipes
    else if (touchDuration < 500) {
        int16_t absX = abs(deltaX);
        int16_t absY = abs(deltaY);
        
        if (absX > 50 || absY > 50) {
            if (absX > absY) {
                // Horizontal swipe
                if (deltaX > 0) {
                    _lastGesture.type = GESTURE_SWIPE_RIGHT;
                } else {
                    _lastGesture.type = GESTURE_SWIPE_LEFT;
                }
            } else {
                // Vertical swipe
                if (deltaY > 0) {
                    _lastGesture.type = GESTURE_SWIPE_DOWN;
                } else {
                    _lastGesture.type = GESTURE_SWIPE_UP;
                }
            }
            
            _lastGesture.x = endX;
            _lastGesture.y = endY;
            _lastGesture.timestamp = currentTime;
            _lastGesture.deltaX = deltaX;
            _lastGesture.deltaY = deltaY;
        }
    }
    
    // Call gesture callback if registered and gesture detected
    if (_gestureCallback && _lastGesture.type != GESTURE_NONE) {
        _gestureCallback(_lastGesture);
    }
}
