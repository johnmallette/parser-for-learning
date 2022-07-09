#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "bytes.h"

class Transaction{
    public:
        Transaction(char *);
        ~Transaction();
        bool fail(){return raw->fail();};
    private:
        Bytes *raw;
        uint32_t version;
        uint32_t locktime;
        std::vector<Input> inputs;
        std::vector<Output> outputs;

        friend std::ostream& operator<<(std::ostream&, Transaction&);
};

#endif
