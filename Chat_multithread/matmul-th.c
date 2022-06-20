#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 64
#define PRINT_SIZE 8

typedef struct{
  int id;
  int row;
  int col;
}arg_t;

// matrix[row][col]
int a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

void init_a(int mat[SIZE][SIZE]){
	int row, col;
	for (row = 0 ; row < SIZE ; row++){
		for (col = 0 ; col < SIZE ; col++){
			mat[row][col] = row+1;
		}
	}
}
void init_b(int mat[SIZE][SIZE]){
	int row, col;
	for (row = 0 ; row < SIZE ; row++){
		for (col = 0 ; col < SIZE ; col++){
			mat[row][col] = row+col+1;
		}
	}
}

void print_matrix_sub(int mat[SIZE][SIZE], int size, int abbr, int start_row){
	int row, col;

	for (row = start_row ; row < start_row+size ; row++){
		for (col = 0 ; col < size ; col++){
			printf("%4d ", mat[row][col]);
		}
		if (abbr){
			printf(" ... ");
			for (col = SIZE-size ; col < SIZE ; col++){
				printf("%4d ", mat[row][col]);
			}

		}
		printf("\n");
	}
}

void print_matrix(int mat[SIZE][SIZE]){
	int size, abbr;

	if (SIZE > PRINT_SIZE){
		size = PRINT_SIZE/2;
		abbr = 1;
	}
	else {
		size = SIZE;
		abbr = 0;
	}

	print_matrix_sub(mat, size, abbr, 0);
	if (abbr){
		printf(" ...\n");
		print_matrix_sub(mat, size, abbr, SIZE-size);
	}
}

void matmul(int row, int col, int a[SIZE][SIZE], int b[SIZE][SIZE], int c[SIZE][SIZE]){
	int  k, s;
	for (row = 0 ; row < SIZE ; row++){
		for (col = 0 ; col < SIZE ; col++){
			s = 0;
			for (k = 0 ; k < SIZE ; k++){
				s += a[row][k]*b[k][col];
			}
			c[row][col] = s;
		}
	}
}

void *mult_work(void *arg){
  arg_t p = *((arg_t *)arg);
  matmul(p.row,p.col,a,b,c);
  pthread_exit(NULL);
}

int main(void){
  int row, col, thread_number, i;
  pthread_t *thread = (pthread_t *)calloc(SIZE*SIZE, sizeof(pthread_t));
  arg_t *thread_data = (arg_t *)calloc(SIZE*SIZE, sizeof(arg_t));
  if(thread_data == NULL){
	fprintf(stderr, "calloc can not active!\n");
	exit(EXIT_FAILURE);
  }
	init_a(a);
	init_b(b);
	printf("A\n");
	print_matrix(a);
	printf("\nB\n");
	print_matrix(b);
	thread_number = 0;
	for(row = 0; row < SIZE; row++){
	  for(col = 0; col < SIZE; col++){
		thread_data[thread_number].id = thread_number;
		thread_data[thread_number].row = row;
		thread_data[thread_number].col = col;
		thread_number++;
	  }
	}
	thread_number = 0;
	for(row = 0; row < SIZE; row++){
	  for(col = 0; col < SIZE; col++){
		pthread_create(&thread[thread_number],NULL,mult_work,(void *)&thread_data[thread_number]);
		thread_number++;
	  }
	}
		for(i=0; i<(SIZE*SIZE); i++){
	  pthread_join(thread[i], NULL);
	  }
	printf("\nC=AB\n");
	print_matrix(c);
	free(thread);
	free(thread_data);
	return 0;
}
