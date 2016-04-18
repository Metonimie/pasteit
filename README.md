# PasteIT #

Command line application for Mac OX that permits the upload of files to PasteBin.com.

This is an educational program, it isn't supposed to be used as professional software!

I made it in order to practice my cpp.

### Information ###

* Supported OS: OS X
* Version: 1.5

### How to use? ###

pasteit filename

* That's it.
* Type pasteit -help for more options.

## Compiling ##

1. Add your pastebin developer key in the ./src/Paste.cpp constructor
2. Compile the program

## Instalatation ##

1. Open up a new terminal window.
2. Type: mkdir -p ~/.pasteit
3. Change directory to the folder you downloaded the program. Eg: cd Downloads
4. Type: mv pasteit ~/.pasteit/pasteit
5. Open ~/.bashrc or ~/.zshrc and append: "alias pasteit=~/.pasteit/pasteit", without quotes.
6. Restart the terminal window. You're done!

## Uninstall ##

1. Open up a new terminal window.
2. BE CAREFULL THAT YOU TYPE THIS CORRECTLY!
3. Type: rm -r ~/.pasteit
4. Open the file .bashrc or .zshrc and delete the alias.
5. You've uninstalled the program!

### Changelist ###

* Improved argument handling.
* Created the program.
