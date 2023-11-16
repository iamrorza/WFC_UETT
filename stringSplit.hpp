#ifndef stringsplithpp
#define stringsplithpp

#include <string>
#include <iostream> 
#include <vector>


std::vector<std::string> * splitString(std::string in, std::string delim){
    std::vector<std::string> * split = new std::vector<std::string>();

    std::string buffer = "";
    for(int i = 0; i < in.size(); ++i){

        if(in[i] != ','){
            buffer += in[i];
        } 
        else{
            if(buffer != ""){
                split->push_back(buffer);
                buffer = "";
            }
        }
    }
    split->push_back(buffer);

    //printVec(split);

    return split;
}

std::vector<std::string> * splitStringBySpace(std::string stringToBeSplit){
    std::vector<std::string> * split = new std::vector<std::string>();

    std::string buffer = "";
    for(int i = 0; i < stringToBeSplit.size(); ++i){

        if(stringToBeSplit[i] != ' '){
            buffer += stringToBeSplit[i];
        } 
        else{
            if(buffer != ""){
                split->push_back(buffer);
                buffer = "";
            }
        }
    }
    split->push_back(buffer);

    //printVec(split);

    return split;
}

#endif