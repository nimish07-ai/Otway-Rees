#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <string>
#include <unistd.h> // for POSIX API
#include <sys/ioctl.h> // for ioctl system call
#include <csignal> // for signal handling
#include <sstream>
using namespace std;

atomic<bool> running{true};
mutex notificationMutex;


struct DisplayText {
    string title;
    string notification;
    string error;
    string header;
    string body;
    string footer;
};
// DisplayText myDisplayText;

// Function to get the terminal window width
int getWindowWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

// Get the height of the terminal window
int getWindowHeight() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_row;
}

// Count the number of lines in a string
int countLines(const std::string& text) {
    std::istringstream iss(text);
    int count = 0;
    std::string line;
    while (std::getline(iss, line)) {
        count++;
    }
    return count;
}


// Function to refresh the screen
void refreshScreen(const DisplayText& displayText) {
    // Clear the screen
    system("clear");

    // Calculate the indentation needed for centering the title
    int windowWidth = getWindowWidth();
    int indentation = (windowWidth - displayText.title.length()) / 2;

    // Calculate the indentation for the notification text
    int notificationIndentation = windowWidth - displayText.notification.length() - 4; // Adjust for space

    // Print the title centered on the screen
    cout << string(1, '\n'); // Move cursor to the middle of the screen
    cout << string(indentation, ' ') << displayText.title << endl;

    // Print the notification aligned to the extreme right
    cout << string(notificationIndentation, ' ') << "\u2709\ufe0f " << displayText.notification << endl;

    // Print the header
    cout << "\n" << displayText.header << endl;

    // Print the body
    cout << "\n " << displayText.body << endl;

    // Print the footer
    // cout << "\n " << displayText.footer << endl;
}



void welcomeFunction() {
    // Clear the screen
    system("clear");

    string asciiArt = R"(
       .aMMMb dMMMMMMP dMP dMP dMP .aMMMb  dMP dMP 
      dMP"dMP   dMP   dMP dMP dMP dMP"dMP dMP.dMP  
     dMP dMP   dMP   dMP dMP dMP dMMMMMP  VMMMMP   
    dMP.aMP   dMP   dMP.dMP.dMP dMP dMP dA .dMP    
    VMMMP"   dMP    VMMMPVMMP" dMP dMP  VMMMP"     
                                                   
        dMMMMb  dMMMMMP dMMMMMP .dMMMb             
       dMP.dMP dMP     dMP     dMP" VP             
      dMMMMK" dMMMP   dMMMP    VMMMb               
     dMP"AMF dMP     dMP     dP .dMP               
    dMP dMP dMMMMMP dMMMMMP  VMMMP"                
    )";    

    // Print the ASCII art line by line with a short delay
    for (char c : asciiArt) {
        // Print the character
        cout << c << flush;

        // Sleep for a short duration to simulate animation
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // Wait for 3-4 seconds
    this_thread::sleep_for(chrono::seconds(3));
}
void handleResize(int signal) {
    refreshScreen({"Title", "Notification", "Header", "Body", "Footer"});
}


void startup()
{
    welcomeFunction();
    signal(SIGWINCH, handleResize);

}




// int main() {
//     // Call the startup function to display the welcome message
//     startup();

//     // Initialize text content using the structure
//     DisplayText myDisplayText;
//     myDisplayText.title = "Otway-Rees";
//     myDisplayText.notification = "10";
//     myDisplayText.header = "Header: This is the header";
//     myDisplayText.body = "Body: This is the body. Line 1.\nLine 2.\nLine 3.\nLine 4.\nLine 5.\nLine 6.\nLine 7.\nLine 8.\nLine 9.";
//     myDisplayText.footer = "Footer: This is the footer";

//     // Main loop to refresh the screen
//     while (running) {
//         // Call the refreshScreen function to update the display
//         refreshScreen(myDisplayText);

//         // Wait for user input before exiting
//         cout << "Press Enter to exit...";
//         cin.get();
//         running = false;
//     }

//     return 0;
// }
