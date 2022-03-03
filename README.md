# LilyGo EPD47 Image Receive And Display

This is a project to display image sent from Android phone on LilyGo 4.7inch e-paper display.

It consists of ;
1. Arduino project which makes the EPD47 device connect to a predefined (in settings.h) wifi ssid and listen for commands on a RESt server (http). This code expects several http post commands to receive 16 tone grayscale images divided to 100x100 parts.
2. Android application which allows the user select any image (in any format) from their phone/tablet, scale & convert it to 16 tone grayscale image parts and send it to the device with http post commands.

You need a LilyGo EPD47 (4.7 inch e-paper display) and an Android device (Android 10 or above) to be able to use this project.
Please see the instructions below for installation and use of both parts. 

If you have any suggestions to make this project better, please feel free to send it to me. You can also contribute to the project sending pull requests or creating a fork.

![ic_launcher_foreground](https://user-images.githubusercontent.com/51407106/156646483-9fe392ed-b12e-408f-8809-478c994db583.png)

# 1. Arduino server application

<h3 align = "left">Prerequisites for compiling and installing on your LilyGO EPD 47 device:</h3>

- Install the [Arduino IDE](https://www.arduino.cc/en/Main/Software). Note: Later instructions may not work if you use Arduino via Flatpak.
- Install the latest [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32) package in the IDE, version >= 1.05
- Download a zipfile from [this github project](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) using the "Download ZIP" button and install it using the IDE ("Sketch" -> "Include Library" -> "Add .ZIP Library...", OR:
- Clone [this github repository](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47) into your sketchbook/libraries folder. For more info, see https://www.arduino.cc/en/Guide/Libraries
- Choose `ESP32 Dev Module` for the board
- Tools -> PSRAM -> `Enable`

<h3 align = "left">Compiling and installing for your device & environment:</h3>

- Download this project's files and open in Arduino
- Edit settings.h and change ssid, password parameters for your wifi network
- mdnsname parameter is set to "display" by default. This matches the Android application default value. Don't change this unless required 
- Connect your EPD47 device to your PC 's USB port with a USB cable
- Select the correct port and click upload from Skectch menu

If compiled successfully, the app loads on the EPD47 and then screen is cleared. 
The device then tries to connect to the wifi network set in the "settings.h" file. If successful, it displays the IP address at bottom line of the screen.
At this point it is ready to receive commands from Android application.

If the device is turned off, you have to turn it on or reset with the leftmost button over the display to make it connect to the network again.

# 2. Android client application

<h3 align = "left">Installing the app on your device:</h3>

- You need an Android phone/tablet with Android 10 or above OS
- Please install the app from [Google Play]https://play.google.com/store/apps/details?id=com.aktuna.aposter
- Connect your EPD47 device to your wifi network as described in instructions above
- Connect your phone to the same wifi network
- Run the app on your phone/tablet
- If both devices are connected to the same network, the Android app will find the EPD47 IP address from its mDNS name ("display") and show it in the IP address field
- If the IP is not detected for any reason, you can manually enter the IP address shown on EPD47 screen bottom line

<h3 align = "left">Using the app:</h3>

- There are 3 buttons on the application
- "Send Image" button lets you select an image from files on your Android device and send it to the EPD47 screen after conversion
- "Clear Image" button sends a command to the display to clear its screen
- "Power Off" button sends a command to the display to turn it off 
- After power off , screen keeps the last image displayed but does not use any power. So it is disconnected. If you want to send another image after this state, you have to power cycle the EPD47 device or reset by using the letfmost button over the display

# credits:
Thanks to my dear friend [Faik Sağlar](https://github.com/ismetfaik) for helping me on converting the image data to the binary format before sending it to the device.

