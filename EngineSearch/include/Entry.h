//
// Created by user on 13.02.2024.
//

#ifndef ENGINESEARCH_ENTRY_H
#define ENGINESEARCH_ENTRY_H
#pragma once
#include <iostream>

struct Entry {
    size_t doc_id, count;

    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    };
};

#endif //ENGINESEARCH_ENTRY_H
