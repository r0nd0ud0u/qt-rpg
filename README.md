# qt-rpg

The goal of this game is to add a higher dimension to a classic narative RPG with deeper atks, equipments and stats.

You can create your own characters, equipments.

That software is based on C++, CMake and Qt framework(5 or 6).

## My configuration
Qt : 5.15.2 or 6.6.1
Compilators : 
	- mingw (Release or debug)
	- msvc 2019 (only for release. still a problem with linking in debug)

A lib in Rust ![lib-rpg](https://github.com/r0nd0ud0u/lib-rpg) is loaded and will step by step be the core of the game.


## Host page

![image](https://github.com/r0nd0ud0u/qt-rpg/assets/57643172/a046602a-88f3-4377-a10f-c3be570ae11f)

First, you have to choose your characters (bosses and heroes)

## Choose your game characters

By left-clicking once on a panel of a character, you can select a character and twice you unselect.
![image](https://github.com/r0nd0ud0u/qt-rpg/assets/57643172/9f03849d-b19d-4d0b-b5c3-ce49d5a0a708)

After selecting, click on the "Retour" button at the top of the window.

## Start the game

Then the button "Start Game" is enabled and you can click on it to start the game.

![image](https://github.com/r0nd0ud0u/qt-rpg/assets/57643172/56f0da82-0c15-4895-a596-addee30f1bd7)

## Basic commands
- To start a turn, click on the button "Prochain tour"
- By toggling atk/bag buttons <your actions could be :
  - one atk
  - use as much potions as you want in the bag
When your actions are done, click on "Fin du round" button.
