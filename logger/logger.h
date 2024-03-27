#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>

// Define a struct to hold log entry information
struct LogEntry {
    std::string timestamp;
    std::string errorLevel;
    std::string message;
};

// Function to get current timestamp
std::string getCurrentTimestamp();

// Function to log a message with error level
void logMessage(const std::string& message, const std::string& errorLevel, std::vector<LogEntry>& log);

#endif // LOGGER_H
