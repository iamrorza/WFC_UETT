#ifndef inputreaderhpp
#define inputreaderhpp

#include <fstream>
#include <string>
#include <iostream>
#include <string_view>
#include <vector>

#include "stringSplit.hpp"
#include "Graph/graph.hpp"

Graph * readTextFile(std::string fileURL){
    Graph * graph = new Graph();
    int biggestDeg = 0;
    std::ifstream file(fileURL);

    if(file.is_open()){
        std::string line;
        
        int examCount;
        int count = 0;
        int currentStudent = 0;
        std::vector<int> conflictClasses;

        while(std::getline(file, line)){

            if(count == 0){
                std::vector<std::string> * splitLine = splitStringBySpace(line);

                graph->numberOfStudents = std::stoi((*splitLine)[1]);
                graph->numberOfExams = std::stoi((*splitLine)[0]);
                graph->numberOfPeriods = std::stoi((*splitLine)[2]);
                examCount = std::stoi((*splitLine)[0]);
                ++count;
            } 
            else if(count >= 1 && count < 1 + examCount){
                std::vector<std::string> * splitLine = splitStringBySpace(line);

                int subjectNum = std::stoi((*splitLine)[0]);
                int numOfStudents = std::stoi((*splitLine)[1]);
                graph->addNode(new Node(subjectNum, numOfStudents));
                ++count;
            }
            else if(count == 1 + examCount){
                ++count;
            }
            else{         
                std::vector<std::string> * splitLine = splitStringBySpace(line);
                std::string student = (*splitLine)[0];
                int studentNumber = std::stoi(student.substr(1, (*splitLine)[0].size()));
                int courseNum = std::stoi((*splitLine)[1]);
                

                if(currentStudent == studentNumber){
                    conflictClasses.push_back(courseNum);
                }
                else {
                    if(conflictClasses.size() == 1){
                        graph->numberOfNoiseStudents++;
                    }
                    else{
                        if(conflictClasses.size() > biggestDeg)biggestDeg = conflictClasses.size();
                        addEdgesToConflictingClasses(&conflictClasses, graph);
                    }

                    currentStudent = studentNumber;
                    conflictClasses.clear();
                    conflictClasses.push_back(courseNum);
                }
                ++count;
            }
        }
        if(conflictClasses.size() == 1){
            graph->numberOfNoiseStudents++;
        }
        else{
            if(conflictClasses.size() > biggestDeg)biggestDeg = conflictClasses.size();
            addEdgesToConflictingClasses(&conflictClasses, graph);
        }
                
        file.close();
    }
    
    //std::cout << "THE BIGGEST DEGREE AT THE INPUT IS " << biggestDeg << std::endl;
    graph->setNodesEntropy(graph->numberOfPeriods);
    return graph;
}



#endif