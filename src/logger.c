#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#define LOG_FILE "logs/log.log"

// Helper function to write the timestamp at the beginning of each log entry
static void write_log_prefix(FILE* file) {
    time_t now;
    time(&now);
    char* timeStr = ctime(&now);
    timeStr[strcspn(timeStr, "\n")] = 0;  // Remove the trailing newline from ctime
    fprintf(file, "%s: ", timeStr);
}

// Function to log a message without error codes
void logMsg(const char* format, ...) {
    FILE* file = fopen(LOG_FILE, "a");
    if (!file) {
        perror("Failed to open log file");
        return;
    }

    write_log_prefix(file);

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, "\n");
    fclose(file);
}

// Function to log a message with an error code
void logError(errorId_t err, const char* format, ...) {
    FILE* file = fopen(LOG_FILE, "a");
    if (!file) {
        perror("Failed to open log file");
        return;
    }

    write_log_prefix(file);

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, " | ERROR CODE: %d\n", err);
    fclose(file);
}

// Function to log entry to a function
void logEnter(const char* fName) {
    logMsg("Enter function %s", fName);
}

// Function to log exit from a function
void logLeave(const char* fName) {
    logMsg("Leave function %s", fName);
}
