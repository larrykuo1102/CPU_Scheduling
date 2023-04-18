#include <stdio.h>

# define QUEENS 9
int Queenes[QUEENS]={0},Counts=0;

int Check(int line,int list){
    int index;
    for (index=0; index<line; index++) {
        int data = Queenes[index];
        if (list==data) 
            return 0;
        else if ((index+data)==(line+list))
            return 0;
        else if ((index-data)==(line-list))
            return 0;
    }
    return 1;
}

void print(){
    int line;
    for (line = 0; line < QUEENS; line++){
        int list;
        for (list = 0; list < Queenes[line]; list++)
            printf("0");
        printf("#");
        for (list = Queenes[line] + 1; list < QUEENS; list++){
            printf("0");
        }
        printf("\n");
    }
    printf("================\n");
}

void eight_queen(int line){
    int list;
    for (list=0; list<QUEENS; list++) {
        if (Check(line, list)) {
            Queenes[line]=list;
            if (line==7) {
                Counts++;
                print();
                Queenes[line]=0;
                return;
            }
            eight_queen(line+1);
            Queenes[line]=0;
        }
    }
}

int main() {
    eight_queen(0);
    printf("Eight Queens has %d Solutions!",Counts);
    return 0;
}