# include <iostream>
# include <vector>
# include <queue>
# include <functional>
# include <unordered_map>
using namespace std ;

struct taskTime {
    int index ;
    int excution ;
    int period ;
    int current ; // remain excution time
    pair<int,int> deadline ; // deadline or next period begin
    taskTime(int index,int excution,int period, int current, pair<int,int> deadline) : index(index),excution(excution), period(period),
        current(current),deadline(deadline) {}
    taskTime():index(-1){}
}; 

void addBigNumber( pair<int,int> & time ) {
    if ( time.first +1 == INT32_MAX ) {
        time.second ++ ;
        time.first = 0 ;
    }   
    else 
        time.first ++ ;
} // bigNumber

void nextPeriod( taskTime & readylist ) {
    if ( INT32_MAX - readylist.period <= readylist.deadline.first ) {
        readylist.deadline.first -= INT32_MAX ;
        readylist.deadline.second ++ ;
        readylist.deadline.first += readylist.period ;
    } // if
    else {
        readylist.deadline.first += readylist.period ;
    }

    readylist.current = readylist.excution ;
} //

pair<int,int> addBigNumberTaskTime( pair<int,int> time, int excutionTime ) {
    if ( INT32_MAX - excutionTime <= time.first ) {
        time.first -= INT32_MAX ;
        time.second ++ ;
        time.first += excutionTime ;
    } // if
    else {
        time.first += excutionTime ;
    }
    return time ;
} // addBigNumberTaskTime

int minusBigNumberTime( pair<int,int> time, pair<int,int> newtime ) {
    if ( time.second != newtime.second ) {
        return INT32_MAX - time.first + newtime.first ; 
    } // if
    return newtime.first - time.first ;
} // minusBigNumberTime

bool checkFinishCondition( vector<taskTime> & tasks, int size) { // check next_period time is the same
    if ( tasks.size() != size )
        return false ;
    for ( int i = 1 ; i < tasks.size() ; i ++ ) {
        if (tasks[i-1].deadline.first != tasks[i].deadline.first || tasks[i-1].deadline.second != tasks[i].deadline.second )
            return false ;
    } // for
    return true ;
} // checkFinishCondition

pair<int,int> findMinDeadline( vector<taskTime> readylist, pair<int,int> min ) {
    // pair<int,int> min = {INT32_MAX,INT32_MAX} ;
    for ( auto i : readylist ) {
        if ( i.deadline.second < min.second )
            min = i.deadline ;
        else if ( i.deadline.second == min.second && i.deadline.first < min.first ) {
            min = i.deadline ;
        }
    }

    return min ;
} // findLeastDeadline

auto deadline_compare = [](const taskTime& p1, const taskTime& p2) {
    if ( p1.deadline == p2.deadline  )
        return p1.index > p2.index ;

    return p1.deadline > p2.deadline;
};

auto rate_compare = [](const taskTime& p1, const taskTime& p2) {
    // float rate1 = static_cast<float>(p1.excution) / p1.period;
    // float rate2 = static_cast<float>(p2.excution) / p2.period;
    if ( float(p1.current)/p1.period == float(p2.current)/p2.period ) 
        return p1.index > p2.index ;

    return float(p1.current)/p1.period < float(p2.current)/p2.period ;
};

class Compare {
public:
    bool operator()(const taskTime& p1, const taskTime& p2) {
        return comp(p1, p2);
    }
    
    function<bool(const taskTime&, const taskTime&)> comp;
};

