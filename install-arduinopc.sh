#!/bin/bash

if [[ "$EUID" != "0" ]]; then
    echo "This script must be run as root"
    exit 1
fi

srcDir=/opt/GitHub/ArduinoPC
binDir=/opt/QtBuilds/ArduinoPC/Debug

ln -s $srcDir/arduinopc.desktop /usr/share/applications/
ln -s $srcDir/arduinopc.png /usr/share/pixmaps/
ln -s $srcDir/run-arduinopc.sh /usr/bin/
ln -s $binDir/ArduinoPC /usr/bin/
