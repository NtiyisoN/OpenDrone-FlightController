#!/bin/bash
#script for installing the neccassary modules for the wifi-hotspot
sudo easy_install pip 
sudo pip install --upgrade virtualenv 
pip install --upgrade pip3
pip install .
sudo apt-get update 
sudo apt update && sudo apt --yes --force-yes install dnsmasq hostapd python3-dev unzip python3-pip && cd ~ && wget --output-document=pyaccesspoint-master.zip https://github.com/Goblenus/pyaccesspoint/archive/master.zip && unzip pyaccesspoint-master.zip && cd pyaccesspoint-master && sudo python3 setup.py install && cd ~ && sudo rm -rf pyaccesspoint-master.zip pyaccesspoint-master 
sudo echo '{"ssid": "OpenDrone", "inet": null, "wlan": "wlan0", "password": "1234567890", "netmask": "255.255.255.0", "ip": "10.0.0.138"}' >> /etc/accesspoint/accesspoint.json 
