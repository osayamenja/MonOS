//
// Created by Osayamen on 6/4/2023.
//

#include <unordered_map>
#include <list>
#include "resident_set.h"
#include "../memory.h"
#include "../loader/load.h"

std::list<Page> pages;
std::unordered_map<int, std::list<Page>::iterator> references;

void resident_set_init(int ws){
    WS = ws;
    pages = std::list<Page>();
    references = std::unordered_map<int, std::list<Page>::iterator>();
}

Page resident_set_check(int page_no){
    Page p = {};

    if(references.count(page_no)){
        p = references.at(page_no).operator*();
        pages.erase(references.at(page_no));
    }
    else{
        int physical_address = get_physical_address(page_no, 0);
        p.data = std::vector<int>();
        int page_book_end = physical_address + pageSize - 1;

        for(int i = physical_address; i < page_book_end; ++i)
            p.data.at(i - physical_address) = Mem.at(i);
    }

    if(pages.size() == WS){
        pages.pop_back();
    }

    pages.push_front(p);
    references[page_no] = pages.begin();
    return p;
}

void resident_set_write(int page_no, int offset, int data){
    // write to memory
    Mem.at(get_physical_address(page_no, offset)) = data;
    resident_set_check(page_no);
}