#include "transaction.h"

Transaction::Transaction(char *arg){
    flag_fail = false;
    raw = new Bytes(arg); 
    
    if(raw->fail()){
        flag_fail = true;
        return;
    }

    *raw >> version >> num_of_inputs; 
    
    for(uint64_t i = 0; i != num_of_inputs.value; i++){
        Input next;
        *raw >> next;
        inputs.push_back(next);
    }

    *raw >> num_of_outputs;

    for(uint64_t i = 0; i != num_of_outputs.value; i++){
        Output next;
        *raw >> next;
        outputs.push_back(next);
    }

    *raw >> locktime;
}

Transaction::~Transaction(){
    delete raw;
}

std::ostream& operator<<(std::ostream& lhs, Transaction& rhs){
    lhs << "version:" << std::dec << rhs.version << std::endl
        << "inputs:" << rhs.num_of_inputs.value << std::endl;

    for(std::vector<Input>::iterator it = rhs.inputs.begin(); it != rhs.inputs.end(); it++){
        lhs << *it << '\n' << std::endl;
    }
    
    lhs << "outputs:" << rhs.num_of_outputs.value << std::endl;

    for(std::vector<Output>::iterator it = rhs.outputs.begin(); it != rhs.outputs.end(); it++){
        lhs << *it << '\n' << std::endl;
    }

    lhs << "locktime:" << rhs.locktime;
    
    return lhs;
}
