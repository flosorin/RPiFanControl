# RPiFanControl

Simple cpp executable controlling a fan according to Raspberry Pi CPU temperature. It uses the [CppRPiGPIO class](https://github.com/flosorin/CppRPiGPIO).

NB: A much simplier and liter solution is provided with a bash script with [RPiFanControlV2](https://github.com/flosorin/RPiFanControlV2). This repo is kept as an example of CppRPiGPIO implementation.

## How does it works

### Hardware

Please refer to the schematic enclosed. 

As the fans typically used to cool down RPi require 5V - 200mA power supply, we cannot wire them directly to a GPIO to turn them on / off, because RPi GPIOs can only deliver 3.3V, up to 16mA each. Instead, we have to power it using the RPi 5V pin and to use a NPN transistor, which works as a switch triggered by the GPIO.

### Software

The program recover and parse the output of the command "vcgencmd measure_temp" to know the CPU temperature. Then, when the maximum threshold is reach (55°C by default), the fan is triggered. When the temperature decreases under the minimum threshold (45°C by default), the fan is turned off.

For this to work, "vcgencmd" must be in the path, which might not be the case according to your OS. I personnaly use OSMC on my RPi and the vcgencmd binary is located at /opt/vc/bin/, so I created a symbolic link in /usr/bin/ (sudo ln -s /opt/vc/bin/vcgencmd /usr/bin/). 

## Install

The provided script install.sh will build the sources and create a service that runs at boot.
