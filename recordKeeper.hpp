#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "stringSplit.hpp"
class RecordKeeper{
    public:
        std::string url;

        class Record{
            public:
                std::string title;
                float normCost;
                float time;

                Record(std::string title, float normCost, float time){
                    this->title = title;
                    this->normCost = normCost;
                    this->time = time;
                }

                std::string toString(){
                    return this->title + "," + std::to_string(this->normCost) + "," + std::to_string(this->time);
                }
        };

        std::vector<Record> records;

        std::vector<Record> readInRecords(){
            std::vector<Record> temp = std::vector<Record>();
            std::ifstream file(this->url);

            if(file.is_open()){
                std::string line;
                while(std::getline(file, line) && line != ""){
                    std::vector<std::string> * split = splitString(line, ",");
                    Record newRecord = Record(split->at(0), std::stof(split->at(1)), std::stof(split->at(2)));
                    temp.push_back(newRecord);
                }
            }

            file.close();
            return temp;
        }

        RecordKeeper(){
            this->url = "bestRecords.txt";
            this->records = readInRecords();
        }

        void compare(std::string title, float normCost, float time){

            if(title.find("/") != -1)title = title.substr(title.find_last_of("/") + 1, title.size());
            
            for(auto record: this->records){
                if(title == record.title){
                    if(normCost < record.normCost){
                        record.normCost = normCost;
                        record.time = time;
                        std::cout << "Best norm cost on " << title << std::endl;
                    }
                    if(normCost == record.normCost){
                        if(time < record.time){
                            record.time = time;
                        }
                    }
                }
            }
        }

        void writeOut(){
            remove("bestRecords.txt");

            std::ofstream MyFile("bestRecords.txt");

            for(auto record: this->records){
                MyFile << record.toString() << std::endl;
            }

            MyFile.close();
        }

};