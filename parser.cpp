#include <iostream>
#include "transaction.h"

int main(int argc, char *argv[]){
   Transaction tx(argv[1]); 
  
   if(tx.fail()){
       std::cout << "PARSING FAILED :)" << std::endl;
       return 1;
   }

   std::cout << tx << std::endl;

   return 0;
}
