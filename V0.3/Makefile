CC = gcc
CFLAGS = -Wall

file1_out="server_fabricboard"
file2_out="client_fabricboard"

file1_c_1="server_fabricboard.c"
file2_c_1="client_fabricboard.c"

file_c_2="SLD_PiI2C.c"
file_c_3="SLD_GPIO.c"
file_c_4="SLD_MISC_FUNC.c"
file_c_5="SLD_SERVER.c"
file_c_6="SLD_POWER_REFCLK_RESET.c"
file_c_7="SLD_SLOT.c"
file_c_8="SLD_PEX.c"
file_c_9="MCP23017.c"
file_c_10="SHT4x.c"
file_c_11="PCA9545A.c"
file_c_12="EMC2302.c"
file_c_13="ISL28022.c"


all: server_fabricboard client_fabricboard

server_fabricboard: server_fabricboard.c
	$(CC) $(CFLAGS) -o $(file1_out)  \
			 $(file1_c_1) \
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


client_fabricboard: client_fabricboard.c
	$(CC) $(CFLAGS) -o $(file2_out)  \
			 $(file2_c_1) \
			 $(file_c_4)


clean:
	rm -f server_fabricboard 
