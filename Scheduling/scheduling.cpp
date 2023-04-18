# include <iostream>
# include <vector>
# include <queue>
# include <functional>
# include <chrono>
using namespace std ;

struct bigInteger{
    int first ;
    int second ;
    bigInteger() : first(0), second(0) {}
    bigInteger(int first, int second) : first(first), second(second) {}
} ;

struct taskTime {
    int index ;
    int excution ;
    int period ;
    int current ; // remain excution time
    bigInteger deadline ; // deadline or next period begin
    taskTime(int index,int excution,int period, int current, bigInteger deadline) : index(index),excution(excution), period(period),
        current(current),deadline(deadline) {}
    taskTime():index(-1){}
}; 

int bigIntegerCompare( bigInteger a, bigInteger b) {
    if ( a.first == b.first && a.second == b.second )
        return 0 ;
    else if ( a.second > b.second || ( a.second == b.second && a.first > b.first ) ) {
        return 1 ;
    }
    else {
        return -1 ;
    }
}
 
void nextPeriod( taskTime & readylist ) {
    if ( INT32_MAX - readylist.period < readylist.deadline.first ) {
        readylist.deadline.first -= INT32_MAX ;
        readylist.deadline.second ++ ;
    } // if

    readylist.deadline.first += readylist.period ;

    readylist.current = readylist.excution ;
} //

bigInteger addBigNumberTaskTime( bigInteger time, int & excutionTime ) {
    if ( INT32_MAX - excutionTime < time.first ) {
        time.first -= INT32_MAX ;
        time.second ++ ;
    } // if
    time.first += excutionTime ;

    return time ;
} // addBigNumberTaskTime

int minusBigNumberTime( bigInteger & time, bigInteger & newtime ) {
    if ( time.second != newtime.second ) {
        return INT32_MAX - time.first + newtime.first ; 
    } // if
    return newtime.first - time.first ;
} // minusBigNumberTime

bool checkFinishCondition( vector<taskTime> & tasks, int size) { // check next_period time is the same
    if ( tasks.size() != size )
        return false ;
    for ( int i = 1 ; i < size ; i ++ ) {
        if ( bigIntegerCompare(tasks[i-1].deadline,tasks[i].deadline) != 0  )
            return false ;
    } // for

    return true ;
} // checkFinishCondition

bigInteger findMinDeadline( vector<taskTime> & readylist, bigInteger min ) {
    for ( auto i : readylist ) {
        if ( i.deadline.second < min.second )
            min = i.deadline ;
        else if ( i.deadline.second == min.second && i.deadline.first < min.first ) {
            min = i.deadline ;
        }
        else
            break ;
    }

    return min ;
} // findLeastDeadline

auto deadline_compare = [](const taskTime& p1, const taskTime& p2) {
    if ( bigIntegerCompare(p1.deadline,p2.deadline)  == 0 ) 
        return p1.period < p2.period ;
    else {
        if ( p1.deadline.second != p2.deadline.second )
            return p1.deadline.second > p2.deadline.second ;
        else  
            return p1.deadline.first > p2.deadline.first ;
    } // else
};

auto rate_compare = [](const taskTime& p1, const taskTime& p2) {
    if ( p1.current*p2.period == p2.current*p1.period ) 
        return p1.index > p2.index ;

    return p1.current*p2.period < p2.current*p1.period ;
};

auto deadline = [](const taskTime& p1, const taskTime& p2) {
    if ( p1.deadline.second != p2.deadline.second )
        return p1.deadline.second < p2.deadline.second ;
    else  
        return p1.deadline.first < p2.deadline.first ;
};

class Compare {
public:
    bool operator()(const taskTime& p1, const taskTime& p2) {
        return comp(p1, p2);
    }
    
    function<bool(const taskTime&, const taskTime&)> comp;
};

