#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "com.h"
#include "chat.h"
#include "msg_queue.h"

typedef struct {
  int id;
}arg_t;

arg_t *thread_data;
pthread_mutex_t lock;


void *server_receiver(void *argv){
  arg_t *p = (arg_t *)argv;
  char inbuf[LINE_LEN_MAX];
  char outbuf[LINE_LEN_MAX+4];
  while (com_recv(p->id,inbuf)){
	printf("Message from client[%d]:%s",p->id,inbuf);
		sprintf(outbuf, "[%d]%s", p->id, inbuf);
		pthread_mutex_lock(&lock);
		msg_queue_enqueue(outbuf);
		pthread_mutex_unlock(&lock);
	}
  return NULL;
}

void *server_sender(void *argv){
  int i;
  char msg[LINE_LEN_MAX+4];
  while (1){
	pthread_mutex_lock(&lock);
	  if (msg_queue_dequeue(msg) == 0){
		  for (i = 0; i < CLIENT_MAX; i++){
			if (com_is_client_connected(thread_data[i].id) > 0){			
				com_send(thread_data[i].id,msg);
			}
		  } 
	  }
	pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main(void){
  int i;
  pthread_t *threads;
  pthread_t  thread1;
  thread_data = (arg_t*)malloc(CLIENT_MAX*sizeof(arg_t));
  threads = (pthread_t*)malloc((CLIENT_MAX)*sizeof(pthread_t));
	for(i = 0; i < CLIENT_MAX; i++){
		thread_data[i].id = -1;
	}

	puts("Starting server...");
	com_server_init();
	pthread_create(&thread1,NULL,&server_sender,NULL);
	//	pthread_detach(thread1);
	puts("Waiting for a client...");
	  	for(i = 0; i < CLIENT_MAX; i++){
	  	  thread_data[i].id = com_server_connect();
		  printf("%d",thread_data[i].id);
	    	  if(thread_data[i].id == INVALID_CLIENT_ID) break;
	    	  pthread_create(&threads[i],NULL, &server_receiver,(void  *)&thread_data[i]);
			  //pthread_detach(threads[i]);
		}
	puts("Client connected.");
	com_server_disconnect(0);
	free(thread_data);
	free(threads);
	return 0;
}
