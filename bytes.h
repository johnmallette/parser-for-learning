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
        void advance_current(int);
        bool flag_fail;
        bool valid_input(char *);
        unsigned char char_to_hex(char);
        void set(std::vector<unsigned char>);
        std::vector<unsigned char> get(int64_t);

        friend std::ostream& operator<<(std::ostream&, Bytes&);

        friend Bytes& operator>>(Bytes&, Compactsize&);
        friend Bytes& operator>>(Bytes&, std::vector<Input>&);
        friend Bytes& operator>>(Bytes&, Input&);
        friend Bytes& operator>>(Bytes&, std::vector<Output>&);
        friend Bytes& operator>>(Bytes&, Output&);
        
        template<typename T>
        friend Bytes& operator>>(Bytes&, T&);
};

#include "input.h"
#include "output.h"

#endif
