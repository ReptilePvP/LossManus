/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Export Utilities
 * 
 * This file contains utilities for exporting data in various formats
 */

#ifndef DATA_EXPORT_H
#define DATA_EXPORT_H

#include <Arduino.h>
#include <vector>
#include "log_entry.h"
#include "../hal/storage.h"
#include "../config.h"

enum ExportFormat {
    FORMAT_CSV,
    FORMAT_JSON,
    FORMAT_TEXT
};

class ExportUtil {
public:
    /**
     * Export log entries to file in specified format
     * @param entries vector of log entries to export
     * @param filename file to export to
     * @param format export format
     * @return true if successful, false otherwise
     */
    static bool exportToFile(const std::vector<LogEntry>& entries, const String& filename, ExportFormat format);
    
    /**
     * Export log entries to CSV format
     * @param entries vector of log entries to export
     * @return CSV formatted string
     */
    static String exportToCSV(const std::vector<LogEntry>& entries);
    
    /**
     * Export log entries to JSON format
     * @param entries vector of log entries to export
     * @return JSON formatted string
     */
    static String exportToJSON(const std::vector<LogEntry>& entries);
    
    /**
     * Export log entries to plain text format
     * @param entries vector of log entries to export
     * @return text formatted string
     */
    static String exportToText(const std::vector<LogEntry>& entries);
};

#endif // DATA_EXPORT_H
