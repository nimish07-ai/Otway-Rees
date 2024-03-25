#include <iostream>
#include <vector>
#include <string>
#include <ctime>

// Define a struct to hold log entry information
struct LogEntry {
    std::string timestamp;
    std::string errorLevel;
    std::string message;
};

// Function to get current timestamp
std::string getCurrentTimestamp() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    return std::string(buffer);
}

// Function to log a message with error level
void logMessage(const std::string& message, const std::string& errorLevel, std::vector<LogEntry>& log) {
    LogEntry entry;
    entry.timestamp = getCurrentTimestamp();
    entry.errorLevel = errorLevel;
    entry.message = message;
    log.push_back(entry); 
}
