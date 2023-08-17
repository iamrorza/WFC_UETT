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


Node * getNextNode(Node * n, std::set<Node *> * alreadyChanged){
    Node * currentBest = nullptr;
    for(auto edge: n->conflicts){
        if(alreadyChanged->count(edge->getOtherNode(n)) == 0){
            if(currentBest == nullptr)currentBest = edge->getOtherNode(n);

            else{
                Node * otherNode = edge->getOtherNode(n);
                if(currentBest->clashWithNeighbours/currentBest->numberOfStudents  < otherNode->clashWithNeighbours/otherNode->numberOfStudents)currentBest = otherNode;
            }
        }
    }
    return currentBest;
}

void optstage(Graph * g){
    g->saveGraphNums();

    float norm = g->normalisedCost(true);

    g->loadGraph();

    int maxAmountOfIterations = g->numberOfExams * 100 ;

    std::set<Node *> previouslyChanged = std::set<Node *>();
    
    int lastChanged = 0;


    for(int i = 1; i <= maxAmountOfIterations; ++i){
        //choose highest conflicting node
        //see if it can be changed
        //if it has a smaller cost, change
        //reset everything around it
        Node * biggestClasher = g->getRandomNode(&previouslyChanged);

        if(biggestClasher == nullptr){
            std::cout << "Broken Out after " << i << "/" << maxAmountOfIterations << std::endl;
            break;
        }
        else{
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
    }

    if(g->normalisedCost(true) < norm){
        g->setAllNodesActualAsColour();
        norm = g->normalisedCost(true);
    }
    g->setAllNodesActualAsColour();
}

void optstage2(Graph * g){
    g->saveGraphNums();

    float norm = g->normalisedCost(true);

    g->loadGraph();

    int maxAmountOfIterations = g->numberOfExams * 100 ;

    std::set<Node *> previouslyChanged = std::set<Node *>();
    
    int lastChanged = 0;

    Node * biggestClasher = g->getBiggestClashNodePerStudent(&previouslyChanged);
    
    for(int i = 1; i <= maxAmountOfIterations; ++i){
        //choose highest conflicting node
        //see if it can be changed
        //if it has a smaller cost, change
        //reset everything around it
        if(biggestClasher == nullptr){
            //std::cout << "Broken Out after " << i << "/" << maxAmountOfIterations << std::endl;
            //break;
            previouslyChanged.clear();
            biggestClasher = g->getBiggestClashNodePerStudent(&previouslyChanged);
        }
        else{
            bool changed = checkForOptimisation(biggestClasher, g);

            if(changed){
                lastChanged = i;
                previouslyChanged.clear();
            }
            previouslyChanged.insert(biggestClasher);
            
            if(i - lastChanged == g->numberOfExams){
                //std::cout << "Broken Out after " << i << "/" << maxAmountOfIterations << std::endl;
                break;
            }
        }

        biggestClasher = getNextNode(biggestClasher, &previouslyChanged);
    }

    if(g->normalisedCost(true) < norm){
        g->setAllNodesActualAsColour();
        norm = g->normalisedCost(true);
    }
    g->setAllNodesActualAsColour();
}
#endif