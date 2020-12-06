# SerialCom
Repo for small project I'm working on to talk to serial devices through Linux

Currently supports sending data over the serial port from ASCII, hexadecimal, or files.  Also can display return data as any of these three forms.  If you want to use this for whatever reason, just clone the repo and just run the Makefile and it will generate a file called serial.  Run this file (make sure your user is part of the `dialout` group or run as root) and it will guide you through the setup procedure.
