#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "linkedlist.h"
#include "scheduler.h"
#include "memory.h"

// function allocate arrived process into wait list
void add_to_wait_list(linkedlist_t* processes, linkedlist_t* wait_list, int current_time){
    node_t* current_process_node = processes->head;
    while (current_process_node != NULL) {
        process_t process = current_process_node->process;
        if (process.arrival_time <= current_time){
            insert(wait_list, process, wait_list->size);
            remove_at(processes, 0);
        }
        current_process_node = current_process_node->next;
    }
}

// function check wait list, if memory can be allocated -> put to ready
void add_to_ready_list(linkedlist_t* wait_list, linkedlist_t* ready_list, char* memory_strat, int* memory, int current_time){
    if (wait_list->size > 0) {
        node_t* current_process_node = wait_list->head;
        while (current_process_node != NULL){
            process_t process = current_process_node->process;
            if (strcmp(memory_strat, "best-fit") == 0){
                if(allocate_memory(&process, memory)){
                    insert(ready_list, process,ready_list->size);
                    remove_at(wait_list,0);
                    printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time, process.name, process.memory_alloc_index);
                }
            }
            if (strcmp(memory_strat, "infinite") == 0){
                insert(ready_list, process,ready_list->size);
                remove_at(wait_list,0);
            }
            current_process_node = current_process_node->next;
        }
    }
}

void print_statistics(int num_processes, int total_turnaround_time, double total_time_overhead, double max_time_overhead, int current_time){
    double avg_turnaround = (double) total_turnaround_time/num_processes;
    printf("Turnaround time %d\n", (int) ceil(avg_turnaround));

    double avg_time_overhead = (double) total_time_overhead/num_processes;
    printf("Time overhead %.2f %.2f\n", max_time_overhead, avg_time_overhead);
    printf("Makespan %d\n", current_time);
}



// scheduler
void do_sjf(linkedlist_t *processes, int quantum, char *memory_strat, int* memory){
    //list of process that is memory allocated and ready to run
    linkedlist_t *ready_list = create_list();
    // list of process that have arrived but not yet been memory allocated
    linkedlist_t *wait_list = create_list();

    int num_processes = processes->size;    
    int current_time = 0;   //simulation time
    int completed_processes = 0;

    // for task 5 statistic
    int total_turnaround_time = 0;
    double total_time_overhead = 0;
    double max_time_overhead = 0;


    while (completed_processes < num_processes){
        // append all of process into wait list if it is arrived
        add_to_wait_list(processes, wait_list, current_time);
        // append to ready list if memory allocation is sucessfull
        add_to_ready_list(wait_list, ready_list, memory_strat, memory, current_time);

        // if there is process ready -> perform/ else then time+= quantum
        if (ready_list->size > 0) {

            // simply go through each elem (O(n)) to find shortest task 
            int min_cpu_time = ready_list->head->process.cpu_time;
            int min_index = 0;

            node_t *current_process_node = ready_list->head->next;
            int current_index = 1;

            while (current_process_node != NULL) {
                if (current_process_node->process.cpu_time < min_cpu_time) {
                    min_cpu_time = current_process_node->process.cpu_time;
                    min_index = current_index;
                }
                // If there are two or more processes that have the same service time 
                // => consider faster arrival time, even same arrival time 
                // => first lexicographically.
                if (current_process_node->process.cpu_time == min_cpu_time) {
                    process_t *min_process = get_process(ready_list, min_index);
                    if(current_process_node->process.arrival_time == min_process->arrival_time){
                        if (strcmp(current_process_node->process.name, min_process->name) <= 0) { 
                            min_cpu_time = current_process_node->process.cpu_time;
                            min_index = current_index;
                        }
                    }
                }
                current_process_node = current_process_node->next;
                current_index++;
            }

            // run the founded shortest task
            process_t *min_process = get_process(ready_list, min_index);
            printf("%d,RUNNING,process_name=%s,remaining_time=%d\n",current_time,min_process->name, min_process->cpu_time);

            // run until it is finished
            int time_finish = current_time + min_process->cpu_time;
            while (current_time < time_finish){
                // for every quatum elapsed, check to see if other process is ready or to put in wait queue
                // append all of process into wait list if it is arrived
                add_to_wait_list(processes, wait_list, current_time);
                // append to ready list if memory allocation is sucessfull
                add_to_ready_list(wait_list, ready_list, memory_strat, memory, current_time);

                current_time += quantum;
            }
            completed_processes++;

            // record the statistic for task 5
            // turn around time
            min_process->time_turnaround = current_time - min_process->arrival_time;
            total_turnaround_time += min_process->time_turnaround;

            // time overhead
            min_process->time_overhead = (double) min_process->time_turnaround / min_process->cpu_time;
            total_time_overhead += min_process->time_overhead;
            if (min_process->time_overhead > max_time_overhead) max_time_overhead = min_process->time_overhead;
            
            // finishing process
            printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n",current_time,min_process->name, wait_list->size + ready_list->size-1);
            printf("%d,FINISHED-PROCESS,process_name=%s,sha=%s\n", current_time,min_process->name, min_process->name);
            free_memory(min_process, memory);
            remove_at(ready_list, min_index);
        }  
        else{
            current_time += quantum;
        }     
    }
    // calculating stats for task 5
    print_statistics(num_processes, total_turnaround_time, total_time_overhead, max_time_overhead, current_time);

}

