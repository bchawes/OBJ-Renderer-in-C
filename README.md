# OBJ-Renderer-in-C                                                                     
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
|Rotate:                              Right Click + Drag
|Move:                                Left Click + Drag
|Zoom In/Out:                         Scroll
|Change Brightness/RenderDistance:    -/+
|Increase/Decrease Line Thickness:    W/S
|Reset Model:                         R
------------------------------------------------------------------------------------------------
When opening the application (Located in Cmake-Build-Debug), the program will prompt the user
for a file directory. Input an OBJ file and it should work!
