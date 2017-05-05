SNESTilesKitten is a tool to view/extract/inject graphic tiles in a SNES Rom file. It allows you to use SNES address (support HiROM/LoROM) so you can use it to discover tile location directly from ASM trace.

If you just want to use the software look at [SNESTilesKitten webpage](https://skarsnik.github.io/SNESTilesKitten/)

This readme is written toward developpers.

# Checking out the project

Since the project use an external project, using just git clone will not be enought. You have to do these 3 steps.

`git clone https://github.com/Skarsnik/SNESTilesKitten.git`

Enter the directory and runs

`git submodule init`

`git submodule update`

# Compiling the project

If you work on Windows and Mac os X you probably want to install QtCreator aside Qt5.

Then start QtCreator and open the `SNESTilesKitten.pro` file.

On Linux you can do the same, or run `qmake SNESTilesKitten`

To create a Windows release, use the `deploystuff.bat` script on the deploy directory.  

# Working on the code

You have to be carefull when you work on the code. All the low level stuff (mainly SNES specific stuff) are located in C files in the `lowlevelstuff` directory. You should probably work on the [base project](https://github.com/Skarsnik/sneshacking/) if you need to change/correct something in there.

# Compression

If you want write a new compression plugin, just copy/paste an already existing one and adapt the code. Qt plugins can be annoying to write from scratch.

# Licencing

All the code is under the GPL3 Licence