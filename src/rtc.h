/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - RTC Interface
 * 
 * This file contains the interface for Real-Time Clock operations
 */

#ifndef HAL_RTC_H
#define HAL_RTC_H

#include <time.h>
#include "../config.h"

class RtcHAL {
public:
    /**
     * Initialize the RTC
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Get current time as time_t
     * @return current time
     */
    static time_t getTime();
    
    /**
     * Get current time as tm struct
     * @param timeinfo pointer to tm struct to fill
     * @return true if successful, false otherwise
     */
    static bool getTimeInfo(struct tm* timeinfo);
    
    /**
     * Set RTC time
     * @param time time_t value to set
     * @return true if successful, false otherwise
     */
    static bool setTime(time_t time);
    
    /**
     * Set RTC time from components
     * @param year year (full year, e.g. 2025)
     * @param month month (1-12)
     * @param day day (1-31)
     * @param hour hour (0-23)
     * @param minute minute (0-59)
     * @param second second (0-59)
     * @return true if successful, false otherwise
     */
    static bool setTime(int year, int month, int day, int hour, int minute, int second);
    
    /**
     * Sync RTC with NTP server
     * @param ntpServer NTP server address
     * @return true if successful, false otherwise
     */
    static bool syncWithNTP(const char* ntpServer = "pool.ntp.org");
    
    /**
     * Format time as string
     * @param time time_t to format
     * @param format strftime format string
     * @return formatted time string
     */
    static String formatTime(time_t time, const char* format);
    
    /**
     * Format current time as string
     * @param format strftime format string
     * @return formatted time string
     */
    static String formatCurrentTime(const char* format);

private:
    static bool _initialized;
    static const char* _timeZone;
};

#endif // HAL_RTC_H
