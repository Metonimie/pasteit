//
// Created by Denis Nuțiu on 16/02/16.
//

#ifndef PASTE_APPLICATION_H
#define PASTE_APPLICATION_H

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <curl/curl.h>
#include <string.h>
#include "Paste.h"

using std::string;
using std::fstream;
using std::istream;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

class Application {
    Paste paste;
    CURL *curl;
    int argc;
    int input_index;
    char ** argv;
    string homePath;
    int getArgumentSize() const;
    char * getArgument();
    void authenticateUser();
    void writeUserKey(char * key, size_t size);
    void prepareFileUpload();
    void getUserKey();
    void uploadPaste();
    void deleteFile();
    void processInput();
    void displayHistory();
    void displayHelpInformation();
    void displayVersionInformation();
public:
    Application(int argc, char ** argv);
    ~Application();
};


#endif //PASTE_APPLICATION_H
