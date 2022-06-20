#ifndef _MSG_QUEUE_H
#define _MSG_QUEUE_H

#define MSG_QUEUE_SIZE 256

void msg_queue_init(void);
int msg_queue_enqueue(char *msg);
int msg_queue_dequeue(char *msg);

#endif
