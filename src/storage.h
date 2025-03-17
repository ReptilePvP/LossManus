/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Storage Interface
 * 
 * This file contains the interface for storage operations (SD card)
 */

#ifndef HAL_STORAGE_H
#define HAL_STORAGE_H

#include <SD.h>
#include <SPI.h>
#include "../config.h"

class StorageHAL {
public:
    /**
     * Initialize the storage system
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Check if storage is available
     * @return true if available, false otherwise
     */
    static bool isAvailable();
    
    /**
     * Get total storage space in bytes
     * @return total space in bytes
     */
    static uint64_t getTotalSpace();
    
    /**
     * Get free storage space in bytes
     * @return free space in bytes
     */
    static uint64_t getFreeSpace();
    
    /**
     * Read file content
     * @param path file path
     * @param buffer buffer to store content
     * @param maxLen maximum length to read
     * @return number of bytes read, -1 on error
     */
    static int readFile(const char* path, char* buffer, size_t maxLen);
    
    /**
     * Write file content
     * @param path file path
     * @param buffer content to write
     * @param len length of content
     * @return number of bytes written, -1 on error
     */
    static int writeFile(const char* path, const char* buffer, size_t len);
    
    /**
     * Append to file
     * @param path file path
     * @param buffer content to append
     * @param len length of content
     * @return number of bytes written, -1 on error
     */
    static int appendFile(const char* path, const char* buffer, size_t len);
    
    /**
     * Delete file
     * @param path file path
     * @return true if successful, false otherwise
     */
    static bool deleteFile(const char* path);
    
    /**
     * Check if file exists
     * @param path file path
     * @return true if exists, false otherwise
     */
    static bool fileExists(const char* path);
    
    /**
     * Create directory
     * @param path directory path
     * @return true if successful, false otherwise
     */
    static bool createDir(const char* path);
    
    /**
     * List files in directory
     * @param path directory path
     * @param extension filter by extension (optional)
     * @return array of filenames
     */
    static String* listDir(const char* path, const char* extension = nullptr);
    
    /**
     * Get file size
     * @param path file path
     * @return file size in bytes, -1 on error
     */
    static int getFileSize(const char* path);
    
    /**
     * Backup file
     * @param sourcePath source file path
     * @param backupDir backup directory
     * @return true if successful, false otherwise
     */
    static bool backupFile(const char* sourcePath, const char* backupDir);

private:
    static bool _initialized;
    static void releaseSPIBus();
    static void acquireSPIBus();
};

#endif // HAL_STORAGE_H
