## ATtiny85 NeoPixel


For use with ATtiny85 (or Arduino!) and WS2811 LEDs

### Instructions

* Download Adafruit's [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library
* Get the [ATtiny cores](https://code.google.com/p/arduino-tiny/) for Arduino & a [programmer](https://www.adafruit.com/products/46)
* Attach your LED data line to Digital Pin 4
* Attach momentary push button/switch to Digital Pin 0, and 5V VCC
* Upload the sample sketch and enjoy your light show! 
* Create and share patterns of your own :-)

![ATtiny85](/attiny.jpg)
![Fritzing Diagram](/attiny85_neopixel.png)


### More Info

Need help getting started with ATtiny and lights? See [Instructable](http://www.instructables.com/id/Use-a-1-ATTiny-to-drive-addressable-RGB-LEDs/) [danasf](https://github.com/danasf) wrote on the subject.

If you're seeing 'R_AVR_13_PCREL' errors when you try to compile, try this [thread](http://forum.arduino.cc/index.php?topic=116674.0) for solutions. This is a known problem with Arduino IDE, ATtiny and sketches > 4k.
