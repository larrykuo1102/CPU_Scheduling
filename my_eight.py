k = 8
all_list = []
def put_queen(state:list,n:int):
    if ( n > 0 ):
        for row in range(1,k+1):
            for column in range(n, k+1):
                # if ( len(state) == k and n == k):
                #     print(state)
                #     all_list.append(state)
                #     state.clear()
                if check_valid(state,[row,column]) :
                    state.append([row,column])  
                    if len(state) == k :
                        if state not in all_list:
                            all_list.append(state.copy())
                        print(state)
                        state.pop()
                    else :
                        a = put_queen(state, n-1)
                        if (a == False and len(state)!=k and len(state)!=0):
                            state.pop()
    return False

def check_valid(current_state:list, next:list):
    row = next[0]
    column = next[1]
    for i in current_state :
        if row == i[0] : # check column
            return False
        if column == i[1]: # check row
            return False
        if abs(i[0]-row) == abs(i[1]-column): # check diagonal
            return False
    return True

def main( n ):
    my_state :list = []
    put_queen(my_state,n)
    print(len(all_list))
main(8)
