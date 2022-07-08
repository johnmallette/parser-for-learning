#ifndef INPUT_H
#define INPUT_H

#include <iostream>

struct Input{
    Bytes id;
    uint32_t index;
    Bytes scriptsig;
    uint32_t sequence;

    friend std::ostream& operator<<(std::ostream& lhs, Input& rhs){
        lhs << "\ttxid:" << rhs.id << std::endl 
            << "\tindex:" << std::dec <<  rhs.index << std::endl
            << "\tscriptSig:" << rhs.scriptsig <<  std::endl
            << "\tsequence:" << rhs.sequence;
        return lhs;
    }
};

#endif
