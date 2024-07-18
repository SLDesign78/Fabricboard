#!/bin/bash

path1="/home/pi/PROJETS/FABRICBOARD/FW/V0.2/"
file_out="fabricboard"
file_c_1="fabricboard.c"
file_c_2="SLD_PiI2C.c"
file_c_3="SLD_GPIO.c"
file_c_4="SLD_MISC_FUNC.c"
file_c_6="SLD_POWER_REFCLK_RESET.c"
file_c_7="SLD_SLOT.c"
file_c_8="SLD_PEX.c"
file_c_9="MCP23017.c"
file_c_10="SHT4x.c"
file_c_11="PCA9545A.c"
file_c_12="EMC2302.c"
file_c_13="ISL28022.c"

#file_c_14=".c"
#file_c_15=".c"
#file_c_16=".c"
#file_c_17=".c"
#file_c_18=".c"

#compile
gcc -Wall -o "$file_out" "$path1$file_c_1" \
			 "$path1$file_c_2" \
			 "$path1$file_c_3" \
			 "$path1$file_c_4" \
			 "$path1$file_c_6" \
			 "$path1$file_c_7" \
			 "$path1$file_c_8" \
			 "$path1$file_c_9" \
			 "$path1$file_c_10" \
			 "$path1$file_c_11" \
			 "$path1$file_c_12" \
			 "$path1$file_c_13" \
			 -lcrypt -lm -lrt -lpthread -lgpiod

#Build
gcc -Wall -c "$path1$file_c_1"  -l pthread