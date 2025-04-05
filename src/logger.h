#ifndef LOGGER_H
#define LOGGER_H
#include "types.h"
// Logs a basic message with timestamp
void logMsg(const char* format, ...);

// Logs a message with timestamp and error ID
void logError(errorId_t err, const char* format, ...);

//Logs function entering
void logEnter(const char* fName);

//Logs function leaving
void logLeave(const char* fName);
#endif