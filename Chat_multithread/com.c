#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "com.h"

#define INVALID_FD (-1)

static int myfd, fd[CLIENT_MAX];
static struct sockaddr_un addr;
static char msg_buf[CLIENT_MAX][LINE_LEN_MAX+1];
static char *msg_buf_head[CLIENT_MAX];
static int msg_left_bytes[CLIENT_MAX];
static int client_num = 0;

void com_init(void){
	bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCK_FILENAME);
	if ((myfd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1){
		printf("error in socket()\n");
		exit(1);
	}
}

void com_server_init(void){
	int i;
	unlink(SOCK_FILENAME);
	com_init();
	if (bind(myfd, (struct sockaddr *)&addr, sizeof(addr))){
		printf("error in bind()\n");
		exit(1);
	}
	for (i = 0 ; i < CLIENT_MAX ; i++){
		fd[i] = INVALID_FD;
	}
}

int com_server_connect(void){
	int id, new_fd;

	if (listen(myfd, 1)) {
		printf("error in listen()\n");
		exit(1);
	}
	if ((new_fd = accept(myfd, NULL, 0)) == -1){
		printf("error in accept()\n");
		exit(1);
	}

	for (id = 0 ; id < CLIENT_MAX; id++){
		if (fd[id] == INVALID_FD) break;
	}
	if (id == CLIENT_MAX) return INVALID_CLIENT_ID;
	fd[id] = new_fd;
	client_num++;
	return id;
}

void com_server_disconnect(int id){
	close(fd[id]);
	fd[id] = INVALID_FD;
	client_num--;
}


void com_client_init(void){
	com_init();
}


void com_client_connect(void){
	while (connect(myfd, (struct sockaddr *)&addr, sizeof(addr))){
		sleep(RETRY_INTERVAL);
	}
	fd[0] = myfd;
}

void com_client_disconnect(void){
	close(fd[0]);
}

int com_send(int id, char *msg){
	return send(fd[id], msg, strlen(msg)+1,0);

}
int com_recv(int id, char *msg){
	int msg_len = 0;

	if (msg_left_bytes[id] == 0){
		msg_left_bytes[id] = recv(fd[id], msg_buf[id], LINE_LEN_MAX+1, 0);
		msg_buf_head[id] = msg_buf[id];
	}
	if (msg_left_bytes[id] > 0){
		msg_len = strlen(msg_buf_head[id])+1;
		strcpy(msg, msg_buf_head[id]);
		msg_left_bytes[id] -= msg_len;
		msg_buf_head[id] += msg_len;
	}
	return msg_len;
}

int com_is_client_connected(int id){
	if (fd[id] == INVALID_FD){
		return 0;
	}
	else {
		return 1;
	}
}
