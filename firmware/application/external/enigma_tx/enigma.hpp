/*
    Created by Lucas Moidel (https://github.com/Lucasmoidel)
*/
#ifndef ENIGMA_HEADER_
#define ENIGMA_HEADER_

namespace enigma {    
    std::string toLower(std::string str);
    int charToInt(std::string letter);
    std::string intToChar(int num);
    std::string enigma(std::string reflector, std::string r1, std::string r2, std::string r3, std::string offset1, std::string offset2, std::string offset3, int plugs[], std::string  message);
    int mod(int a, int b);
    int index(int arr[], int size, int x);
    

}

#endif