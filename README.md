# WirelessMouse

This project's goal is to provide a way to control my PC mouse from my phone.

## What is it

### Desktop app
The desktop app is made in C# using .NET 6 and Windows forms for a tray icon and popups. So currently is only a Windows only application.
Runs a application that displays in the noticaiation try to act as a UDP server and recive and act upon packets sent from the mobile app. This has persistant storage of the password used for encoding

### Mobile Interface
The mobile interface is made in C# using .NET MAUI and is compatible with Android, iOS, Windows and macOS.
This compiles and deploys into a mobile (or otherwise) application which allows a password and server to be set and persistantly stored.
The application acts as trackpad and has single and double click functionality as well as the obvious dragging (like a trackpad).

## Future stuff
I plan on adding:
- Keyboard passthrough (using native phone keyboards)
- Click and drag functionality without a toggle button
- USB mounted mouse device (winapi is a bit iffy in some cases)
