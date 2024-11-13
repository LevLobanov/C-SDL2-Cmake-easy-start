#include <iostream>
#include "windows.h"
#include <fcntl.h>
#include <cstdint>
#include "logger.h"
#include <ctime>


Logger::Logger(LoggerMode mode) : LFStream()
{
    #ifdef _DEBUG
    Mode = mode;
    if (Mode == LMode_Console)
    {
        AllocConsole();
        
        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
        FILE *hf_out = _fdopen(hCrt, "w");
        setvbuf(hf_out, NULL, _IONBF, 1);
        *stdout = *hf_out;

        HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
        hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
        FILE *hf_in = _fdopen(hCrt, "r");
        setvbuf(hf_in, NULL, _IONBF, 128);
        *stdin = *hf_in;

        HANDLE handle_err = GetStdHandle(STD_ERROR_HANDLE);
        hCrt = _open_osfhandle((intptr_t)handle_err, _O_TEXT);
        FILE *hf_err = _fdopen(hCrt, "w");
        setvbuf(stderr, NULL, _IONBF, 0);
        *stderr = *hf_err;
    }
    else
    {
        LFStream.open("game.log", std::ios::app);
    }
    #endif
}


Logger::~Logger()
{
    #ifdef _DEBUG
    bool result = true;
    FILE *fp;
    if (freopen_s(&fp, "NUL:", "r", stdin) == 0)
        setvbuf(stdin, NULL, _IONBF, 0);
    if (freopen_s(&fp, "NUL:", "w", stdout) == 0)
        setvbuf(stdout, NULL, _IONBF, 0);
    if (freopen_s(&fp, "NUL:", "w", stderr) == 0)
        setvbuf(stderr, NULL, _IONBF, 0);
    FreeConsole();
    #endif
}

void Logger::log(LogLevel level, const std::string& message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << levelToString(level) << ": " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << levelToString(level) << ": " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


void Logger::debug(const std::string& message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << "DEBUG: " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << "DEBUG: " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


void Logger::info(const std::string& message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << "INFO: " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << "INFO: " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


void Logger::warn(const std::string& message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << "WARNING: " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << "WARNING: " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


void Logger::error(const std::string& message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << "ERROR: " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << "ERROR: " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


void Logger::error(int errorCode, const std::string &message)
{
    #ifdef _DEBUG
    if(Mode == LMode_Console) {
        std::cout << "[" << errorCode << "] ERROR: " << message << std::endl;
    } else {
        if (LFStream.is_open()) {
            LFStream << "[" << errorCode << "] ERROR: " << message << std::endl;
            LFStream.flush();
        }
    }
    #endif
}


std::string Logger::levelToString(LogLevel level)
{
    switch (level) {
    case L_DEBUG:
        return "DEBUG";
    case L_INFO:
        return "INFO";
    case L_WARNING:
        return "WARNING";
    case L_ERROR:
        return "ERROR"; 
    default:
        return "UNKNOWN";
    }
}


char Logger::getCurrentTime()
{
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    return *timestamp;
}