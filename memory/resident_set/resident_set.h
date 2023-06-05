//
// Created by Osayamen on 6/4/2023.
//

#ifndef MONOOS_RESIDENT_SET_H
#define MONOOS_RESIDENT_SET_H


#include <vector>

extern int WS;
struct Page{
    std::vector<int> data;
};

void resident_set_init(int WS);
Page check(int page_no);
void display();


#endif //MONOOS_RESIDENT_SET_H
