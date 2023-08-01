#ifndef OPTSTAGEHPP
#define OPTSTAGEHPP

#include <set>

#include "Graph/graph.hpp"

bool checkForOptimisation(Node * n, Graph * g){
    //reset the conflict array
    
    n->redoConflictArray();

    //choose smallest
    int smallest = n->findSmallestInConflictArray();
    
    if(smallest != n->colour){
        n->changeColour(smallest);
        return true;
    }   
    return false;
}

void optstage(Graph * g){
    int maxAmountOfIterations = g->numberOfExams * 10;

    std::set<Node *> previouslyChanged = std::set<Node *>();

    int lastChanged = 0;
    for(int i = 1; i <= maxAmountOfIterations; ++i){
        //choose highest conflicting node
        //see if it can be changed
        //if it has a smaller cost, change
        //reset everything around it

        Node * biggestClasher = g->getBiggestClash(&previouslyChanged);
        bool changed = checkForOptimisation(biggestClasher, g);

        if(changed){
            previouslyChanged.clear();
        }
        previouslyChanged.insert(biggestClasher);

        if(i - lastChanged == g->numberOfExams)break;
        
    }

}
#endif