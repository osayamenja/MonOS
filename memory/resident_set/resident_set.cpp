//
// Created by Osayamen on 6/4/2023.
//

#include <unordered_map>
#include <list>
#include "resident_set.h"

std::list<Page> pages;
std::unordered_map<int, std::list<Page>::iterator> references;

void resident_set_init(int ws){
    WS = ws;
    pages = std::list<Page>();
    references = std::unordered_map<int, std::list<Page>::iterator>();
}

Page check(int page_no){
    Page p = {
            {page_no}
    };
    if(references.count(page_no)){
        pages.erase(references.at(page_no));
    }
    else if(pages.size() == WS){
        pages.pop_back();
    }

    pages.push_front(p);
    references[page_no] = pages.begin();
    return p;
}