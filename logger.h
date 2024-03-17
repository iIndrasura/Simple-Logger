// Logger.h

/*
 * File:   Logger.h
 * Author: Indrasura
 *
 * Created on March 15, 2024, 6:00 PM
 */

#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

// Build flag for conditional compilation
#ifdef DEBUG_ENABLED
    #define LOGGING_ENABLED
#endif

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger
{
public:
    // Set minimum logging level
    static void setLogLevel(LogLevel level)
    {
    #ifdef LOGGING_ENABLED
            logLevel_ = level;
    #endif
    }

    // Set output stream (default: console)
    static void setOutputStream(std::ostream& stream)
    {
        outputStream_ = &stream;
    }

    // Optionally enable logging to a file
    static void setLogFile(const std::string& filename)
    {
    #ifdef LOGGING_ENABLED
            if (logFile_.is_open()) {
                logFile_.close(); // Close existing file
            }
            logFile_.open(filename, std::ofstream::out | std::ofstream::app);
            if (!logFile_.is_open()) {
                std::cerr << "Failed to open log file: " << filename << std::endl;
            }
    #endif
    }

    template <typename T>
    static void log(LogLevel level, const T& message)
    {
    #ifdef LOGGING_ENABLED
            if (level >= logLevel_) {
                std::lock_guard<std::mutex> lock(mutex_);
                *outputStream_ << logPrefix(level) << stringify(message) << std::endl;
                if (logFile_.is_open()) {
                    logFile_ << logPrefix(level) << stringify(message) << std::endl;
                }
            }
    #endif
    }

private:
#ifdef LOGGING_ENABLED
    static LogLevel logLevel_;
#endif
    static std::mutex mutex_;
    static std::ostream* outputStream_;
    static std::ofstream logFile_;

    static std::string logPrefix(LogLevel level)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        struct tm timeinfo;
        localtime_s(&timeinfo, &time);
        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << " ";

        switch (level) {
        case LogLevel::DEBUG:
            oss << "[DEBUG]: ";
            break;
        case LogLevel::INFO:
            oss << "[INFO]: ";
            break;
        case LogLevel::WARNING:
            oss << "[WARNING]: ";
            break;
        case LogLevel::ERROR:
            oss << "[ERROR]: ";
            break;
        case LogLevel::CRITICAL:
            oss << "[CRITICAL]: ";
            break;
        }
        return oss.str();
    }

    // Convert non-string types to string
    template <typename T>
    static std::string stringify(const T& value)
    {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    Logger() = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Default initialization
};

// Default settings
#ifdef LOGGING_ENABLED
    LogLevel Logger::logLevel_ = LogLevel::INFO;
#endif
    std::mutex Logger::mutex_;
    std::ostream* Logger::outputStream_ = &std::cout;
    std::ofstream Logger::logFile_;

// Convenience macros (defined only if LOGGING_ENABLED)
#ifdef LOGGING_ENABLED
    #define LOG_INFO(message) do { \
                                    std::ostringstream oss; \
                                    oss << message; \
                                    Logger::log(LogLevel::INFO, oss.str()); \
                                } while(0)
    #define LOG_DEBUG(message) do { \
                                    std::ostringstream oss; \
                                    oss << message; \
                                    Logger::log(LogLevel::DEBUG, oss.str()); \
                                } while(0)
    #define LOG_WARNING(message) do { \
                                    std::ostringstream oss; \
                                    oss << message; \
                                    Logger::log(LogLevel::WARNING, oss.str()); \
                                } while(0)
    #define LOG_ERROR(message) do { \
                                    std::ostringstream oss; \
                                    oss << message; \
                                    Logger::log(LogLevel::ERROR, oss.str()); \
                                } while(0)
    #define LOG_CRITICAL(message) do { \
                                        std::ostringstream oss; \
                                        oss << message; \
                                        Logger::log(LogLevel::CRITICAL, oss.str()); \
                                    } while(0)
#else
    #define LOG_DEBUG(message)
    #define LOG_INFO(message)
    #define LOG_WARNING(message)
    #define LOG_ERROR(message)
    #define LOG_CRITICAL(message)
#endif

#endif // LOGGER_HPP
