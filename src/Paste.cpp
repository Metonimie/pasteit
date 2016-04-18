//
// Created by Denis Nuțiu on 13/02/16.
//
#include "../inc/Paste.h"


Paste::Paste() : DEVELOPER_KEY(""), // IMPORTANT! ADD YOUR KEY HERE!
                 apiPasteCode(""),
                 apiPastePrivate(0),
                 apiPasteName("Untitled"),
                 apiPasteExpireDate("N"),
                 apiUserKey("null"),
                 apiPasteFormat("text") {

}

Paste::~Paste() { }

string Paste::getDeveloperKey() const {
    return Paste::DEVELOPER_KEY;
}

long Paste::developerKeySize() const {
    return Paste::DEVELOPER_KEY.size();
}
int Paste::getApiPastePrivate() const {
    return apiPastePrivate;
}


string Paste::getApiPasteCode() const {
    return apiPasteCode;
}

string Paste::getApiPasteName() const {
    return apiPasteName;
}

string Paste::getApiPasteExpireDate() const {
    return apiPasteExpireDate;
}

string Paste::getApiUserKey() const {
    return apiUserKey;
}

string Paste::getApiPasteFormat() const {
    return apiPasteFormat;
}

string Paste::postData() const {
    std::stringstream data;
    data << "api_option=paste&api_user_key=" << Paste::getApiUserKey() << "&api_paste_private="
           << Paste::getApiPastePrivate() << "&api_paste_name=" << Paste::getApiPasteName()
           << "&api_paste_expire_date=" << Paste::getApiPasteExpireDate()
           << "&api_paste_format=" << Paste::getApiPasteFormat() << "&api_dev_key=" << Paste::DEVELOPER_KEY
           << "&api_paste_code=" << Paste::getApiPasteCode();
    return data.str();
}

unsigned long Paste::postSize() const {
    return Paste::postData().size();
}

bool Paste::checkPasteFormat(const string &value) const {
    std::array<std::string, 246> allowed_values = {"4cs", "6502acme", "6502kickass","6502tasm","abap","actionscript","actionscript3","ada",
            "aimms","algol68","apache","applescript","apt_sources","arm","asm","asp","asymptote","autoconf","autohotkey",
            "autoit","avisynth","awk","bascomavr","bash","basic4gl","dos","bibtex","blitzbasic","b3d","bmx","bnf","boo",
            "bf","c","c_winapi","c_mac","cil","csharp","cpp","cpp-winapi","cpp-qt","c_loadrunner","caddcl","cadlisp",
            "cfdg","chaiscript","chapel","clojure","klonec","klonecpp","cmake","cobol","coffeescript","cfm","css",
            "cuesheet","d","dart","dcl","dcpu16","dcs","delphi","oxygene","diff","div","dot","e","ezt","ecmascript",
            "eiffel","email","epc","erlang","fsharp","falcon","fo","f1","fortran","freebasic","freeswitch","gambas",
            "gml","gdb","genero","genie","gettext","go","groovy","gwbasic","haskell","haxe","hicest","hq9plus",
            "html4strict","html5","icon","idl","ini","inno","intercal","io","ispfpanel","j","java","java5","javascript",
            "jcl","jquery","json","julia","kixtart","latex","ldif","lb","lsl2","lisp","llvm","locobasic","logtalk","lolcode",
            "lotusformulas","lotusscript","lscript","lua","m68k","magiksf","make","mapbasic","matlab","mirc","mmix","modula2",
            "modula3","68000devpac","mpasm","mxml","mysql","nagios","netrexx","newlisp","nginx","nimrod","text","nsis",
            "oberon2","objeck","objc","ocaml-brief","ocaml","octave","pf","glsl","oobas","oracle11","oracle8","oz",
            "parasail","parigp","pascal","pawn","pcre","per","perl","perl6","php","php-brief","pic16","pike",
            "pixelbender","plsql","postgresql","postscript","povray","powershell","powerbuilder","proftpd","progress",
            "prolog","properties","providex","puppet","purebasic","pycon","python","pys60","q","qbasic","qml","rsplus",
            "racket","rails","rbs","rebol","reg","rexx","robots","rpmspec","ruby","gnuplot","rust","sas","scala","scheme",
            "scilab","scl","sdlbasic","smalltalk","smarty","spark","sparql","sqf","sql","standardml","stonescript","sclang",
            "swift","systemverilog","tsql","tcl","teraterm","thinbasic","typoscript","unicon","uscript","ups","urbi",
            "vala","vbnet","vbscript","vedit","verilog","vhdl","vim","visualprolog","vb","visualfoxpro","whitespace",
            "whois","winbatch","xbasic","xml","xorg_conf","xpp","yaml","z80","zxbasic"};

    for (std::array<std::string, 246>::iterator i = allowed_values.begin(); i != allowed_values.end(); i++) {
        if ( *i == value) {
            return true;
        }
    }
    return false;
}

bool Paste::checkPasteExpireDate(const string &value) const {
    std::array<std::string, 7> allowed_values = {"N", "10M", "1H", "1D", "1W", "2W", "1M"};
    for (std::array<std::string, 7>::iterator i = allowed_values.begin(); i != allowed_values.end(); i++) {
        if ( *i == value) {
            return true;
        }
    }
    return false;
}

void Paste::setApiPastePrivate(int value) {
    if (value >= 0 && value < 3) {
        Paste::apiPastePrivate = value;
    } else {
        throw std::out_of_range("Incorrect value! Value must be an in range of 0-2. 0-public, 1-unlisted, 2-private");
    }
}

void Paste::setApiPasteCode(const string &value) {
    Paste::apiPasteCode = value;
}

void Paste::setApiPasteName(const string &value) {
    Paste::apiPasteName = value;
}

void Paste::setApiPasteExpireDate(const string &value) {
    if (Paste::checkPasteExpireDate(value)) {
        Paste::apiPasteExpireDate = value;
    } else {
        throw std::invalid_argument("Incorrect date!");
    }

}

void Paste::setApiUserKey(const string &value) {
    Paste::apiUserKey = value;
}

void Paste::setApiPasteFormat(const string &value) {
    if (Paste::checkPasteFormat(value)) {
        Paste::apiPasteFormat = value;
    } else {
        throw std::invalid_argument("Incorrect code format specified!");
    }
}
