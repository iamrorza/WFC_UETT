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
    g->saveGraphNums();

    float norm = g->normalisedCost();

    for(int _ = 0; _ < 5; ++_){
        g->loadGraph();

        int maxAmountOfIterations = g->numberOfExams * 10;

        std::set<Node *> previouslyChanged = std::set<Node *>();
        
        int lastChanged = 0;
        for(int i = 1; i <= maxAmountOfIterations; ++i){
            //choose highest conflicting node
            //see if it can be changed
            //if it has a smaller cost, change
            //reset everything around it

            Node * biggestClasher = g->getRandomNode(&previouslyChanged);

            if(biggestClasher == nullptr)break;
            
            bool changed = checkForOptimisation(biggestClasher, g);

            if(changed){
                lastChanged = i;
                previouslyChanged.clear();
            }
            previouslyChanged.insert(biggestClasher);

            if(i - lastChanged == g->numberOfExams){
                std::cout << "Broken Out after " << i << "/" << maxAmountOfIterations << std::endl;
                break;
            }
        }

        if(g->normalisedCost() < norm){
            g->setAllNodesActualAsColour();
            norm = g->normalisedCost();
        }
    }
    g->setAllNodesActualAsColour();
    

}
#endif