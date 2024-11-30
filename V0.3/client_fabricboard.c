/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  Server_fabricboard.c
*************************************************************************************************************/
//  Description  : Interrupt Compule Module Fabricboard
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 18/07/2023: creation

*************************************************************************************************************/

#include "fabricboard.h"


// ***** User Socket commands *****
const char* set_fan1_CMD 			= "set_fan1";
const char* set_fan2_CMD 			= "set_fan2";



void send_mess(int fd, char* _str1, char* _str2) {

	char *message = (char*) calloc(strlen(_str1) + strlen(_str2) + 1, sizeof(char));
	strcpy(message, _str1);
	strcat(message, ",");
	strcat(message, _str2);
	send(fd, message, strlen(message)+1, 0);	
	free(message);
}	



int main(int argc, char *argv[]){
 
	int server_fd;
    struct sockaddr_un server_addr;
    int connection_result;


    if((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
		printf ("Error: Create Socket: %s\n", strerror (errno));
		return HIGH;
	}

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, INFO_SERVER_PATH);

    if ((connection_result = connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0 ) {
		printf ("Error: Socket Connect: %s\n", strerror (errno));
		return HIGH;		
	}	

    if (connection_result == -1) {
        perror("Error:");
        exit(1);
    }

	// S E T  F A N 1
	if(StringComp((char*)argv[1], set_fan1_CMD)) {	 
		//Check if argv[2] is present
		if(argc==3) { 
			//Send command to Server
			send_mess(server_fd, argv[1], argv[2]); 		
		}
		//argv[2] is not present
		else if(argc==2) {
			send(server_fd, argv[1], strlen(argv[1])+1, 0);
		}
	}

	
	// S E T  F A N 2
	else if(StringComp((char*)argv[1], set_fan2_CMD)) {
		//Check if argv[2] is present
		if(argc==3) { 
			//Send command to Server
			send_mess(server_fd, argv[1], argv[2]); 
		}
		//argv[2] is not present
		else if(argc==2) {
			send(server_fd, argv[1], strlen(argv[1])+1, 0);
		}	
	}
	
	// O T H E R
	else {
		//Send command to Server
		char *message = (char*) calloc(strlen(argv[1]) + 1, sizeof(char));
		strcpy(message, argv[1]) ;
		send(server_fd, message, strlen(message)+1, 0);		
		free(message);
	}	







	//Data Returned from Server
    char *buffer = (char*) calloc(buffersize, sizeof(char)) ;
    read(server_fd, buffer, buffersize);
    printf("%s\n", buffer);

    //printf("Client: I recieved %c from server!\n", ch);
    //free(message);
    free(buffer);
	close(server_fd);
    exit(0);
 
}
