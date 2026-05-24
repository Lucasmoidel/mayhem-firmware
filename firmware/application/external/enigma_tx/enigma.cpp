/*
    Created by Lucas Moidel (https://github.com/Lucasmoidel)
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>    
#include "enigma.hpp"

std::string toLower(std::string str){
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c);});
    return str;
}

int charToInt(std::string letter){
    std::string letters[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    for (int i = 0; i < 26; i++){
        if(letters[i] == toLower(letter)){
            return i;
        }
    }
    return -1;
}

std::string intToChar(int num){
    std::string letters[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    return letters[num];
}

std::string enigma(std::string reflector, std::string r1, std::string r2, std::string r3, int offset1, int offset2, int offset3, int plugs[], std::string  message){
    int rotors[4][26];
    int offsets[3] = {charToInt("a"), charToInt("a"), charToInt("a")};
    int notch[3];

    int rotor1[26] = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9};
    int rotor2[26] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
    int rotor3[26] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};

    int reflectora[26] = {4, 9, 12, 25, 0, 11, 24, 23, 21, 1, 22, 5, 2, 17, 16, 20, 14, 13, 19, 18, 15, 8, 10, 7, 6, 3};
    int reflectorb[26] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19};
    int reflectorc[26] = {5, 21, 15, 9, 8, 0, 14, 24, 4, 3, 17, 25, 23, 22, 6, 2, 19, 10, 20, 16, 18, 1, 13, 12, 7, 11};

    

    if (toLower(reflector) == "a"){
        for (int i = 0; i < 26; i++){
            rotors[0][i] = reflectora[i];
        }
    } else if (toLower(reflector) == "b"){
        for (int i = 0; i < 26; i++){
            rotors[0][i] = reflectorb[i];
        }
    } else if (toLower(reflector) == "c"){
        for (int i = 0; i < 26; i++){
            rotors[0][i] = reflectorc[i];
        }
    }

    if (toLower(r1) == "1"){
        for (int i = 0; i < 26; i++){
            rotors[1][i] = rotor1[i];
        }
        notch[0] = charToInt("r");
    } else if (toLower(r1) == "2"){
        for (int i = 0; i < 26; i++){
            rotors[1][i] = rotor2[i];
        }      
        notch[0] = charToInt("f");
    } else if (toLower(r1) == "3"){
        for (int i = 0; i < 26; i++){
            rotors[1][i] = rotor3[i];
        }
        notch[0] = charToInt("w");
    }

    if (toLower(r2) == "1"){
        for (int i = 0; i < 26; i++){
            rotors[2][i] = rotor1[i];
        }
        notch[1] = charToInt("r");
    } else if (toLower(r2) == "2"){
        for (int i = 0; i < 26; i++){
            rotors[2][i] = rotor2[i];
        }      
        notch[1] = charToInt("f");
    } else if (toLower(r2) == "3"){
        for (int i = 0; i < 26; i++){
            rotors[2][i] = rotor3[i];
        }
        notch[1] = charToInt("w");
    }

    if (toLower(r3) == "1"){
        for (int i = 0; i < 26; i++){
            rotors[3][i] = rotor1[i];
        }
        notch[2] = charToInt("r");
    } else if (toLower(r3) == "2"){
        for (int i = 0; i < 26; i++){
            rotors[3][i] = rotor2[i];
        }      
        notch[2] = charToInt("f");
    } else if (toLower(r3) == "3"){
        for (int i = 0; i < 26; i++){
            rotors[3][i] = rotor3[i];
        }
        notch[2] = charToInt("w");
    }


    offsets[0] = offset1;
    offsets[1] = offset2;
    offsets[2] = offset3;

    std::string result;
    int c = 0;
    for (int i = 0; i < message.length(); i++){
        if (message.substr(i, i+1) == " "){
            result+=" ";
        } else {
            offsets[2] = (offsets[2] + 1) % 26;
            if(offsets[1] == notch[1]-1){
                offsets[0] = (offsets[0] + 1) % 26;
                offsets[1] = (offsets[1] + 1) % 26;
            }
            if(offsets[2] == notch[2]){
                offsets[1] = (offsets[1] + 1) % 26;
            }

            c = plugs[charToInt(message.substr(i, i+1))];
            
            c = (c + offsets[2]) % 26;
            c = (rotors[3][c] - offsets[2]) % 26;

            c = (c + offsets[1]) % 26;
            c = (rotors[2][c] - offsets[1]) % 26;

            c = (c + offsets[0]) % 26;
            c = (rotors[1][c] - offsets[0]) % 26;

            c = rotors[0][c];
            
            for (int i = 0; i < 26; i++){
                if(rotors[1][i] == (c + offsets[0]) % 26){
                    c = i;
                }
            }

            c = (c - offsets[0]) % 26;

            for (int i = 0; i < 26; i++){
                if(rotors[2][i] == (c + offsets[1]) % 26){
                    c = i;
                }
            }            
            c = (c - offsets[1]) % 26;
            
            for (int i = 0; i < 26; i++){
                if(rotors[3][i] == (c + offsets[2]) % 26){
                    c = i;
                }
            }           
            c = (c - offsets[2]) % 26;

            for (int i = 0; i < 26; i++){
                if(plugs[i] == c){
                    c = i;
                }
            }

            result += intToChar(c);
        }
    }
    return result;
}