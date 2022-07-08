#include "bytes.h"

Bytes::Bytes(){
    flag_fail = false;
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

void Bytes::set(std::vector<unsigned char> arg){
    bytes = arg;
    current = bytes.begin();
}

std::vector<unsigned char> Bytes::get(int64_t num){
    bool little = false;
    if(num < 0){
        num = -num;
        little = true;
    }

    std::vector<unsigned char> bye_bytes;
    std::vector<unsigned char>::iterator stop = current + num;
    
    while(current != stop){
        bye_bytes.push_back(*current);
        current++;
    }

    if(little) std::reverse(bye_bytes.begin(), bye_bytes.end());

    return bye_bytes;
}


std::ostream& operator<<(std::ostream& lhs, Bytes &rhs){
    for(int i = 0; i < rhs.bytes.size(); i++)
        lhs << std::hex << std::setw(2) << std::setfill('0') << (int)rhs.bytes[i];
    return lhs;
}

Bytes& operator>>(Bytes& lhs, unsigned char& rhs){
    rhs = 0x00;
    rhs = *lhs.current;
    lhs.current++;
    return lhs;
}

Bytes& operator>>(Bytes& lhs, uint16_t& rhs){
    rhs = 0x0000;

    for(int i = 2; i; i--){
        rhs <<= 8;
        rhs ^= *(lhs.current + (i-1));
    }

    lhs.current += 2;
    return lhs;
}

Bytes& operator>>(Bytes& lhs, uint32_t& rhs){
    rhs = 0x00000000;

    for(int i = 4; i; i--){
        rhs <<= 8;
        rhs ^= *(lhs.current + (i-1));
    }

    lhs.current += 4;
    return lhs;
}

Bytes& operator>>(Bytes& lhs, uint64_t& rhs){
    rhs = 0x0000000000000000;

    for(int i = 8; i; i--){
        rhs <<= 8;
        rhs ^= *(lhs.current + (i-1));
    }

    lhs.current += 8;
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

Bytes& operator>>(Bytes& lhs, Input& rhs){
    //scriptsize larger than or equal to 2**63 breaks this
    //figured that would be cool since one MB blocks are still
    //a thing
    Compactsize scriptsig_size;
    rhs.id.set(lhs.get(-32));
    lhs >> rhs.index >> scriptsig_size;
    rhs.scriptsig.set(lhs.get((int64_t)scriptsig_size.value));
    lhs >> rhs.sequence;
    return lhs;
}

Bytes& operator>>(Bytes& lhs, Output& rhs){
    Compactsize scriptpubkey_size;
    lhs >> rhs.amount >> scriptpubkey_size;
    rhs.scriptpubkey.set(lhs.get((int64_t)scriptpubkey_size.value));
    return lhs;
}