void Schedule( vector<vector<int> > task, int Display, int type ) {
    cout << "Scheduler Begin" << endl ;
    cout << task[0][1] << " " << task[1][1] << endl ;
    vector<taskTime> readylist ; // { index, excutionTime, period, current, deadline}
    // vector<taskTime> readylist_map ;
    taskTime currenttask ;
    taskTime outputtask ;
    priority_queue<taskTime, vector<taskTime>, Compare> pqueue;
    // priority_queue<vector<taskTime>, vector<vector<taskTime>>, Compare> pqueue;

    if ( type == 1 ) {
        pqueue = priority_queue<taskTime, vector<taskTime>, Compare>(Compare{deadline_compare});
    }
    else if ( type == 0 ) {
        // cout << "rate _ compare" ;
        pqueue = priority_queue<taskTime, vector<taskTime>, Compare>(Compare{rate_compare});
    }
    cout << "Success declare priority queue" << endl ;
    // priority queue
    bool start = true ;
    int preempted = 0 ;
    int  taskaction_output = -1, taskaction_current  = 0 ; // task action: start:0, end:1, preempted2, resume:3
    pair<int,int> time = {0,0}, outputtime = {0,0} ;
    pqueue.push(taskTime( 0, task[0][0], task[0][1], task[0][0], pair<int,int>(task[0][1],0 )) ) ;
    // push 1st task to queue
    cout << "push task to queue" << endl ;
    // erase from readylist 
    // pqueue.top().deadline.first
    // pqueue.top().deadline.second
    // currenttask
    // perform: excution -> finish time == time + excution
    // if ( time + excution > pqueue.top().deadline.first(second) )
    // -> preempted
    // else 
    // finish the task and take next task
    // checkFinishCondition(readylist) -> bool
    while( !checkFinishCondition(readylist, task.size()) || ! pqueue.empty() ) { // 
        // cout << "Time: " << time.first << " " << time.second << " Readylist length: " << readylist.size() << " pqueue: " << pqueue.size() << endl ;
        
        outputtime = time ;
        taskTime newtask ; 
        if ( pqueue.size() != 0) {
            newtask = pqueue.top() ; 
            pqueue.pop() ;
        }
        outputtask = currenttask ;
        currenttask = newtask ;
        if ( outputtask.index != -1 && outputtask.current == 0 )  
            taskaction_output = 1 ;
        else if ( outputtask.index != -1 && outputtask.current != 0 && currenttask.index != outputtask.index ) {
            taskaction_output = 2 ;
            preempted ++ ;
        }

        if ( currenttask.current != currenttask.excution )
            taskaction_current = 3 ;
        else if ( currenttask.current == currenttask.excution ) 
            taskaction_current = 0 ;

        pair<int,int> temptime = addBigNumberTaskTime( time, currenttask.current ) ;
        // cout << temptime.first << " test" << temptime.second << endl ;
        if ( currenttask.index == -1 ) {
            pair<int,int> minDeadline = findMinDeadline( readylist, {INT32_MAX,INT32_MAX} ) ;
            for ( int k = 0 ; k < readylist.size() ; k ++ ) {
                if ( minDeadline.first == readylist[k].deadline.first && minDeadline.second == readylist[k].deadline.second ) {
                    nextPeriod( readylist[k] ) ;
                    pqueue.push( readylist[k] ) ;
                    readylist.erase(readylist.begin()+k ) ;
                    k -- ;
                }
            } // for
            time = minDeadline ;
        } // if
        else if ( !readylist.empty()) { // check period condition
            pair<int,int> minDeadline = findMinDeadline( readylist, temptime ) ; // find time less than temptime
            // cout << "temptime " << temptime.first << " " << temptime.second << endl ; 
            // cout << "minDeadline " << minDeadline.first << " " << minDeadline.second << endl ; 
            for ( int k = 0 ; k < readylist.size() ; k ++ ) {
                if ( minDeadline.first == readylist[k].deadline.first && minDeadline.second == readylist[k].deadline.second ) {
                    nextPeriod( readylist[k] ) ;
                    pqueue.push( readylist[k] ) ;
                    readylist.erase(readylist.begin()+k ) ;
                    k -- ;
                }
            } // for
            currenttask.current = currenttask.current - minusBigNumberTime( time, minDeadline ) ; 
            time = minDeadline ;
                
            if ( currenttask.current == 0 ) {
                readylist.push_back( currenttask ) ;
            } // if
            else {
                pqueue.push( currenttask ) ;
            } // else
        } // if
        else {
            currenttask.current = 0 ;
            time = temptime ;
            readylist.push_back( currenttask ) ;
        } // else

        





        // taskTime newtask ; 
        // if ( pqueue.size() != 0) {
        //     newtask = pqueue.top() ; 
        //     pqueue.pop() ;
        // }
        // outputtask = currenttask ;
        // currenttask = newtask ;

        // if ( outputtask.index != -1 && outputtask.current == 0 )  
        //     taskaction_output = 1 ;
        // else if ( outputtask.index != -1 && outputtask.current != 0 && currenttask.index != outputtask.index ) {
        //     taskaction_output = 2 ;
        //     preempted ++ ;
        // }

        // if ( currenttask.current != currenttask.excution )
        //     taskaction_current = 3 ;
        // else if ( currenttask.current == currenttask.excution ) 
        //     taskaction_current = 0 ;
        // // do task
        // currenttask.current -- ;
        // if ( currenttask.current > 0 ) {
        //     pqueue.push( currenttask ) ;
        // } // if
        // else if ( currenttask.current == 0 ) {
        //     int tempindex = currenttask.excution ;
        //     readylist.push_back( currenttask ) ;
        // }

        if ( outputtime.first == 0 && outputtime.second == 0 ) { 
            for ( int i = 1 ; i < task.size() ; i ++ ) {
                pqueue.push(taskTime( i, task[i][0], task[i][1], task[i][0], pair<int,int>(task[i][1],0 )) ) ;
            } // for
        } // if

        // if time == deadline
        // if ( !readylist.empty()) { // check period condition
        //     for ( int k = 0 ; k < readylist.size() ; k ++ ) {
        //         if ( time.first >= readylist[k].deadline.first && time.second >= readylist[k].deadline.second ) {
        //             nextPeriod( readylist[k] ) ;
        //             pqueue.push( readylist[k] ) ;
        //             readylist.erase(readylist.begin()+k ) ;
        //             k -- ;
        //         }
        //     }
        // } // else if
        // push to priority queue :
        //      if task not done push into queue
        //      if the task done -> push into readylist
        //      or if period of task arrived then push into queue
        //      ( from readylist )

        // output
        if ( Display == 1 ) {
            if ( outputtask.index != currenttask.index ) { // idle, no event
                cout << outputtime.second << " + " << outputtime.first << " " ;
                if ( outputtask.index != -1) {
                    cout << outputtask.index+1 << " " << taskaction_output << " " ;
                } // if
                cout << currenttask.index+1 << " " << taskaction_current << endl ;
            }
        } // if

        // addBigNumber(time) ;
        // cout << "Time: " << time.first << " Readylist length: " << readylist.size() << " pqueue: " << pqueue.size() << endl ;
    } // while
    cout << time.second << " + " << time.first << " " << currenttask.index+1 << " " << "1" << endl ;
    cout << " Total Time " << time.first+1 << "+ " << time.second << " Preempted " << preempted << " Finished" << endl ;
} //

// calculate deadline
// big number
// next period

int main(int argc, char const *argv[])
{

    int R ;
    int S, D, N ;
    vector<vector<int> > task ;
    cout << "Scheduling Begin" << endl ;
    cin >> R ;
    cout << "Total " << R << "runs." << endl ;
    for ( int run = 0 ; run < R ; run ++ ) {
        cout << run+1 << endl ;
        cin >> S >> D >> N ;
        for ( int i = 0 ; i < N ; i ++ ) {
            int value1,value2 ;
            cin >> value1 >> value2 ;
            task.push_back({value1,value2}) ;
        } // for
        Schedule( task, D, S ) ;



        task.clear() ;
    } // for




    // file.close() ;
    return 0;
}
