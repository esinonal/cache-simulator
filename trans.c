/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void func1(int M, int N, int A[N][M], int B[M][N]);
void func2(int M, int N, int A[N][M], int B[M][N]);
void func3(int M, int N, int A[N][M], int B[M][N]);
/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
        if ((M==32)&&(N==32)){
          func1(M, N, A, B);
        }
        else if ((M==32)&&(N==64)){
          func2(M, N, A, B);
       }

        else if ((M==64)&&(N==64)){
          func3(M, N, A, B);
        }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}
char func1char[] = "func1-32x32";
void func1(int M, int N, int A[N][M], int B[M][N]){

    int var1;
    int var2;
    int var3;
    int var4;
    int var5;
    int var6;
    int var7;
    int var8;

    for (int i = 0; i < N; i += 8) {  //block by 8
      for (int jj = 0; jj < M; jj += 8) {
        for (int ii = i; ii< i+8; ii++ ){

          var1= A[ii][jj];
          var2= A[ii][jj+1];
          var3= A[ii][jj+2];
          var4= A[ii][jj+3];
          var5= A[ii][jj+4];
          var6= A[ii][jj+5];
          var7= A[ii][jj+6];
          var8= A[ii][jj+7];

          B[jj+7][ii]=var8;
          B[jj+6][ii]=var7;
          B[jj+5][ii]=var6;
          B[jj+4][ii]=var5;
          B[jj+3][ii]=var4;
          B[jj+2][ii]=var3;
          B[jj+1][ii]=var2;
          B[jj][ii]=var1;
        }
      }
    }
}

char func2char[] = "func2-32x64";
void func2(int M, int N, int A[N][M], int B[M][N]){

    int var1;
    int var2;
    int var3;
    int var4;

    for (int i = 0; i < N; i += 8) {  //block by 4x8
      for (int jj = 0; jj < M; jj += 4) {
        for (int ii = i; ii< i+8; ii++ ){

          var1= A[ii][jj];
          var2= A[ii][jj+1];
          var3= A[ii][jj+2];
          var4= A[ii][jj+3];

          B[jj+3][ii]=var4;
          B[jj+2][ii]=var3;
          B[jj+1][ii]=var2;
          B[jj][ii]=var1;
        }
      }
    }
}
char func3char[] = "func3-64x64";
void func3(int M, int N, int A[N][M], int B[M][N]){

    int var1;
    int var2;
    int var3;
    int var4;

    for (int i = 0; i < N; i += 4) {  //block by 4x4
      for (int jj = 0; jj < M; jj += 4) {
        for (int ii = i; ii< i+4; ii++){

          var1= A[ii][jj];
          var2= A[ii][jj+1];
          var3= A[ii][jj+2];
          var4= A[ii][jj+3];

          B[jj+3][ii]=var4;
          B[jj+2][ii]=var3;
          B[jj+1][ii]=var2;
          B[jj][ii]=var1;
        }
      }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register any additional transpose functions */
    registerTransFunction(func1, func1char);
    registerTransFunction(func2, func2char);
    registerTransFunction(trans, trans_desc);
    registerTransFunction(func3, func3char);
    registerTransFunction(transpose_submit,transpose_submit_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
