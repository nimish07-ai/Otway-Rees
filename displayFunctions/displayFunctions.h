#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
#include <csignal>
#include <sstream>

using namespace std;

const string ANSI_COLOR_RED = "\x1b[31m";
const string ANSI_COLOR_RESET = "\x1b[0m";

struct DisplayText {
    string title;
    string notification;
    string error;
    string header;
    string body;
    string footer;
};


int getWindowWidth();
void refreshScreen(const DisplayText& displayText);
void handleResize(int signal);
void welcomeFunction();
void startup();

#endif 
