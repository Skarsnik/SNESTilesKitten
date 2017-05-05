---
layout: default
title: SNESTilesKitten
---

Welcome on the SNESTilesKitten page

SNESTilesKitten is a tool to view/extract/inject graphic tiles in a SNES Rom file. It allows you to use SNES address (support HiROM/LoROM) so you can use it to discover tile location directly from ASM trace.

![SNESTilesKitten preview](https://skarsnik.github.io/SNESTilesKitten/webimage/stk.png)


You can create tiles preset with it so you can reuse your discoveries or share them with other user of SNESTilesKitten. Some presets are provided with it.

# Download

You can use the top button to get the source code of SNESTilesKitten. It only have Windows release for now.

To get instructions on how to compile the tool, see the [github project page](https://github.com/Skarsnik/SNESTilesKitten/)

# Tutorial

You can find a basic tutorial on the [tutorial page](https://skarsnik.github.io/SNESTilesKitten/Tutorial)


# Command line

SNESTilesKitten offer a command line tool : SNESTinyKitten. Just run the executable with the --help command to learn more.

Here an example of injecting a set of tiles.

```
D:\Project\SNESTilesKitten>SNESTinyKitten.exe --inject --copy --rom "D:\Project\Zelda - A Link to the Past.smc" --input "D:\Project\The Legend of Zelda - Link Sprites derppy.png" "D:\Project\SNESTilesKitten\Presets\The Legend of Zelda - Link Sprites.stk"
ROM has SMC header
ROM type is LoROM
ROM Tile : THE LEGEND OF ZELDA
Doing a copy of D:/Project/Zelda - A Link to the Past.smc to D:/Project/Zelda - A Link to the Past_copy.smc
Injecting data
Number of tile extracted from the PNG : 896
28672  bytes of data writen to the ROM
```
