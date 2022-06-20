#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "com.h"
#include "chat.h"

char inbuf[INPUT_LEN_MAX];
char outbuf[LINE_LEN_MAX];

int main(void){
	puts("Starting client...");
	com_client_init();
	puts("Connecting to server...");
	com_client_connect();
	puts("Connected.");
	do {
		fgets(inbuf, LINE_LEN_MAX-1, stdin);
		com_send(0, inbuf);
		com_recv(0, outbuf);
		printf("%s", outbuf);
	} while (strcmp(inbuf, QUIT_MSG) != 0);
	com_client_disconnect();
	printf("Chat end.\n");
	return 0;
}

