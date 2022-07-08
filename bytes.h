#ifndef BYTES_H
#define BYTES_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

struct Input;
struct Output;

#include "compactsize.h"

class Bytes{
    public:
        Bytes();
        Bytes(char *);
        bool fail(){return flag_fail;};
        uint32_t size(){return bytes.size();};
        
    private:
        std::vector<unsigned char> bytes;
        std::vector<unsigned char>::iterator current;
        bool flag_fail;
        bool valid_input(char *);
        unsigned char char_to_hex(char);
        void set(std::vector<unsigned char>);
        std::vector<unsigned char> get(int64_t);

        friend std::ostream& operator<<(std::ostream&, Bytes&);

        friend Bytes& operator>>(Bytes&, unsigned char&);
        friend Bytes& operator>>(Bytes&, uint16_t&);
        friend Bytes& operator>>(Bytes&, uint32_t&);
        friend Bytes& operator>>(Bytes&, uint64_t&);
        friend Bytes& operator>>(Bytes&, Compactsize&);
        friend Bytes& operator>>(Bytes&, Input&);
        friend Bytes& operator>>(Bytes&, Output&);
};

#include "input.h"
#include "output.h"

#endif
