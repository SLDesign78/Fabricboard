#! /usr/bin/python3
# Description : This program manage the I2C bus on a RPI connected to a C3RPI
#
# History :
#   - 18/01/2024 : Anis HAMOUDA : ajouter un time.sleep (0.1) entre chaque cmd i2ctransfer
#   - 22/03/2022 : A.Algrin     : Creation
TOOL_VERSION = "0.2"
TOOL_TXT = "This program manage the I2C bus on a RPI connected to a C3RPI"

#==========================================================================
# IMPORTS
#==========================================================================
import os
import sys
import argparse
# import json
import time

#==========================================================================
# GLOBAL VARIABLE
#==========================================================================
CPLD_SBI_I2C_ADDR_UPDATE = 0x40
CPLD_SBI_I2C_ADDR_DATA = 0x42

FRU_I2C_ADDRESS_OFFSET = {"board":0x00, "system":0x10}

TEMP_FILE_READ_NAME = "temp_file_read_for_fru"

BIN_TEMP_FILE_NAME = "temp_file_for_fru"
XML_TEMP_FILE_NAME = "%s.xml" %(BIN_TEMP_FILE_NAME)

#==========================================================================
# MAIN PROGRAM
#==========================================================================
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=TOOL_TXT)
    parser.add_argument("fru", choices=['board', 'system'], help="Command. Possible value are read : 'r' or write : 'w'"            )
    parser.add_argument("-p", "--port", default="14", type=str, help="I2C bus id.")
    parser.add_argument("-f", "--file_fru_bin"   , default=""  , type=str, help="Binary file to write in the system FRU. If this option is set with the system fru, the system fru is write. Otherwise the select FRU is read. ")
    
    try:
        parser.add_argument("-r", "--raw", default=False, action=argparse.BooleanOptionalAction, help="If set, read data will be print raw.")
    except AttributeError:
        parser.add_argument("-r", "--raw", default=False, action="store_true", help="If set, read data will be print raw.")


    args = parser.parse_args()

    fru_type  = args.fru
    i2c_port  = int(args.port)
    file_fru_bin = args.file_fru_bin
    print_raw = args.raw

    if file_fru_bin == "":
        i2c_reg_address = 0x11
        i2c_reg_fru_datas = i2c_reg_address + 3
        
        fru_data_list = []
        
        for i in range (FRU_I2C_ADDRESS_OFFSET[fru_type], FRU_I2C_ADDRESS_OFFSET[fru_type] + 16):
            # Setting Address register
            command = "i2ctransfer -f -y %d w2@0x%02X 0x%02X 0x%02X" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_address, i)
            print(command)
            os.popen(command)
            time.sleep (0.1)
            # Reding data
            command = "i2ctransfer -f -y %d w1@0x%02X 0x%02X r16" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_fru_datas)
            print(command)
            read_data = os.popen(command).readlines()[0].split(" ")
            time.sleep (0.1)
            
            for data in read_data:
                fru_data_list.append(data.replace("\n", ""))
                if print_raw:
                    print ("%s " %data.replace("0x", "").replace("\n", ""), end="")
                    
            if print_raw:
                print ("")
        
        if not print_raw:
            temp_file_read = open(TEMP_FILE_READ_NAME, "wb")
            
            for data in fru_data_list:
                temp_file_read.write (bytearray.fromhex(data.replace("0x", "")))
            
            temp_file_read.close()
            
            read_data = os.popen("bull-ipmi-fru %s" %TEMP_FILE_READ_NAME).readlines()
            
            for line in read_data:
                print (line, end="")
        
        
    elif file_fru_bin != "":
        print ("Writing datas to system FRU base on file : %s" %file_fru_bin)
      
        print ("Writing binary file")
        bin_file = open (file_fru_bin, 'rb')
        bin_file_data = bin_file.readlines()[0]
        
        binData = []
        
        data_to_write = "0x00"
        for i in range (0, len(bin_file_data)):
            binData.append(bin_file_data[i])
            data_to_write = "%s 0x%02x" %(data_to_write, bin_file_data[i])
            
        bin_file.close()
        
        i2c_system_fru_write_data_reg_address = 0x24
        
        for i in range (0, 256, 16):
            data_to_write_str = "0x%02X" %i2c_system_fru_write_data_reg_address
            for j in range (i, i+16):
                data_to_write_str = "%s 0x%02X" %(data_to_write_str, bin_file_data[j])
            
            command = "i2ctransfer -f -y %d w17@0x%02X %s" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, data_to_write_str)
            print(command)
            read_data = os.popen(command).readlines()
            time.sleep (0.1)
            i2c_reg_fru_address = 0x11
            i2c_reg_fru_address_val = int(i/16)
            i2c_reg_fru_command_val = 0x02
            
            command = "i2ctransfer -f -y %d w3@0x%02X 0x%02X 0x%02X 0x%02X" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_fru_address, i2c_reg_fru_address_val, i2c_reg_fru_command_val)
            print(command)
            read_data = os.popen(command).readlines()
            
            time.sleep (0.1)
            
            i2c_reg_fru_command_val = 0x00
            i2c_reg_fru_command = 0x12
        
            command = "i2ctransfer -f -y %d w2@0x%02X 0x%02X 0x%02X" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_fru_command, i2c_reg_fru_command_val)
            print(command)
            read_data = os.popen(command).readlines()
            
            time.sleep (0.1)
            
            i2c_reg_fru_command_val = 0x04
            command = "i2ctransfer -f -y %d w2@0x%02X 0x%02X 0x%02X" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_fru_command, i2c_reg_fru_command_val)
            print(command)
            read_data = os.popen(command).readlines()
            time.sleep (0.1)
            i2c_reg_fru_command_val = 0x00
            command = "i2ctransfer -f -y %d w2@0x%02X 0x%02X 0x%02X" %(i2c_port, CPLD_SBI_I2C_ADDR_DATA, i2c_reg_fru_command, i2c_reg_fru_command_val)
            print(command)
            read_data = os.popen(command).readlines()
            
    else:
        print ("ERROR : Unknown FRU type or command.")
        sys.exit(-1)

    sys.exit(0)