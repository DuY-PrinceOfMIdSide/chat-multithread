#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "com.h"
#include "chat.h"

char inbuf[LINE_LEN_MAX];
char outbuf[LINE_LEN_MAX+4];

int main(void){
	puts("Starting server...");
	com_server_init();
	puts("Waiting for a client...");
	com_server_connect();
	puts("Client connected.");
	while (com_recv(0, inbuf) > 0){
		printf("Message from client:%s", inbuf);
		sprintf(outbuf, "[%d]%s", 0, inbuf);
		com_send(0 ,outbuf);
		printf("Message to client:%s", outbuf);
	}

	com_server_disconnect(0);
	return 0;
}
