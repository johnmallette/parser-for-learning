#ifndef INPUT_H
#define INPUT_H

#include <iostream>

struct Input{
    Bytes id{(int64_t)-32};
    uint32_t index;
    Bytes scriptsig;
    uint32_t sequence;

    friend std::ostream& operator<<(std::ostream& lhs, Input& rhs){
        lhs << "  txid:" << rhs.id << std::endl 
            << "  index:" << std::dec <<  rhs.index << std::endl
            << "  scriptsig:" << rhs.scriptsig <<  std::endl
            << "  sequence:" << rhs.sequence;
        return lhs;
    }
};

#endif
