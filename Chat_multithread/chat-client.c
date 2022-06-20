#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "com.h"
#include "chat.h"

char inbuf[INPUT_LEN_MAX];
char outbuf[LINE_LEN_MAX+4];

int end = 0;

void *send_msg(void *agrv){
  while(1){
	fgets(inbuf, LINE_LEN_MAX-1, stdin);
	com_send(0, inbuf);
	if(strcmp(inbuf, QUIT_MSG) == 0){
	  end = 1;
	  break;
	}
  } 
}
void *receive_msg(void *agrv){
  do{
	com_recv(0,outbuf);
	printf("%s",outbuf);
  }while(end == 0);
}


int main(void){
  pthread_t thread_rcv;
  pthread_t thread_send;
	puts("Starting client...");
	com_client_init();
	puts("Connecting to server...");
	com_client_connect();
	puts("Connected.");
    pthread_create(&thread_send,NULL,&send_msg,NULL);
	pthread_create(&thread_rcv,NULL,&receive_msg,NULL);
	pthread_join(thread_rcv,NULL);
	pthread_join(thread_send,NULL);
	com_client_disconnect();
	printf("Chat end.\n");
	return 0;
}

