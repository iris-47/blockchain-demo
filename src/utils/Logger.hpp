// Logger.hpp

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <cstdarg>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

class Logger {
public:
    // 禁止拷贝构造和赋值操作
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // 日志级别
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
    };

    // 获取Logger实例
    static Logger& getLogger(LogLevel level = LogLevel::INFO) {
        static Logger instance;
        instance.currentLevel = level;
        return instance;
    }

    void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    // 设置日志输出到文件
    void setOutputFile(const std::string& filename) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_outputFile.is_open()) {
            _outputFile.close();
        }
        _outputFile.open(filename, std::ios::out);
        if (!_outputFile.is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
        _useFile = true;
    }

    // 设置日志输出到标准输出
    void setOutputStdout() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_outputFile.is_open()) {
            _outputFile.close();
        }
        _useFile = false;
    }

    // 使用流的方式记录日志
    template <typename T>
    Logger& operator<<(const T& msg) {
        if (currentLevel < logLevel) return *this;
        std::lock_guard<std::mutex> lock(_mutex);

        std::ostream& out = (_useFile && _outputFile.is_open()) ? _outputFile : std::cout;
        out << msg;

        return *this;
    }

    // 用于重载std::endl
    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        if (currentLevel < logLevel) return *this;
        std::lock_guard<std::mutex> lock(_mutex);

        std::ostream& out = (_useFile && _outputFile.is_open()) ? _outputFile : std::cout;
        out << manip;

        return *this;
    }

    // 记录日志
    void log(LogLevel level, const std::string& msg) {
        if (level < logLevel) return;
        std::lock_guard<std::mutex> lock(_mutex);

        std::ostream& out = (_useFile && _outputFile.is_open()) ? _outputFile : std::cout;
        out << "[" << currentDateTime() << "] [" << getLogLevelString(level) << "] " << msg << std::endl;
    }

    void log(LogLevel level, const char* format, ...) {
        if (level < logLevel) return;
        std::lock_guard<std::mutex> lock(_mutex);

        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        std::ostream& out = (_useFile && _outputFile.is_open()) ? _outputFile : std::cout;
        out << "[" << currentDateTime() << "] [" << getLogLevelString(level) << "] " << buffer << std::endl;
    }

private:
    Logger() :logLevel(LogLevel::INFO),  _useFile(false) {}
    Logger(LogLevel level) : logLevel(level), _useFile(false) {}
    Logger(LogLevel level, const std::string& filename) : logLevel(level) {
        setOutputFile(filename);
    }

    // 获取日志级别字符串
    std::string getLogLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }

    // 获取当前日期和时间的字符串表示
    std::string currentDateTime() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::ostringstream oss;
        oss << (localTime->tm_year + 1900) << "-"
            << (localTime->tm_mon + 1) << "-"
            << localTime->tm_mday << " "
            << localTime->tm_hour << ":"
            << localTime->tm_min << ":"
            << localTime->tm_sec;
        return oss.str();
    }

    bool _useFile;
    
    LogLevel logLevel;
    LogLevel currentLevel;
    std::ofstream _outputFile;
    std::mutex _mutex;
};

#endif // LOGGER_HPP
