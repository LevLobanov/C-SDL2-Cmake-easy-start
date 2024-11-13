/**
 * @file logger.h
 * @brief Provides logger for SDL gui apps. WORKS ONLY ON WINDOWS!
 * 
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


/**
 * @brief Used to define a level for logger.
 * Colors used in each level:
 * L_DEBUG - Blue.
 * L_INFO - White.
 * L_WARNING - Yellow.
 * L_ERROR - Red.
 *
 */
enum LogLevel
{
    L_DEBUG,
    L_INFO,
    L_WARNING,
    L_ERROR
};


/**
 * @brief Used to define output for logs.
 * 
 */
enum LoggerMode
{
    LMode_Console,
    LMode_File
};


/**
 * @brief A logger instance. Use methods of Logger class to log colored info, debug, errors and warnings.
 * Really logs anything only if "_DEBUG" preprocessor definition is set.
 * This is singleton, so one instance for all code. Use it like this: Logger::getInstance(LMode_Console).error(0, "Error message");
 *
 */
class Logger
{
public:
    static Logger &getInstance(LoggerMode mode)
    {
        static Logger logger(mode);
        return logger;
    }
    
    /**
     * @brief Log message with provided level in white color.
     *
     * @param level instance of LogLevel, defines level of message.
     * @param message contains of log message.
     */
    void log(LogLevel level, const std::string &message);

    /**
     * @brief log message with level DEBUG, colored in blue.
     *
     * @param message contains of log message.
     */
    void debug(const std::string &message);

    /**
     * @brief log message with level INFO, colored in white.
     *
     * @param message contains of log message.
     */
    void info(const std::string &message);

    /**
     * @brief log message with level WARNING, colored in yellow.
     *
     * @param message contains of log message.
     */
    void warn(const std::string &message);

    /**
     * @brief log message with level ERROR, colored in red.
     *
     * @param message contains of log message.
     */
    void error(const std::string &message);

    /**
     * @brief log message with level ERROR and error code in brackets, colored in red.
     *
     * @param errorCode
     * @param message
     */
    void error(int errorCode, const std::string &message);

private:
    std::ofstream LFStream; // File stream for logger file mode
    LoggerMode Mode; // Working mode of logger, defines output of logger

    Logger(LoggerMode mode); // Private constructor to prevent external instantiation

    ~Logger(); // Private destructor to prevent external destruction

    /**
     * @brief Converts logging level in string format.
     *
     * @param level Logging level
     * @return std::string String format of logging level
     */
    std::string levelToString(LogLevel level);

    /**
     * @brief Get the Current Time.
     *
     * @return char Formatted time
     */
    char getCurrentTime();
};

#endif