# Python UI For Europa Exploration MSD

## Overview

The python UI is designed to be an interactive display for the rover. The display consists of data plots, movement controls, and image displays. The functionality is built around the Matplotlib animation library and the project's `animation` function directs all repetitive tasks.

## Running

`C:/Python38/python.exe ./pythonUI/plotSerial.py -c COM7 `

If a com port parameter is not given, the default port is set to COM4.

## Testing

While the STM32 was not in hand and serial communication was to be tested, a serial emulator was used. Using the [com0com](https://com0com.sourceforge.net/) application, a user can tie two serial ports on their computer together. The `serialEmulate.py` script would generate and post serial messages to imitate the STM's expected responses. While not designed to handle all call-response functionality, it helped with getting the data processing running.

### Emulator layout

com0com would tie COM4 and COM5 together. `serialEmulate.py` would send data to COM4. The app would be waiting at COM5.

It helped to have putty open the app's com port first to check the raw serial transmission, before dealing with the app.

## Sensor Data

The script decomposes the comma delineated serial string into the respective sensors. While the plots are restricted to the most recent 20 points, all the data is recorded and saved into a CSV file.

## Camera Data

Camera functionality within the STM was not finished. For imagine RIT, the camera was used with an arduino and connected to an additional COM port in the receiving computer. Using the arducam library (included in the above directory), a host-app saves the photos.

When the Europa UI opens, the user is prompted to select a directory for images. This should align with where the arducam host-app saves the photos.

The Europa UI finds the most recent saved photo and updates a photo block respectively. The speed of the photo block being updates is restricted to the rate of the animation function.

Photo reconstruction is not implemented into the UI yet.

## Controls

The buttons are standard tkinter buttons with callback functions that write the respective commands to serial.

When the STM receives a command, it returns `Instruction Received` to the serial line. This has the ability to interfere with the data string decomposition.
