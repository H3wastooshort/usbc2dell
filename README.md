# USB-C to DELL
An adapter to allow charging Laptops equipped with legacy connectors via USB Type-C.

![a rendered animation of the PCB roatating](https://h3wastooshort.github.io/usbc2dell/rotating.gif)

## Why make my own?

I could just slap together a USB-C PD trigger-board and a cut-off end of a cable and call it a day, but that would cause two problems:
 - the laptop won't charge because it's not seeing an ID chip
 - even if it does use the power supply, it might draw too much current if connected to for example a 45W USB-C power supply.

This project remidies this, by negotiating USB PD and then emulating an ID chip with the same parameters as read from the USB-C power supply. Using a 45W USB-C power supply, the laptop will know that it's only capable of 45W.

## Schematic
![the schematic](https://h3wastooshort.github.io/usbc2dell/usbc2dell.svg)

## PCB
![a rendered image of the PCB front](https://h3wastooshort.github.io/usbc2dell/top.png)
![a rendered image of the PCB back](https://h3wastooshort.github.io/usbc2dell/bottom.png)
