#!/bin/sh

# on
echo "default-on" > /sys/class/leds/ds\:green\:wlan/trigger
# blink
echo "timer" > /sys/class/leds/ds\:green\:wlan/trigger
echo 100 > /sys/class/leds/ds:green:wlan/delay_on
echo 900 > /sys/class/leds/ds:green:wlan/delay_off
# off
echo "none" > /sys/class/leds/ds\:green\:wlan/trigger
