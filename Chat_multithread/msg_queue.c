#include <stdio.h>
#include <string.h>
#include "msg_queue.h"
#include "chat.h"
#include "com.h"

static char buf[MSG_QUEUE_SIZE][LINE_LEN_MAX+1];
static int head, tail;

void msg_queue_init(void){
	head = 0;
	tail = 0;
}

int msg_queue_enqueue(char *msg){
	int new_tail;

	new_tail = (tail+1) % MSG_QUEUE_SIZE;
	if (new_tail == head) return -1; // キュー溢れ

	strncpy(buf[tail], msg, LINE_LEN_MAX+1);
	tail = new_tail;
	return 0;
}

int  msg_queue_dequeue(char *msg){
	if (head == tail) return -1; // キューが空
	
	strncpy(msg, buf[head], LINE_LEN_MAX+1);
	head = (head+1) % MSG_QUEUE_SIZE;
	return 0;
}

