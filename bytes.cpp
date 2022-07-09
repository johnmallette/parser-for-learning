#include "bytes.h"

Bytes::Bytes(){
    flag_fail = false;
    num_of_bytes = 0;
    current = bytes.begin();
}

Bytes::Bytes(char *arg){
    flag_fail = false;

    if(!valid_input(arg)){
        flag_fail = true;
        return;
    }

    for(int i = 0; arg[i] != '\0'; i += 2){
        unsigned char uc = 0x00;
        uc ^= char_to_hex(arg[i]);
        uc <<= 4;
        uc ^= char_to_hex(arg[i + 1]);
        bytes.push_back(uc);
    }

    current = bytes.begin();
}

Bytes::Bytes(int64_t num){
    flag_fail = false;
    num_of_bytes = num;
    current = bytes.begin();
}

void Bytes::advance_current(int num){
    if(num == 0) return;
    if(current == bytes.end()) flag_fail = true;
    if(flag_fail) return;

    current++;
    advance_current(num-1);

    return;
}

bool Bytes::valid_input(char *arg){
    int size = 0;
    
    while(arg[size] != '\0'){
        char c = arg[size];
        if(!((c >= '0' && c <= '9') || 
             (c >= 'a' && c <= 'f') ||
             (c >= 'A' && c <= 'F')))
            return false;
        size++;
    }
    if(size % 2) return false;
    return true;
}

unsigned char Bytes::char_to_hex(char c){
    unsigned char uc = 0x00;
    if(c > 70) c -= 32;
    c = (c < 58) ? c - 48 : c - 55;
    uc = 0x0F & c;
    return uc;
}

std::ostream& operator<<(std::ostream& lhs, Bytes &rhs){
    for(int i = 0; i < rhs.bytes.size(); i++)
        lhs << std::hex << std::setw(2) << std::setfill('0') << (int)rhs.bytes[i];
    return lhs;
}


template<typename T>
Bytes& operator>>(Bytes& lhs, T& rhs){
    rhs = 0;

    std::vector<unsigned char>::iterator stop = lhs.current;
    lhs.advance_current(sizeof(T));

    for(std::vector<unsigned char>::iterator it = lhs.current; it != stop; it--){
        rhs <<= 8;
        rhs ^= *(it - 1);
    }

    return lhs;
}

Bytes& operator>>(Bytes& lhs, Bytes& rhs){
    bool little = false;

    if(rhs.num_of_bytes < 0){
        rhs.num_of_bytes = -rhs.num_of_bytes;
        little = true;
    }

    std::vector<unsigned char>::iterator start = lhs.current;

    lhs.advance_current(rhs.num_of_bytes);

    while(start != lhs.current){
        rhs.bytes.push_back(*start);
        start++;
    }

    if(little) std::reverse(rhs.bytes.begin(), rhs.bytes.end());

    return lhs;
}

Bytes& operator>>(Bytes& lhs, Compactsize& rhs){
    rhs.value = 0x0000000000000000;
    unsigned char byte = 0x00;

    lhs >> byte;

    switch(byte){
        case 0xfd:
            uint16_t two;
            lhs >> two;
            rhs.value = (uint64_t)two;
            break;
        case 0xfe:
            uint32_t four;
            lhs >> four;
            rhs.value = (uint64_t)four;
            break;
        case 0xff:
            lhs >> rhs.value;
            break;
        default:
            rhs.value = (uint64_t)byte;
    }
 
    return lhs;
}

Bytes& operator>>(Bytes& lhs, std::vector<Input>& rhs){
    Compactsize num_of_inputs;

    lhs >> num_of_inputs;

    for(uint64_t i = 0; i != num_of_inputs.value; i++){
        Input next;
        lhs >> next;
        rhs.push_back(next);
    }

    return lhs;
}

Bytes& operator>>(Bytes& lhs, Input& rhs){
    //scriptsize larger than or equal to 2**63 breaks this
    //figured that would be cool since one MB blocks are still
    //a thing
    Compactsize scriptsig_size;
    
    lhs >> rhs.id >> rhs.index >> scriptsig_size;
    rhs.scriptsig.num_of_bytes = (int64_t)scriptsig_size.value;
    lhs >> rhs.scriptsig >> rhs.sequence;

    return lhs;
}

Bytes& operator>>(Bytes& lhs, std::vector<Output>& rhs){
    Compactsize num_of_outputs;

    lhs >> num_of_outputs;

    for(uint64_t i = 0; i != num_of_outputs.value; i++){
        Output next;
        lhs >> next;
        rhs.push_back(next);
    }

    return lhs;
}

Bytes& operator>>(Bytes& lhs, Output& rhs){
    Compactsize scriptpubkey_size;

    lhs >> rhs.amount >> scriptpubkey_size;
    rhs.scriptpubkey.num_of_bytes = (int64_t)scriptpubkey_size.value;
    lhs >> rhs.scriptpubkey;

    return lhs;
}
