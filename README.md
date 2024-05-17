# Tetris_Flipdot

The goal of this repository was to create a working Tetris that runs on Arduino UNO R3 SMD and sends correct data packets to a external flip-dot display. This has been achived since the code is able to send data to an external flip-dot display, where the Tetris state of game is reflected correctly. The code is able to read inputs from a cheap USB controller and with these inputs the game can be controlled accordingly.

During the development process I learned alot about bit manipulation what was used to optimize the code. Optimizations were necessary considering the small amount of SRAM the microcontroller provided. Nevertheless the project was a fun and interesting challenge that needed alot of data visualization and understanding memory managment of Arduino Uno. Finally since the project can still be optimized in different areas I choose to not do so due to time constraints and because the system is already functional.

The project was created for my bachelor thesis.