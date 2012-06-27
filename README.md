linked-list-christmas
=====================
See blog post: http://www.gnewt.at/blog/2012/06/christmas-lights-linked-lists/

The Arduino serial interface works simply.
1. Open the Arduino's serial port at baudrate 9600
2. Write two bytes to the port. The first nibble is empty* and the next three are the R, G, and B values respectively.
3. Dance

* If you want to do something like set the entire string of lights at once, you run into an annoying issue.
The lights update every single time you send data, so updating all 36 looks very blinky and stuttery.
To alleviate this, you can turn light updating OFF by setting the first nibble of a command to 0xF.
You can then re-enable the light updating by setting the first nibble of a command to 0xE.

Example (python):
~~~
a = serial.Serial('/dev/tty.usbmodem411', baudrate=9600)
a.write('\x00\xf0') # Adds a green light

a.write('\xf0\x0f') # Stops light updates, adds a blue light
# <add some more light colors here-- the new colors will not show up yet>
a.write('\xe0\xf0') # The last color added is green, and the whole string updates once.
~~~
