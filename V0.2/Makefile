CC = gcc
CFLAGS = -Wall

file_out="IHM_INT_fabricboard"
file_c_1="IHM_INT_fabricboard.c"
file_c_2="SLD_PiI2C.c"
file_c_3="SLD_GPIO.c"
file_c_4="SLD_MISC_FUNC.c"
file_c_5="SLD_IHM_Console.c"
file_c_6="SLD_POWER_REFCLK_RESET.c"
file_c_7="SLD_SLOT.c"
file_c_8="SLD_PEX.c"
file_c_9="MCP23017.c"
file_c_10="SHT4x.c"
file_c_11="PCA9545A.c"
file_c_12="EMC2302.c"
file_c_13="ISL28022.c"




all: IHM_INT_fabricboard



IHM_INT_fabricboard: IHM_INT_fabricboard.c
	$(CC) $(CFLAGS) -o $(file_out) $(file_c_1) \
			 $(file_c_2) \
			 $(file_c_3) \
			 $(file_c_4) \
			 $(file_c_5) \
			 $(file_c_6) \
			 $(file_c_7) \
			 $(file_c_8) \
			 $(file_c_9) \
			 $(file_c_10) \
			 $(file_c_11) \
			 $(file_c_12) \
			 $(file_c_13) \
			 -lcrypt -lm -lrt -lpthread -lgpiod


clean:
	rm -f IHM_INT_fabricboard 
