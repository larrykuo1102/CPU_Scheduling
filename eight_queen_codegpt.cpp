

// #include <iostream> 
// #define N 8 
  
// // Function to check if two queens threaten each other or not 
// bool isSafe(int board[N][N], int row, int col) 
// { 
//     int i, j; 
  
//     /* Check this row on left side */
//     for (i = 0; i < col; i++) 
//         if (board[row][i]) 
//             return false; 
  
//     /* Check upper diagonal on left side */
//     for (i = row, j = col; i >= 0 && j >= 0; i--, j--) 
//         if (board[i][j]) 
//             return false; 

//     /* Check lower diagonal on left side */     for (i = row, j = col; j >= 0 && i < N; i++, j--) 

//         if (board[i][j]) 

//             return false; 

  

//     return true; // no queen threatens this position so it's safe.  

  
// }
  

//  // Function to print the solution matrix.  

//  void printSolution(int board[N][N]) {  

//      for (int i = 0; i < N; i++) {  

//          for (int j = 0; j < N; j++) {  

//              cout << board[i][j] << " " ;  

//          } 
//          cout << endl ;  

//      } 
//      cout << endl ; 
// }     // Function to solve the 8 Queen problem using backtracking.    
// bool solve8Queen(int board[N][N], int col) {      // If all queens are placed then return true.      
//      if (col >= N)      return true ;       // Consider this column and try placing this queen in all rows one by one.       
//      for (int i = 0 ; i < N ; i++) {          // Check if queen can be placed on board[i][col]          
//       if (isSafe(board , i , col)) {              // Place this queen in board position board[i][col]             
//       board[i][col] = 1 ;               // recur to place rest of the queens              
//       if (solve8Queen(board , col + 1))              
//       return true ;               // If placing queen in board
//       }}}
// //  how to implement eight queen algorithm 




































#include <iostream>
#include <vector>
 
using namespace std;
 
int N = 8; // Size of the chessboard.
 
// Function to check if two queens threaten each other or not.
bool isSafe(vector<vector<int>> &board, int row, int col) {

    // Check this row on left side.
    for (int i = 0; i < col; i++) {

        // Check if there is a queen present in that row.
        if (board[row][i] == 1) {

            return false;
        }
    }

    // Check upper diagonal on left side. 
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {

        // Check if there is a queen present in that diagonal. 
        if (board[i][j] == 1) {

            return false;
        } 											   }

    // Check lower diagonal on left side. 
    for (int i = row, j = col; j >= 0 && i < N; i++, j--) {

        // Check if there is a queen present in that diagonal. 
        if (board[i][j] == 1) {

            return false;   }   }   
            return true; 
}   

void solveNQueen(vector<vector<int>> &board, int col) {   /* base case: If all queens are placed then return true */   
    if (col >= N) {     
        cout << "Solution: " << endl << endl;;     
        for (int i = 0; i < N; ++i) {       
            for (int j = 0 ; j < N ; ++j){         
                cout << board[i][j] << " " ;       
            }       
            cout << endl ;     
        }     
        cout << endl ;     
        return ;   
    }   /* Consider this column and try placing this queen in all rows one by one */   
    for (int i = 0 ; i < N ; ++i){     /* Check if the queen can be placed on board[i][col] */     
        if (isSafe(board, i , col)){       /* Place this queen in board[i][col] */       
            board[i][col] = 1 ;       /* recur to place rest of the queens */       
            solveNQueen(board , col + 1);      /* If placing queen in board[i][col] doesn't lead to a solution then remove queen from board[i][col] */       
            board[i][col] = 0 ;     
        }
    }  
} 


int main() {   
    vector<vector<int>> board(N , vector<int>(N , 0));      
    solveNQueen(board , 0);      
    return 0;
}