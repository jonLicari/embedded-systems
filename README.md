# Embedded Systems Collection
Embedded systems projects developed on ARM Cortex M3 covering the basics of embedded software development, embedded system design, and real-time operating systems and scheduling techniques. 
Embedded hardware-software design and development tools (e.g. uVision, Quartus II and SOPC builder) are used throughout.

## Project 1: Blinky
Introduction to Keil uVision IDE and ARM Cortex M3 architecture. 
Execution of code on NXP LPC1768 microcontroller and MCB1700 development board.

The project will be created to flash LEDs at a speed based on the voltage produced by the microcontroller's ADC.
Peripheral access via GPIO is achieved by bit-banding, masking, and direct memory access.

## Project 2: LED Bit-band
Introduction to bit-banding, conditional branching, and barrelshifting with the ARM Cortex M3 architecture.
Performance efficiency will be analyzed through debugging and target mode.

LEDs are lit up using bit-banding, masking, and function techniques.
Application of barrelshifting will be implemented.

## Project 3: Scheduling Tasks with uVision and RTX
Introduction to uVsion and ARM Cortex M3's various task-based RTX based RTOS capabilities.
Round-robin, preemptive, and non-preemptive scheduling techniques will be implemented.

## Project 4: Scheduling Multithreaded Applications with RTX & uVision
Introduction to RTX based multithreaded applications using uVision and ARM Cortex M3.
RTX and CMSIS libraries will be utilized to schedule round-robin, priority preemptive, and non-preemptive multithreaded applications.

## Project 5: Real-Time Scheduling
A type of Fixed Priority Scheduling (FPS), Rate Monotonic Scheduling (RMS), is implemented as a solution to priority inversion.
Virtual timers, inter-thread communication, static and dynamic priority inversion concepts are explored.

## Project 6: Media Center
A media center was created and deployed on the MCB1700 board featuring:
- a photo gallery to display .bmp files
- an mp3 player that plays streaming audio/ mp3 tracks from the PC
- a game center offering Connect Four
