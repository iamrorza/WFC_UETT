#ifndef stringsplithpp
#define stringsplithpp

#include <string>
#include <iostream> 
#include <vector>

void printVec(std::vector<std::string> * vec){
    for(int i = 0; i < (*vec).size(); ++i){
        std::cout << (*vec)[i] << ", ";
    }
    std::cout << std::endl;
}

void printIntVec(std::vector<int> vec){
    std::cout << "Adj Size: " << (vec).size() << "  | " ;
    for(int i = 0; i < (vec).size(); ++i){
        std::cout << (vec)[i] << ", ";
    }
    std::cout << std::endl;
}

std::vector<std::string> * splitString(std::string in, std::string delim){
    std::vector<std::string> *  split = new std::vector<std::string>();
    int currentIndex = 0;

    while(currentIndex < in.find_last_of(delim)){
        split->push_back(in.substr(currentIndex, in.find(delim, currentIndex)));
        currentIndex = in.find(delim, currentIndex);
    }
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