# SerialCom
Repo for small project I'm working on to talk to serial devices through Linux.  I make no guarantees about the accuracy of any data written or read by this code.  If you have any suggestions feel free to let me know.

If you want to use this, just clone the repo and enter the src directory, and run `sudo make install`, which will build the program and install it to the `/usr/bin/` directory.  Then simply type `serialcom` in any terminal to start using the program.  Make sure that you are either running the program as root or are a member of the `dialout` group in order to access the serial devices.

Other options include:
`-d` - specify a device to connect to
`-b` - specify a baud rate to use
`-om` - specify which output mode should be used, can be ascii, hex, or file
`-im` - specify which input mode should be used, can be ascii, hex, or file
`-t` - specify a timeout for reading response data from the serial port, in 0.1's of a second
`-h` - show the help menu

While running the program, you can also input commands to change various settings:
`!dc` - disconnect from the current device
`!chdev` - change device to connect to
`!chbd` - change the baud rate for the serial connection
`!imode` - change the input mode, can be ascii, hex, or file
`!omode` - change the output mode, can be ascii, hex, or file
`!lscmd` - list all commands
`!lsbd` - list all supported baud rates on this executable
