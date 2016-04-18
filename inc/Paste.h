//
// Created by Denis Nuțiu on 13/02/16.
//

#ifndef PASTE_PASTE_H
#define PASTE_PASTE_H

#include <iostream>
#include <sstream>
#include <array>

using std::string;

class Paste {
    const string DEVELOPER_KEY;
    string apiPasteCode; // paste content, url-encode
    int apiPastePrivate; // 0-public 1-unlisted, 2-private
    string apiPasteName; // url-encode.
    string apiPasteExpireDate; // 10M 1H
    string apiUserKey; // if null paste will be created as Guest.
    string apiPasteFormat; // syntax
    bool checkPasteExpireDate(const string &value) const;
    bool checkPasteFormat(const string &value) const;
public:
    Paste();
    ~Paste();
    int getApiPastePrivate() const;
    long developerKeySize() const;
    string getDeveloperKey() const;
    string getApiPasteCode() const;
    string getApiPasteName() const;
    string getApiPasteExpireDate() const;
    string getApiUserKey() const;
    string getApiPasteFormat() const;
    string postData() const;
    unsigned long postSize() const;
    void setApiPastePrivate(int value = 2);
    void setApiPasteCode(const string &value);
    void setApiPasteName(const string &value = "Untitled");
    void setApiPasteExpireDate(const string &value = "N");
    void setApiUserKey(const string &value);
    void setApiPasteFormat(const string &value = "text");
};

#endif //PASTE_PASTE_H