void Schedule( vector<vector<int> > & task, int & Display, int & type, int & taskSize ) {
    // cout << "Scheduler Begin" << endl ;
    bool available = true ;
    double availableUtilize = 0.0 ;
    for ( auto i : task ) {
        availableUtilize += double(i[0])/i[1] ;
    }
    if ( availableUtilize > 1 ) {
        cout << 0 << " " << 0 << endl ;
        return ;
    }
    vector<taskTime> readylist ; // { index, excutionTime, period, current, deadline}
    taskTime task_current ;
    taskTime task_pre ;
    priority_queue<taskTime, vector<taskTime>, Compare> pqueue;

    if ( type == 1 ) {
        pqueue = priority_queue<taskTime, vector<taskTime>, Compare>(Compare{deadline_compare});
    }
    else if ( type == 0 ) {
        // cout << "rate _ compare" ;
        pqueue = priority_queue<taskTime, vector<taskTime>, Compare>(Compare{rate_compare});
    }
    // priority queue
    bool start = true ;
    int preempted = 0 ;
    int  taskaction_pre = -1, taskaction_current  = 0 ; // task action: start:0, end:1, preempted2, resume:3
    // bigInteger time = {0,0}, outputtime = {0,0} ;
    bigInteger time = bigInteger(0,0), outputtime = bigInteger(0,0) ;
    pqueue.push(taskTime( 0, task[0][0], task[0][1], task[0][0], bigInteger(task[0][1],0 )) ) ;
    // push 1st task to queue

    while( !checkFinishCondition(readylist, taskSize) || ! pqueue.empty() ) { // 
        // cout << "Time: " << time.first << " " << time.second << " Readylist length: " << readylist.size() << " pqueue: " << pqueue.size() << endl ;
        
        outputtime = time ;
        taskTime newtask ; 
        if ( pqueue.size() != 0) {
            newtask = pqueue.top() ; 
            pqueue.pop() ;
        }
        task_pre = task_current ;
        task_current = newtask ;
        if ( task_pre.index != -1 && task_pre.current == 0 )  
            taskaction_pre = 1 ;
        else if ( task_pre.index != -1 && task_pre.current != 0 && task_current.index != task_pre.index ) {
            taskaction_pre = 2 ;
            preempted ++ ;
        }

        if ( task_current.current != task_current.excution )
            taskaction_current = 3 ;
        else if ( task_current.current == task_current.excution ) 
            taskaction_current = 0 ;

        if ( task_current.index == -1 ) {
            sort(readylist.begin(), readylist.end(), deadline) ;
            time = readylist[0].deadline ;
            nextPeriod( readylist[0] ) ;
            pqueue.push( readylist[0] ) ;
            readylist.erase(readylist.begin()+0 ) ;
        } // if
        else if ( !readylist.empty()) { // check period condition
            bigInteger temptime = addBigNumberTaskTime( time, task_current.current ) ;
            sort(readylist.begin(), readylist.end(), deadline) ;
            bigInteger minDeadline ; // = findMinDeadline( readylist, temptime ) ; // find time less than temptime

            if ( bigIntegerCompare( temptime, readylist[0].deadline) == -1 ) {
                minDeadline = temptime ;
            }
            else {
                minDeadline = readylist[0].deadline ;
            }
            for ( int k = 0 ; k < readylist.size() ; k ++ ) {
                if ( bigIntegerCompare(minDeadline,readylist[k].deadline) == 0  ) {
                    nextPeriod( readylist[k] ) ;
                    pqueue.push( readylist[k] ) ;
                    readylist.erase(readylist.begin()+k ) ;
                    k -- ;
                }
                else
                    break ;
            } // for
            task_current.current = task_current.current - minusBigNumberTime( time, minDeadline ) ; 
            time = minDeadline ;
                
            if ( task_current.current == 0 ) {
                readylist.push_back( task_current ) ;
            } // if
            else {
                pqueue.push( task_current ) ;
            } // else
        } // if
        else {
            
            time = addBigNumberTaskTime( time, task_current.current ) ;
            task_current.current = 0 ;
            readylist.push_back( task_current ) ;
        } // else

        

        if ( outputtime.first == 0 && outputtime.second == 0 ) { 
            for ( int i = 1 ; i < taskSize ; i ++ ) {
                pqueue.push(taskTime( i, task[i][0], task[i][1], task[i][0], bigInteger(task[i][1],0 )) ) ;
            } // for
        } // if

        // output
        if ( Display == 1 ) {
            if ( task_pre.index != task_current.index ) { // idle, no event
                cout << long(outputtime.second)*INT32_MAX + outputtime.first << " " ;
                if ( task_pre.index != -1) {
                    cout << task_pre.index+1 << " " << taskaction_pre << " " ;
                } // if
                cout << task_current.index+1 << " " << taskaction_current << endl ;
            }
        } // if

    } // while
    if ( Display == 1)
        cout << (long long)(time.second)*INT32_MAX + time.first << " " << task_current.index+1 << " " << "1" << endl ;
    time = findMinDeadline( readylist, {INT32_MAX,INT32_MAX} ) ;
    cout << (long long)(time.second)*INT32_MAX + time.first << " " << preempted <<  endl ;
} // Schedule


int main(int argc, char const *argv[])
{

    int R ;
    int S, D, N ;
    vector<vector<int> > task ;
    // cout << "Scheduling Begin" << endl ;
    cin >> R ;
    for ( int run = 0 ; run < R ; run ++ ) {
        cout << run+1 << endl ;
        cin >> S >> D >> N ;
        for ( int i = 0 ; i < N ; i ++ ) {
            int value1,value2 ;
            cin >> value1 >> value2 ;
            task.push_back({value1,value2}) ;
        } // for
        auto t3 = std::chrono::high_resolution_clock::now();
        Schedule( task, D, S, N ) ;
        auto t4 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration3 = t4 - t3;
        cout << "Time: " << duration3.count() << endl ;


        task.clear() ;
    } // for

    return 0;
}
