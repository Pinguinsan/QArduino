#!/bin/bash

ArduinoPC >> /tmp/$(date +%s)-$$-qserialterminal.log

return $?
