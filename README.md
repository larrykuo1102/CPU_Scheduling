# Implement RTOS CPU scheduling
## **AOS Homework** 

[smaple input](./Scheduling/sample/input1)

[sample output](./Scheduling/sample/output1)

[My output](./Scheduling/output.txt)

## Problem
```
Rate-monotonic (RM) scheduling and earliest-deadline-first (EDF) scheduling algorithms are optimal in real-time scheduling with fixed and dynamic priority assignment, respectively. However, we may need to use one of them once in a while but not mixed so that a program can execute conveniently. You might need to consider that the scheduler has to run forever and as efficient as possible.
```

##  Project
```
You are to design an efficient program for the two algorithms generating complete schedules (up to a hyperperiod, least common multiple (LCM) of periods but better not use the concept of LCM) for task sets according to the following input format from stdin and output format to stdout. All the inputs and outputs are non-negative integers separated by a space WITHOUT the comment except the real current times and elapse times are in floating numbers in seconds up to the accuracy of nanoseconds. Note that if two tasks have the same priority, the one with smaller task id gets higher priority.
```
## Program input


* The program will get the input parameters from the standard input
    * R - the number of runs > 0
    * S - the scheduling policy, 0: RM (the shortest period gets higher priority), 1:EDF (the earliest deadline gets higher priority)
    * D - display schedule, 1, or not, 0
    * N - 0 < number of tasks ? 128, tasks are numbered starting from 1
    * T1 P1 - execution time, period, for the 1st task, periods are NOT sorted
    * T2 P2 - execution time, period, for the 2nd task
    * …
    * Tn Pn - execution time, period, for the nth task
    * For example, an input of 2 runs of task sets might look like this:

```
2			// 2 runs
1			// the 1st run, EDF
1			// display schedule
2			// 2 tasks
1 2			// (execution time 1) (period 2), for the 1st task
2 6			// (execution time 2) (period 6), for the 2nd task
0			// the 2nd run, RMS
0			// display schedule
2			// 2 tasks			
1 2                     // execution time 1, period 2, for the 1st task
1 3			// execution time 1, period 3, for the 2nd task 
```

## Program Output
* The program will generate results to the standard output:
    * RUN

        * RUN - run number
    * TIME [TASK ACTION] … - event order matters, time does not repeat, idle time units may disappear.

        * TIME - simulated time in integers
        * TASK - task ID
        * ACTION - start, 0, end, 1, preempted, 2, resume, 3
    * END PREEMPTED

        * END - end simulated time, or 0, if the task set is not schedulable
        * PREEMPTED - the times of preemption

```
1                       // start the 1st run
0 1 0		        // at time 0, task 1 starts;
1 1 1 2 0		// at time 1, task 1 ends, then task 2 starts; 
2 2 2 1 0		// at time 2, task 2 is preempted, then task 1 starts; 
3 1 1 2 3		// at time 3, task 1 ends, then task 2 resumes;
4 2 1 1 0		// at time 4, task 2 ends, then task 1 starts;
5 1 1		        // at time 5, task 1 ends;
6 1			// at time 6, end of the 1st run; preempted 1 time
2                	// start the 2nd run
6 0			// at time 6, end of the 2nd schedule; preempted 0 time
```