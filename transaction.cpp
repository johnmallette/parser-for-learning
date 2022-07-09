#include "transaction.h"

Transaction::Transaction(char *arg){
    raw = new Bytes(arg); 
    
    if(fail())return;

    *raw >> version >> inputs >> outputs >> locktime; 
}

Transaction::~Transaction(){
    delete raw;
}

std::ostream& operator<<(std::ostream& lhs, Transaction& rhs){
    lhs << "version:" << std::dec << rhs.version << std::endl
        << "inputs:" << rhs.inputs.size() << std::endl;

    for(std::vector<Input>::iterator it = rhs.inputs.begin(); it != rhs.inputs.end(); it++){
        lhs << *it << '\n' << std::endl;
    }
    
    lhs << "outputs:" << rhs.outputs.size() << std::endl;

    for(std::vector<Output>::iterator it = rhs.outputs.begin(); it != rhs.outputs.end(); it++){
        lhs << *it << '\n' << std::endl;
    }

    lhs << "locktime:" << rhs.locktime;
    
    return lhs;
}
