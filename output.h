#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

struct Output{
    uint64_t amount;
    Bytes scriptpubkey;

    friend std::ostream& operator<<(std::ostream& lhs, Output& rhs){
        lhs << "\tamount:" << std::dec << rhs.amount << std::endl
            << "\tscriptpubkey:" << rhs.scriptpubkey;
        return lhs;
    }
};

#endif
