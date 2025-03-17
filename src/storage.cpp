/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Storage Implementation
 */

#include "storage.h"

// Static member initialization
bool StorageHAL::_initialized = false;

bool StorageHAL::init() {
    DEBUG_PRINT("Initializing storage system...");
    
    if (_initialized) {
        DEBUG_PRINT("Storage already initialized");
        return true;
    }
    
    // Configure SPI for SD card
    acquireSPIBus();
    
    // Initialize SD card
    if (!SD.begin(SD_SPI_CS_PIN)) {
        DEBUG_PRINT("SD card initialization failed!");
        return false;
    }
    
    _initialized = true;
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    DEBUG_PRINTF("SD Card initialized. Size: %llu MB\n", cardSize);
    
    // Create backup directory if it doesn't exist
    if (!SD.exists("/backup")) {
        createDir("/backup");
    }
    
    return true;
}

bool StorageHAL::isAvailable() {
    return _initialized;
}

uint64_t StorageHAL::getTotalSpace() {
    if (!_initialized) return 0;
    return SD.cardSize();
}

uint64_t StorageHAL::getFreeSpace() {
    if (!_initialized) return 0;
    // SD library doesn't have a direct method for free space
    // This is an approximation
    return SD.cardSize() / 2; // Placeholder - would need a proper implementation
}

int StorageHAL::readFile(const char* path, char* buffer, size_t maxLen) {
    if (!_initialized) return -1;
    
    acquireSPIBus();
    
    File file = SD.open(path, FILE_READ);
    if (!file) {
        DEBUG_PRINTF("Failed to open file for reading: %s\n", path);
        return -1;
    }
    
    size_t bytesRead = file.read((uint8_t*)buffer, maxLen - 1);
    buffer[bytesRead] = '\0'; // Null terminate
    
    file.close();
    return bytesRead;
}

int StorageHAL::writeFile(const char* path, const char* buffer, size_t len) {
    if (!_initialized) return -1;
    
    acquireSPIBus();
    
    // Delete file if it exists
    if (SD.exists(path)) {
        SD.remove(path);
    }
    
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
        DEBUG_PRINTF("Failed to open file for writing: %s\n", path);
        return -1;
    }
    
    size_t bytesWritten = file.write((const uint8_t*)buffer, len);
    file.close();
    
    if (bytesWritten != len) {
        DEBUG_PRINTF("Write incomplete: %d/%d bytes written\n", bytesWritten, len);
    }
    
    return bytesWritten;
}

int StorageHAL::appendFile(const char* path, const char* buffer, size_t len) {
    if (!_initialized) return -1;
    
    acquireSPIBus();
    
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        DEBUG_PRINTF("Failed to open file for appending: %s\n", path);
        return -1;
    }
    
    size_t bytesWritten = file.write((const uint8_t*)buffer, len);
    file.close();
    
    if (bytesWritten != len) {
        DEBUG_PRINTF("Append incomplete: %d/%d bytes written\n", bytesWritten, len);
    }
    
    return bytesWritten;
}

bool StorageHAL::deleteFile(const char* path) {
    if (!_initialized) return false;
    
    acquireSPIBus();
    
    if (!SD.exists(path)) {
        DEBUG_PRINTF("File not found for deletion: %s\n", path);
        return false;
    }
    
    if (SD.remove(path)) {
        DEBUG_PRINTF("File deleted: %s\n", path);
        return true;
    } else {
        DEBUG_PRINTF("Failed to delete file: %s\n", path);
        return false;
    }
}

bool StorageHAL::fileExists(const char* path) {
    if (!_initialized) return false;
    
    acquireSPIBus();
    return SD.exists(path);
}

bool StorageHAL::createDir(const char* path) {
    if (!_initialized) return false;
    
    acquireSPIBus();
    
    if (SD.mkdir(path)) {
        DEBUG_PRINTF("Directory created: %s\n", path);
        return true;
    } else {
        DEBUG_PRINTF("Failed to create directory: %s\n", path);
        return false;
    }
}

String* StorageHAL::listDir(const char* path, const char* extension) {
    static String fileList[20]; // Static array to return
    int fileCount = 0;
    
    if (!_initialized) return fileList;
    
    acquireSPIBus();
    
    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) {
        DEBUG_PRINTF("Failed to open directory: %s\n", path);
        return fileList;
    }
    
    File entry;
    while ((entry = dir.openNextFile()) && fileCount < 20) {
        if (!entry.isDirectory()) {
            String fileName = entry.name();
            
            // Filter by extension if provided
            if (extension == nullptr || fileName.endsWith(extension)) {
                fileList[fileCount++] = fileName;
            }
        }
        entry.close();
    }
    
    dir.close();
    return fileList;
}

int StorageHAL::getFileSize(const char* path) {
    if (!_initialized) return -1;
    
    acquireSPIBus();
    
    File file = SD.open(path, FILE_READ);
    if (!file) {
        DEBUG_PRINTF("Failed to open file to get size: %s\n", path);
        return -1;
    }
    
    int size = file.size();
    file.close();
    return size;
}

bool StorageHAL::backupFile(const char* sourcePath, const char* backupDir) {
    if (!_initialized) return false;
    
    acquireSPIBus();
    
    // Check if source file exists
    if (!SD.exists(sourcePath)) {
        DEBUG_PRINTF("Source file not found for backup: %s\n", sourcePath);
        return false;
    }
    
    // Create backup directory if it doesn't exist
    if (!SD.exists(backupDir)) {
        if (!createDir(backupDir)) {
            return false;
        }
    }
    
    // Extract filename from path
    String fileName = String(sourcePath);
    int lastSlash = fileName.lastIndexOf('/');
    if (lastSlash >= 0) {
        fileName = fileName.substring(lastSlash + 1);
    }
    
    // Create backup filename with timestamp
    char timestamp[20];
    time_t now;
    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
    
    String backupPath = String(backupDir);
    if (backupPath.charAt(backupPath.length() - 1) != '/') {
        backupPath += '/';
    }
    backupPath += fileName + "." + String(timestamp);
    
    // Copy file
    File sourceFile = SD.open(sourcePath, FILE_READ);
    if (!sourceFile) {
        DEBUG_PRINTF("Failed to open source file for backup: %s\n", sourcePath);
        return false;
    }
    
    File backupFile = SD.open(backupPath.c_str(), FILE_WRITE);
    if (!backupFile) {
        DEBUG_PRINTF("Failed to create backup file: %s\n", backupPath.c_str());
        sourceFile.close();
        return false;
    }
    
    // Copy data in chunks
    const size_t bufferSize = 512;
    uint8_t buffer[bufferSize];
    size_t bytesRead;
    
    while ((bytesRead = sourceFile.read(buffer, bufferSize)) > 0) {
        backupFile.write(buffer, bytesRead);
    }
    
    sourceFile.close();
    backupFile.close();
    
    DEBUG_PRINTF("File backed up: %s -> %s\n", sourcePath, backupPath.c_str());
    return true;
}

void StorageHAL::releaseSPIBus() {
    SPI.end();
}

void StorageHAL::acquireSPIBus() {
    releaseSPIBus();
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
}
