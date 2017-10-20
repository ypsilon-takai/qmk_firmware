#!/bin/bash

for i in {5..1}; do
  echo "$i"
  sleep 1
done

dfu-programmer atmega32u4 erase

dfu-programmer atmega32u4 flash planck_rev4_yosi.hex

dfu-programmer atmega32u4 start
