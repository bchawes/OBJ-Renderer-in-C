\*# OBJ-Renderer-in-C
  /$$$$$$  /$$$$$$$     /$$$$$       /$$$$$$$                            /$$                    
 /$$__  $$| $$__  $$   |__  $$      | $$__  $$                          | $$                    
| $$  \ $$| $$  \ $$      | $$      | $$  \ $$  /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$ 
| $$  | $$| $$$$$$$       | $$      | $$$$$$$/ /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$
| $$  | $$| $$__  $$ /$$  | $$      | $$__  $$| $$$$$$$$| $$  \ $$| $$  | $$| $$$$$$$$| $$  \__/
| $$  | $$| $$  \ $$| $$  | $$      | $$  \ $$| $$_____/| $$  | $$| $$  | $$| $$_____/| $$      
|  $$$$$$/| $$$$$$$/|  $$$$$$/      | $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$| $$      
 \______/ |_______/  \______/       |__/  |__/ \_______/|__/  |__/ \_______/ \_______/|__/      
                                                                                                
(A "quick" test of CPU based rendering in C!)
------------------------------------------------------------------------------------------------
Created By Bryce Hawes, In July and September 2025
(I Forgot about the project for a little while!)

Libraries Used:
<SDL2/SDL.h>, <stdio.h>, <tgmath.h>

Titles obtained from https://patorjk.com/software/taag/

AI Disclaimer:
Some code was debugged using GitHub Co-Pilot. Line-drawing function calculations and functions
involving the creation and destruction of the window were partially generated w/ GitHub Co-Pilot.
Formulas for rotation of model were obtained from ChatGPT, and cross-referenced with research on
Google and Stack Exchange (Math and Overflow).

Utilizing AI as a programming tool for debugging and performance improvement purposes is a major 
benefit to both learning and improving as a coder. I believe the ethical use and proper 
documentation of the use of this technology is essential to establish a proper relationship
between programmers and AI. 

------------------------------------------------------------------------------------------------
  ___           _                   _   _                 
 |_ _|_ __  ___| |_ _ __ _   _  ___| |_(_) ___  _ __  ___ 
  | || '_ \/ __| __| '__| | | |/ __| __| |/ _ \| '_ \/ __|
  | || | | \__ \ |_| |  | |_| | (__| |_| | (_) | | | \__ \
 |___|_| |_|___/\__|_|   \__,_|\___|\__|_|\___/|_| |_|___/
------------------------------------------------------------------------------------------------
|Rotate:                              Right Click + Drag
|Move:                                Left Click + Drag
|Zoom In/Out:                         Scroll
|Change Brightness/RenderDistance:    -/+
|Increase/Decrease Line Thickness:    W/S
------------------------------------------------------------------------------------------------
Default model (for genuinely no reason) is Hatsune Miku! To Swap this out, find an OBJ file and
copy it's values to the miku.obj file in the cmake-build-debug!\*
