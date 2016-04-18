//
// Created by Denis Nuțiu on 16/02/16.
//

#include "../inc/Application.h"

const string VERSION = "1.5";

// credits: Alexandre Jasmin
struct curl_string {
    char *ptr;
    size_t len;
};

// Custom tab manipulator for cout << tab;
std::ostream & tab(std::ostream & out) {
    out << "\t";
    return out;
}

void init_string(struct curl_string *s) {
    s->len = 0;
    s->ptr = (char *)malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct curl_string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = (char *)realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}
// end credits;

Application::Application(int argc, char **argv) : argc(argc), argv(argv), input_index(0) {
    if (argc > 1) {
    #ifdef __APPLE__
        Application::homePath = getenv("HOME");
        homePath.append("/.pasteit/");
    #endif
        Application::authenticateUser();
        Application::processInput();
    } else {
        cerr << "Invalid usage!" << endl;
        cerr << "Type: pasteit --help for instructions!" << endl;
    }
}

void Application::authenticateUser() {
    std::ifstream file(Application::homePath + "keypi");
    std::stringstream stream;
    if (file.is_open() ) {
        stream << file.rdbuf();
        Application::paste.setApiUserKey(stream.str());
    }
}

void Application::processInput() {
    if (Application::input_index < Application::argc - 1) {
        Application::input_index++;
    #ifdef DEBUG
        cout << "input_index: " << Application::input_index << endl;
        cout << "argc: " << Application::argc << endl;
    #endif
    }

    if ( !strcmp(argv[Application::input_index], "--help") || !strcmp(argv[Application::input_index], "--h") ) {
        Application::displayHelpInformation();
    } else if ( !strcmp(argv[Application::input_index], "--user") || !strcmp(argv[Application::input_index], "--u") ) {
        if ( Application::argc < 4) {
            cerr << "Invalid number of arguments!" << endl;
        } else {
            Application::getUserKey();
        }
    } else if ( !strcmp(argv[Application::input_index], "--version") ) {
        Application::displayVersionInformation();
    } else if ( !strcmp(argv[Application::input_index], "--history") ) {
        Application::displayHistory();
    } else if ( !strcmp(argv[Application::input_index], "--del") || !strcmp(argv[Application::input_index], "--d") ) {
        if ( Application::argc < 3) {
            cerr << "Invalid number of arguments!" << endl;
        } else {
            Application::deleteFile();
        }
    } else if (input_index == argc - 1) {
        Application::prepareFileUpload();
    } else if ( strstr(argv[input_index], "-name=") || strstr(argv[input_index], "-n=") ) {
        char * cleanName = curl_easy_escape(Application::curl, Application::getArgument(),
                                            Application::getArgumentSize());
        Application::paste.setApiPasteName( cleanName );
        Application::processInput();
    } else if ( strstr(argv[input_index], "-public=") || strstr(argv[input_index], "-p=") ) {
        Application::paste.setApiPastePrivate( atoi(Application::getArgument()) );
        Application::processInput();
    } else if ( strstr(argv[input_index], "-expire=") || strstr(argv[input_index], "-e=") ) {
        try {
            Application::paste.setApiPasteExpireDate( Application::getArgument() );
            Application::processInput();
        } catch (std::invalid_argument ex) {
            cerr << ex.what() << endl;
        }
    } else if ( strstr(argv[input_index], "-format=") || strstr(argv[input_index], "-f=") ) {
        try {
            Application::paste.setApiPasteFormat( Application::getArgument() );
            Application::processInput();
        } catch (std::invalid_argument ex) {
            cerr << ex.what() << endl;
        }
    } else {
    #ifdef DEBUG
        cout << "Request send from the last block." << endl;
    #endif
        Application::prepareFileUpload();
    }
}

void Application::displayHelpInformation() {
    cout << "Usage: " << endl;
    cout << "pasteit [--option] | [-mods] filename" << endl;
    cout << endl;
    cout << tab << "Options:" << endl;
    cout << tab << tab << "--help | --h - Displays help information" << endl;
    cout << tab << tab << "--version - Displays version information" << endl;
    cout << tab << tab << "--history - Displays the paste history" << endl;
    cout << tab << tab << "--user | --u USERNAME PASSWORD - Setup a pastebin account." << endl;
    cout << tab << tab << "--del | --d [ history | acc | all ] - Deletes the paste history or account." << endl;
    cout << endl;
    cout << tab << "Mods:" << endl;
    cout << tab << "Eg: pasteit name=Info2015 public=1 file.txt" << endl;
    cout << tab << tab << "-name | -n- Set's the paste name to something." << endl;
    cout << tab << tab << "-public | -p - Changes the paste visibility. 0 - public, 1 - unlisted, 2 - private." << endl;
    cout << tab << tab << "-expire | -e - Changes the paste expiration date." << endl;
    cout << tab << tab << tab << "Allowed values: N, 10M, 1H, 1D, 1W, 2W, 1M" << endl;
    cout << tab << tab << "-format | -f - Changes the code syntax." << endl;
    cout << tab << tab << tab << "Allowed values: https://pastebin.com/api#5" << endl;
}

void Application::displayHistory() {
    std::ifstream history;
    string line;
    history.open(Application::homePath + "historypi");
    if ( history.is_open() ) {
        cout << "Latest pastes: " << endl;
        while ( getline(history, line) ) {
            cout << tab << line << endl;
        }
    } else {
        cerr << "Error opening history file." << endl;
    }
}

char * Application::getArgument(){
    char * pos = strstr(argv[input_index], "=");
    pos++;
    return pos;
}

