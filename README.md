# Invisible Light Wristband
An Arduino script that uses the [Neosensory Bluefruit SDK](https://github.com/neosensory/neosensory-sdk-for-bluefruit) to send near-infrared and ultraviolet data to Buzz.

## Setup
[This tutorial](https://neosensory.com/blog/invisible-light-wristband/) will walk you through assembling the hardware required for this project and uploading the code. To compile this Arduino sketch, you must also install the [Plotter library](https://github.com/devinaconley/arduino-plotter) from Devin Conley, which you can do via Arduino's Library Manager. 

## Pairing
Once you have created the invisible light sensing wristband and uploaded the code, turn on your Buzz wristband and press and hold the plus and minus buttons on top of your Buzz to put it in pairing mode. Your invisible light sensing wristband should quickly connect to your Buzz.

## Use
When connected to the invisible light sensing wristband, your Buzz wristband will vibrate to indicate when there is a change in near-infrared or ultraviolet light intensity. One motor will vibrate for IR and another for UV. The motors vibrate if the UV or IR signal deviates from that signal's running mean. The intensity of vibration indicates how much the signal has deviated from the running mean. The vibrations also use different textures to indicate a positive or a negative deviation: a smooth vibration is a positive change while a 'rough' or 'pulsating' vibration indicates a negative change.

Try moving your wristband into and out of the sun to feel the change in UV signal (there will also be some degree of change in IR signal). You should feel a strong, smooth vibration when you move your wrist out of the shade and into the sun, which will decrease gradually as the UV signal's running mean adjusts to this new, sunny environment. Moving your wrist from the sun to the shade (after the running mean has adjusted) will be similar, except with a 'rough' vibration texture to indicate a decrease in UV. 

## Plotting
This Arduino sketch is setup to plot the UV and IR signal data for convenient visualizing. To use these plots, follow the [installation instructions](https://github.com/devinaconley/arduino-plotter/wiki/Installation-and-Quickstart#installation) on Devin Conley's Arduino Plotter repository. If you are not using the plots, you still must install the library in order to compile the code or instead you must remove the `plot.ino` file and the plot functions from the `invisisble_light_wristband.ino` file.
