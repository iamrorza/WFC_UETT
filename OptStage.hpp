#ifndef OPTSTAGEHPP
#define OPTSTAGEHPP

#include <set>
#include "Graph/graph.hpp"

void propagateChange(Node * n){
    for(auto edge: n->conflicts){
        edge->getOtherNode(n)->getClashWithNeighbours(true);
    }
}

bool checkForOptimisation(Node * n, Graph * g){
    //reset the conflict array
    n->redoConflictArray();

    //choose smallest
    int smallest = n->findSmallestInConflictArray();
    
    if(smallest != n->colour){
        n->changeColour(smallest);
        propagateChange(n);
        return true;
    }   

    return false;
}


void optstage(Graph * g){
    g->saveGraphNums();

    float norm = g->normalisedCost(true);

    int maxAmountOfIterations = g->numberOfExams * 30;

    std::set<Node *> previouslyChanged = std::set<Node *>();


    for(int i = 1; i <= maxAmountOfIterations; ++i){
        //choose random node
        //see if it can be changed
        //if it has a smaller cost, change
        //reset everything around it
        Node * biggestClasher = g->getRandomNode(&previouslyChanged);
        
        if(biggestClasher == nullptr){
            break;
        }
        else{
            bool changed = checkForOptimisation(biggestClasher, g);

            if(changed){
                previouslyChanged.clear();
            }
            previouslyChanged.insert(biggestClasher);
        }
    }

    if(g->normalisedCost(true) < norm){
        g->setAllNodesActualAsColour();
        norm = g->normalisedCost(true);
    }
    g->setAllNodesActualAsColour();
}

#endif