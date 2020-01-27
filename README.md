# Night Lamp
#### Hi everyone, I wanted to buy a night lamp but then I decided to make my own so here is the result:
![20200122_230702](https://user-images.githubusercontent.com/30238276/73168447-e5da7880-410a-11ea-9eac-25e2bc594a43.jpg)
## Project Information
I used Arduino nano and rotary encoder for user interaction. I also implemented EEPROM operations such as save the current mode, brightness, led status. This lamp work with 3.3V lipo battery but it has 3.3v to 5v boost converter to supply enough power to Arduino. This lamp also has battery level indicator on top of the oled display there is no voltmeter, I just connected battery to arduino's analog pin to measure 3.3v battery voltage then divided with 5 so each "I" represents 3.3/5 voltage level. This means battery is full if you see "IIIII". The base and top is 3D printed. On top, I used transparent filament to make the RGB leds light visible.
For leds I used Neopixel circle RGB leds circuit.
* There is much more than just static light, there are animated fire and animated spiral light effects in it!
* Code works multithread so you can change the settings while the animation is playing.,
* You can place order for this at <a href="https://www.ege3dbaski.studio/">ege3dbaski.studio</a>
## Photos
![1](https://user-images.githubusercontent.com/30238276/73170478-8e8ad700-410f-11ea-8ce2-c9df5441f492.jpg)
![2](https://user-images.githubusercontent.com/30238276/73170479-8f236d80-410f-11ea-8471-b91758a5ef62.jpg)
![3](https://user-images.githubusercontent.com/30238276/73170480-8f236d80-410f-11ea-9e0b-53d3d47c6fe9.jpg)
![4](https://user-images.githubusercontent.com/30238276/73170481-8f236d80-410f-11ea-986a-d1a57ce705ca.jpg)
[![image](https://i.hizliresim.com/86qM8k.jpg)](https://hizliresim.com/86qM8k)
