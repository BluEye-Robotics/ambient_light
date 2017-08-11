# Ambient light

This project emulates ambient light at different water depths.

There are three different source folders in src/. One for the neopixel ring, one for the led strips and one using SDL to visualize the colors on the computer screen without arduino.
See the READMEs in the respective source folders for further instructions.

[src/specrend.h](src/specrend.h) is an external module for color spectra to rgb rendering used in both the arduino projects. It is modified to use less RAM, so that it runs on arduino Uno.

The variable absorptions is defined in all main programs and contains the absorptions from START_WAVE_LENGTH to STOP_WAVE_LENGTH with step WAVE_LENGTH_STEP as per http://omlc.org/spectra/water/data/smith81.dat.
This data is probably among the largest sources of error since absorption varies a lot with parameters like algae and water quality.

Another source of error is the data for the sunlight spectra.
In the main programs this is set in the sun_spec function, which calculates black body radiation with the temperature SUNLIGHT_TEMP.
This temperature varies especially with day of year, hour of day and weather.
