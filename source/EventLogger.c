//
//  EventLogger.c
//  MrsWatson
//
//  Created by Nik Reiman on 1/2/12.
//  Copyright (c) 2012 Teragon Audio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include "EventLogger.h"

#define ANSI_COLOR_RED     "[31m"
#define ANSI_COLOR_GREEN   "[32m"
#define ANSI_COLOR_YELLOW  "[33m"
#define ANSI_COLOR_BLUE    "[34m"
#define ANSI_COLOR_MAGENTA "[35m"
#define ANSI_COLOR_CYAN    "[36m"
#define ANSI_COLOR_WHITE   "[37m"
#define ANSI_COLOR_RESET   "[0m"

EventLogger eventLoggerGlobalInstance;

void initEventLogger(void) {
  eventLoggerGlobalInstance = malloc(sizeof(EventLoggerMembers));
  eventLoggerGlobalInstance->logLevel = LOG_INFO;
  eventLoggerGlobalInstance->startTime = time(NULL);
  eventLoggerGlobalInstance->colorLogging = false;
}

static EventLogger _getGlobalInstance(void) {
  return eventLoggerGlobalInstance;
}

void setLogLevel(LogLevel logLevel) {
  EventLogger eventLogger = _getGlobalInstance();
  eventLogger->logLevel = logLevel;
}

void setColorLogging(bool enabled) {
  EventLogger eventLogger = _getGlobalInstance();
  eventLogger->colorLogging = enabled;
}

static char _logLevelStatusChar(const LogLevel logLevel) {
  switch(logLevel) {
    case LOG_DEBUG:    return ' ';
    case LOG_INFO:     return '-';
    case LOG_ERROR:    return '!';
    case LOG_CRITICAL: return '*';
    default:           return ' ';
  }
}

static const char* _logLevelStatusColor(const LogLevel logLevel) {
  switch(logLevel) {
    case LOG_DEBUG:    return ANSI_COLOR_WHITE;
    case LOG_INFO:     return ANSI_COLOR_GREEN;
    case LOG_ERROR:    return ANSI_COLOR_RED;
    case LOG_CRITICAL: return ANSI_COLOR_YELLOW;
    default:           return ANSI_COLOR_WHITE;
  }
}

static void _printMessage(const LogLevel logLevel, const long elapsedTime, const bool colorLogging, const char* message) {
  if(colorLogging) {
    fprintf(stderr, "\x1b%s%c %06ld %s\x1b%s\n", _logLevelStatusColor(logLevel), _logLevelStatusChar(logLevel), elapsedTime, message, ANSI_COLOR_RESET);
  }
  else {
    fprintf(stderr, "%c %06ld %s\n", _logLevelStatusChar(logLevel), elapsedTime, message);
  }
}

void logMessage(const LogLevel logLevel, const char* message) {
  EventLogger eventLogger = _getGlobalInstance();
  if(logLevel >= eventLogger->logLevel) {
    time_t currentTime = time(NULL);
    _printMessage(logLevel, currentTime - eventLogger->startTime, eventLogger->colorLogging, message);
  }
}

void logDebug(const char* message) {
  logMessage(LOG_DEBUG, message);
}

void logInfo(const char* message) {
  logMessage(LOG_INFO, message);
}

void logError(const char* message) {
  logMessage(LOG_ERROR, message);
}

void logCritical(const char* message) {
  logMessage(LOG_CRITICAL, message);
}