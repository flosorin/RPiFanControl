#!/bin/bash

# Create executable
make

# Copy executable and service file in the rigth location
sudo cp install/fancontrol.service /etc/systemd/system/
sudo cp install/FanControl /usr/local/bin/

# Add user to group gpio (create it if it does not exist)
if [ ! $(getent group gpio) ]; then
       sudo groupadd gpio	
fi
sudo usermod -aG gpio $(logname)

# Enable service to run at boot
sudo systemctl enable fancontrol


