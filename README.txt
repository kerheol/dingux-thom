
    Welcome to Dingux-THOM

Original Authors of THOM

  Sylvain Huet    (see http://www.sylvain-huet.com)
  Eric Botcazou   (see http://nostalgies.thomsonistes.org/)

Author of the PSP, Gp2x, Wiz and Dingoo port version 

  Ludovic.Jacomme also known as Zx-81 (see http://zx81.zx81.free.fr)

1. INTRODUCTION
   ------------

  Thom is one of the best emulator of the Thomson home computer running
  on MacOS, Windows and Unix. The emulator faithfully imitates the
  TO7-70 model (see http://nostalgies.thomsonistes.org/)

  Dingux-THOM is a port on Dingoo/Dingux of the version 1.5.5 of Thom.

  This package is under GPL Copyright, read COPYING file for
  more information about it.


2. INSTALLATION
   ------------

  Unzip the zip file, and copy the content to the root directory of your
  SD memory.

  You need to modify dmenu configuration file to add this emulator.
  Edit the file local/dmenu/themes/Base/menu_emulators.cfg and you can
  add the following lines (for example) :
 
  MenuItem Thomson
  {
    Icon = "res/emulators.png"
    Name = " Thomson To7"
    Executable = "./dingux-thom"
    WorkDir = "/usr/local/emulators/dingux-thom"
  }

  For any comments or questions on this version, please visit 
  http://zx81.zx81.free.fr or http://www.gp32x.com/


3. CONTROL
   ------------

  In the THOM emulator window, there are three different mapping 
  (standard, left trigger, and right Trigger mappings). 
  You can toggle between while playing inside the emulator using 
  the two trigger keys.

    -------------------------------------
    Dingoo       TO7-70          (standard)
  
    A          Space
    B          ENTER
    Y          1
    X          Joystick Fire
    Up         Up
    Down       Down
    Left       Left 
    Right      Right

    -------------------------------------
    Dingoo        TO7-70   (left trigger)
  
    A          FPS
    B          Swap joystick / cursor
    Y          Load state
    X          Save state
    Up         Joystick Up
    Down       Joystick Down
    Left       Joystick Left 
    Right      Joystick Right

    -------------------------------------
    Dingoo        TO7-70   (right trigger)
  
    A          STOP 
    B          ENTER
    Y          DELETE
    X          Auto-fire
    Up         Joystick Up
    Down       Joystick Down
    Left       Dec auto-fire
    Right      Inc auto-fire
    
    Press Select  to enter in emulator main menu.
    Press Start   open/close the On-Screen keyboard

  In the main menu

    RTrigger   Reset the emulator

    Y   Go Up directory
    X   Valid
    B   Valid
    A   Go Back to the emulator window

  The On-Screen Keyboard of "Danzel" and "Jeff Chen"

    Use the stick to choose one of the 9 squares, and
    use A, B, X, Y to choose one of the 4 letters 
    of the highlighted square.

    Use LTrigger and RTrigger to see other 9 squares 
    figures.


4. LOADING TAPE FILES (K7)
   ------------

  If you want to load tape image in the virtual drive of your emulator,
  you have to put your tape file (with .zip or .k7 file extension) on your 
  Dingoo memory in the 'k7' directory. 

  Then, while inside THOM emulator, just press SELECT to enter in the emulator 
  main menu, choose "Load K7", and then using the file selector choose one tape 
  file to load in your emulator.

  To run the game of your tape file, you have to use the virtual keyboard
  (press START key) and type the TO7-70 command 'LOADM"",,R' followed by 
  ENTER (B).

  You can also use directly the shortcut in the emulator menu (Command LOADM option)

  To RUN BASIC program, you have to type the TO7-70 command 'RUN""' followed
  by ENTER (B)

  You can also use directly the shortcut in the emulator menu (Command RUN option)

  It may happen that you need to rewind the tape using the Rewind K7 menu ...

5. LOADING DISK FILES (SAP)
   ------------

  If you want to load tape image in the virtual disk drive of your emulator,
  you have to put your disk file (with .sap file extension) on your Dingoo
  in the 'disc' directory. 

  Then, while inside THOM emulator, just press SELECT to enter in the emulator 
  main menu, choose "Load Disc 0" (or Disc 1), and then using the file 
  selector choose one disc file to load in your emulator.

  Use the same commands as described in the "Load tape" section.

  If you want to specify the command to run for given games then
  you can do it in the run.txt, using the following syntax :

    tapename=LOAD"CASS:RunName",,R
    tapename=LOADM"CASS:RunName",,R
    tapename=RUN"CASS:RunName"
    diskname=LOAD"RunName",,R
    diskname=LOADM"RunName",,R
    diskname=RUN"RunName"

6. LOADING SNAPSHOT FILES (STZ)
   ------------

I've modified original THOM emulator to add a save state feature. The save
state format is specific to Dingoo-THOM, but it might be useful to run previously
loaded games (using K7 and disk menu).

   
7. LOADING KEY MAPPING FILES
   ------------

  For given games, the default keyboard mapping between Dingoo Keys and TO7-70 keys,
  is not suitable, and the game can't be played on DingooTHOM.

  To overcome the issue, you can write your own mapping file. Using notepad for
  example you can edit a file with the .kbd extension and put it in the kbd 
  directory.

  For the exact syntax of those mapping files, have a look on sample files already
  presents in the kbd directory (default.kbd etc ...).

  After writting such keyboard mapping file, you can load them using 
  the main menu inside the emulator.

  If the keyboard filename is the same as the tape file (.k7 or .sap) then 
  when you load this tape file, the corresponding keyboard file is 
  automatically loaded !

  You can now use the Keyboard menu and edit, load and save your
  keyboard mapping files inside the emulator. The Save option save the .kbd
  file in the kbd directory using the "Game Name" as filename. The game name
  is displayed on the right corner in the emulator menu.

  
8. COMPILATION
   ------------

  It has been developped under Linux using gcc with Dingux SDK. 
  To rebuild the homebrew run the Makefile in the src archive.