int Application::getArgumentSize() const{
    char * pos = strstr(argv[input_index], "=");
    int size = 0;
    while(pos[size]) { size++; }
    return size;
}

void Application::deleteFile() {
    if ( !strncmp(Application::argv[2], "history", 7) ) {
        string tmp = Application::homePath + "historypi";
        std::remove(tmp.c_str());
    } else if (!strncmp(Application::argv[2], "acc", 3)) {
        string tmp = Application::homePath + "keypi";
        std::remove(tmp.c_str());
    } else if (!strncmp(Application::argv[2], "all", 3)) {
        string tmp1 = Application::homePath + "historypi";
        string tmp2 = Application::homePath + "keypi";
        std::remove(tmp1.c_str());
        std::remove(tmp2.c_str());
    } else {
        cerr << "Invalid argument!" << endl;
    }
}

void Application::displayVersionInformation() {
    cout << "PasteIT version" << VERSION << " by Metonymy" << endl;
}

void Application::getUserKey() {
    // dev key size, option size, username size, pass size, null size
    long usernameSize = strlen(argv[2]);
    long passwordSize = strlen(argv[3]);

    char data[Application::paste.developerKeySize() + 69 + 1 + usernameSize + passwordSize];
    strcpy(data, "api_option=userdetails&api_dev_key=");
    strcpy(data + 35, Application::paste.getDeveloperKey().c_str());
    strcpy(data + 35 + Application::paste.developerKeySize(), "&api_user_name=");
    strcpy(data + 35 + Application::paste.developerKeySize() + 15, argv[2]);
    strcpy(data + 35 + Application::paste.developerKeySize() + 15 + usernameSize, "&api_user_password=");
    strcpy(data + 35 + Application::paste.developerKeySize() + 15 + usernameSize + 19, argv[3]);

    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a Application::curl handle */
    Application::curl = curl_easy_init();
    if(Application::curl) {
        /* We init our curl struct and then we add it to the curl functions. */
        struct curl_string s;
        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(Application::curl, CURLOPT_POST, true);
        curl_easy_setopt(Application::curl, CURLOPT_URL, "http://pastebin.com/api/api_login.php");
        /* Now specify the POST data */
        curl_easy_setopt(Application::curl, CURLOPT_POSTFIELDS, data);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(Application::curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(Application::curl);
        #ifdef DEBUG
                cout << data << endl;
                cout << "Login successful! User key set!" << endl;
        #endif
        if ( strstr(s.ptr, "Bad") ) {
            cerr << s.ptr << endl;
        } else {
            Application::writeUserKey(s.ptr, s.len);
        }
        free(s.ptr);
    }

    curl_global_cleanup();
}

void Application::writeUserKey(char * key, size_t size) {
    std::ofstream file;
    file.open(Application::homePath + "keypi");
    file.write(key, size);
    cout << "User key set!" << endl;
}

void Application::prepareFileUpload() {
    std::ifstream file(argv[Application::input_index]);
    stringstream stream;
    if (file.is_open()) {
        stream << file.rdbuf();
        Application::paste.setApiPasteCode(stream.str());
    #ifndef DEBUG
        Application::uploadPaste();
    #else
        cout << endl;
        cout << "Request Info:" << endl;
        cout << endl;
        cout << Application::argv[Application::input_index] << endl;
        cout << Application::paste.getApiPasteName() << endl;
        cout << Application::paste.getApiPasteExpireDate() << endl;
        cout << Application::paste.getApiPastePrivate() << endl;
        cout << Application::paste.getApiPasteFormat() << endl;
        cout << Application::paste.getApiUserKey() << endl;
    #endif
    } else {
        cerr << "Error opening " << argv[Application::input_index] << endl;
    }
}

void Application::uploadPaste() {
    Application::curl = curl_easy_init();

    //Encode the code
    char * uncleanCode = (char *) malloc(Application::paste.getApiPasteCode().size());
    strcpy(uncleanCode, Application::paste.getApiPasteCode().c_str());
    char * cleanCode = curl_easy_escape(curl, uncleanCode, static_cast<int>(Application::paste.getApiPasteCode().size()));

    Application::paste.setApiPasteCode(cleanCode);
    free(uncleanCode);
    free(cleanCode);

    const unsigned long postSize = Application::paste.postSize();
    char data[postSize];
    strncpy(data, Application::paste.postData().c_str(), postSize);
    data[postSize] = '\0';

    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    if(Application::curl) {
        struct curl_string s;
        std::ofstream history;
        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(Application::curl, CURLOPT_POST, true);
        curl_easy_setopt(Application::curl, CURLOPT_URL, "http://pastebin.com/api/api_post.php");
        /* Now specify the POST data */
        curl_easy_setopt(Application::curl, CURLOPT_POSTFIELDS, data);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(Application::curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        if ( strstr(s.ptr, "http://") ) { // Check if we got a valid url.
            cout << "URL: " << s.ptr << endl;
            // add paste to history.
            history.open(Application::homePath + "historypi", std::ios::app);
            s.ptr = (char *) realloc(s.ptr, s.len + 1);
            if (!s.ptr) { cerr << "Not enough memory!" << endl; }
            s.ptr[s.len++] = '\n'; // We need a newline after the url.
            s.ptr[s.len] = '\0';
            if (history.is_open()) {
                history.write(s.ptr, s.len);
            }
            history.close(); // not necessary
        } else {
            cerr << s.ptr << endl;
        }
        free(s.ptr);
        /* always cleanup */
        curl_easy_cleanup(Application::curl);
    }
    curl_global_cleanup();
}

Application::~Application() { }
