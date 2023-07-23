//
// Created by Osayamen on 6/4/2023.
//

#ifndef MONOOS_RESIDENT_SET_H
#define MONOOS_RESIDENT_SET_H


#include <vector>

extern int WS;
struct Page{
    int page_no;
    std::vector<int> data;
};

void resident_set_init(int WS);
Page resident_set_check(int page_no);
void resident_set_write(int page_no, int offset, int data);
void display();


#endif //MONOOS_RESIDENT_SET_H