void do_rr(linkedlist_t *processes, int quantum, char *memory_strat, int* memory){
    //list of process that is memory allocated and ready to run
    linkedlist_t *ready_list = create_list();
    // list of process that have arrived but not yet been memory allocated
    linkedlist_t *wait_list = create_list();

    int num_processes = processes->size;
    int current_time = 0;  //simulation time
    int completed_processes = 0;

    // for task 5 statistic
    int total_turnaround_time = 0;
    double total_time_overhead = 0;
    double max_time_overhead = 0;

    //used to record if the RUNNING message is printed
    int flag = 1;

    while (completed_processes < num_processes){
        // append all of process into wait list if it is arrived
        add_to_wait_list(processes, wait_list, current_time);
        // append to ready list if memory allocation is sucessfull
        add_to_ready_list(wait_list, ready_list, memory_strat, memory, current_time);
        
        // if there is process ready -> perform/ else then time+= quantum
        if(ready_list->size != 0) {
            process_t *current_process = get_process(ready_list, 0);
            if (flag){
                printf("%d,RUNNING,process_name=%s,remaining_time=%d\n",current_time,current_process->name, current_process->remaining_time);
                flag = 0;
            }

            // for every quatum elapsed, check to see if other process is ready or to put in wait queue
            // append all of process into wait list if it is arrived            
            current_time += quantum;
            add_to_wait_list(processes, wait_list, current_time);

            // append to ready list if memory allocation is sucessfull
            add_to_ready_list(wait_list, ready_list, memory_strat, memory, current_time);


            if (current_process->remaining_time > quantum){
                current_process->remaining_time -= quantum;
                // if there are other ready process -> suspend and move to last
                if (ready_list->size > 1){
                    move_to_end(ready_list);
                    flag = 1;
                }
            }
            else{
                current_process->remaining_time = 0;
                printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n",current_time,current_process->name, wait_list->size + ready_list->size-1);
                printf("%d,FINISHED-PROCESS,process_name=%s,sha=%s\n", current_time,current_process->name, current_process->name);
                free_memory(current_process, memory);
                completed_processes++;
                
                // record the statistic for task 5
                current_process->time_turnaround = current_time - current_process->arrival_time;
                current_process->time_overhead = (double) current_process->time_turnaround / current_process->cpu_time;

                total_turnaround_time += current_process->time_turnaround;
                total_time_overhead += current_process->time_overhead;

                if (max_time_overhead < current_process->time_overhead) max_time_overhead = current_process->time_overhead;
                remove_at(ready_list, 0);
                flag = 1;

            }
        }  
        else{
            current_time += quantum;
        }     
    }
    // calculating stats for task 5
    print_statistics(num_processes, total_turnaround_time, total_time_overhead, max_time_overhead, current_time);

}

