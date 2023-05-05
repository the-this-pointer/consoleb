# consoleb
##### A Console Compatible With Embedded Devices

This console is used in my project [here](https://github.com/the-this-pointer/timer-board-stm32f103). 
It supports both update callback for the page and a ready to use menu system.

You can see the working example on an OLED LCD in the link above, or just clone this repository on a `Windows` machine and compile it.
It uses `pthread` for the ui thread on windows, and `FreeRTOS` on the Timer project.

Just remember I seperated this code from the Timer repository and I've planned to update it later when the repo become stable. 
