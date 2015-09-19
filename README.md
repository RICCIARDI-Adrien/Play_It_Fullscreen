# Morrowind_Fullscreen
Allow to run Morrowind in fullscreen on Windows 10.  
On Windows 10, running Morrowind in fullscreen mode will show a window border and a part of the window won't be visible as it is out of the screen bounds.  
This little program will remove the Morrowind window border and put the window in fullscreen mode.

## Compilation
Use Cygwin to compile the program.  
This is a console program, so you can run it from the Cygwin console.  
The program has been tested on Windows 10 Professionnal N 64-bit.

## Usage
Start the program before launching Morrowind.  
When Morrowind is detected, the program will put it in fullscreen and exit.  
**Warning :** the program detects the Morrowind window by searching for a window titled "Morrowind". Thus, the first window called this way will be put in fullscreen, so be sure that only the Morrowind window is called "Morrowind" when starting the program...
