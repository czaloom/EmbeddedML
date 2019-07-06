In these folders, compiled images of the firmware are provided, in order to allow the user to test the application in a quick and easy way.

The binaries are provided in .hex format and can be flashed into the micro-controller flash memory using the procedures described below. 

1 - Install ST-LINK Utility: http://www.st.com/web/en/catalog/tools/FM146/CL1984/SC720/SS1450/PF251168.
2 - Plug the Nucleo Board to the host PC using a micro USB cable.
3 - Connect the Nucleo board SWD connector to the SensorTile using the provided cable (see the user manual for more information)
4 - Open the ST-LINK utility.
5 - Connect to the board selecting "Target -> Connect" from the menu or pressing the corresponding button.
6 - Open the binary file selecting "File -> Open File..." and choose the one you want to flash.
7 - From the menu choose: "Target -> Program"
8 - The Start address, it's 0x08004000.
9 - Click Start.
10- Wait until flashing is complete.




 