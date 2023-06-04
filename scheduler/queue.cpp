//
// Created by Osayamen on 2/25/2023.
//

#include <deque>
#include <iomanip>
#include <set>
#include <vector>
#include <sstream>

#include "../computer/computer.h"
#include "../utility/utility.h"

//Multi-level feedback queue
std::vector<std::deque<PCB>> feedbackQueues;
std::set<int> queue_pointers; // data structure providing efficient (O(lgn)) retrieval of the next nonempty queue.
int queue_levels;

void queue_init(int levels){
    feedbackQueues = std::vector<std::deque<PCB>>(levels);
    queue_levels = levels;
    for(auto & feedbackQueue : feedbackQueues){
        feedbackQueue = std::deque<PCB>();
    }
    queue_pointers = std::set<int>();
}

void queue_insert(PCB p){ // add to queue with the highest priority.
    if(p.finishedTQ && p.priority < (queue_levels - 1)){
        p.priority++;
    }

    feedbackQueues.at(p.priority).push_back(p);

    // Set elements are unique, so this operation is correct and perhaps even more efficient
    // than invoking count(p.priority) to detect uniqueness prior to insertion.
    queue_pointers.insert(p.priority);
}

PCB queue_fetch(){
    if(queue_pointers.empty()) return NULL_PCB;
    int q_ptr = queue_pointers.begin().operator*();

    PCB temp = feedbackQueues.at(q_ptr).front();
    feedbackQueues.at(q_ptr).pop_front();

    if(feedbackQueues.at(q_ptr).empty()) queue_pointers.erase(q_ptr);
    return temp;
}

void queue_dump(){
    std::stringstream dump_stream;
    dump_stream << generate_header();
    dump_stream << std::setw(17) <<"ReadyQ Dump" << std::endl;
    dump_stream << generate_header();

    for(int i = 0; i < queue_levels; ++i){
        std::deque<PCB> q = feedbackQueues.at(i);

        dump_stream << "Q" << i << "->Index: Process ID" << std::endl;
        int j = 0;
        for(const auto& p: q){
            dump_stream << j << ": " << p.PID << std::endl;
            j++;
        }
    }

    std::string  dump_str = dump_stream.str();
    atomically_print_to_stdout(dump_str);
}
