#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// N-queenのN
#define N 13
// 格納可能な解の個数
#define M 100000

typedef struct{
  int row;
 }arg_t;

int answer[N][M];
int answer_num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *nqueen_top(void *arg);
void print_ans(void);
void nqueen(int column, int ans[]);
int can_put(int column, int row, int ans[]);

int main(void){
  int i;
  pthread_t  thread[N];
  arg_t *thread_data ;
  thread_data = (arg_t*)malloc(N*sizeof(arg_t));
  for (i = 0 ; i < N ; i++){
	thread_data[i].row = i;
	}
  for (i = 0 ; i < N ; i++){
	pthread_create(&thread[i],NULL,&nqueen_top,(void *)&thread_data[i]);
	}
  for(i=0; i < N;i++){
	pthread_join(thread[i],NULL);
  }
  print_ans();
  free(thread_data);
  return 0;
}

// 第0列目の第row行に置いたとき、残りの可能な置き方をすべて調べる
void *nqueen_top(void *arg){
  arg_t *p =  (arg_t *)arg ;
  int ans[N];
  ans[0] = p->row;
  nqueen(1, ans);
  return NULL;
}

// column列から右(column列含む)の可能な置き方をすべて調べる
// column列より左は配置済みでありans[0]...ans[column-1]に位置が格納されている
void nqueen(int column, int ans[]){
	int i, j;

	for (i = 0 ; i < N ; i ++){
		if (can_put(column, i, ans)){    // column列のi行目に置けるかチェック
			ans[column] = i;             // 置けるならansに格納
			if (column == N-1){          // 右端の列に置けたら完成なので解を格納
			  	pthread_mutex_lock(&mutex);
				for (j = 0 ; j < N ; j ++){
					answer[j][answer_num] = ans[j];
				}
				answer_num ++;
					pthread_mutex_unlock(&mutex);
			}
			else {                       // 右端列以外なら、次の列の配置に進む
				nqueen(column+1, ans);
			}
		}
	}
}

// column列のrow行に置けるなら非0を返す
int can_put(int column, int row, int ans[]){
	int i;

	for (i = 0 ; i < column ; i++){
		if (ans[i] == row){                      // 横並びチェック
			return 0;
		}
		else if (ans[i] - row == i - column){    // 斜め並びチェック(1)
			return 0;
		}
		else if (row - ans[i] == i - column){    // 斜め並びチェック(2)
			return 0;
		}
	}
	return 1;
}

// すべての解を出力する
void print_ans(void){
	int i, j;

	for (i = 0 ; i < answer_num ; i ++){
		for (j = 0 ; j < N ; j ++){
			// 0から9はそのまま、10以降はa,b,...で代用
			if (answer[j][i] < 10){
				printf("%d", answer[j][i]);
			}
			else {
				printf("%c", 'a' + answer[j][i]- 10);
			}
		}
		printf("\n");
	}
	printf("%d answer(s).\n", answer_num);
}

