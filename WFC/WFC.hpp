#ifndef wfchpp
#define wfchpp

#include <vector>
#include <stack>
#include <iostream>
#include <thread>

#include "../Graph/graph.hpp"
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"


int getRandomFromMax(int max){
    int random = (std::rand() % max);
    return random;
}






int findSameCostAwayFromAvg(Node * n, int number){
    float avgNum = n->getAvgConflictNumber();
    float furthestDif = 100;

    int biggestCol = -1;
    for(int i= 0; i < n->conflictArray.size(); ++i){
        if(n->conflictArray.at(i) == number && abs(avgNum - i) > furthestDif){
            biggestCol = i;
            furthestDif =  abs(avgNum - i);
        }    
    }

    if(biggestCol != -1)return biggestCol;

    return number;
}

bool collapseNodeWithConflictArraySearch(Node * n, Graph * g){
    long long minimum = LLONG_MAX;
    int minIndex = -2;
    bool same = false;
    for(int i = 0; i < n->conflictArray.size(); ++i){
        if(n->conflictArray.at(i) > -1){
            if(n->conflictArray.at(i) < minimum){
                //std::cout << "Node " << n->examID << ". " << n->conflictArray.at(i) << " " << minimum << std::endl;
                minimum = n->conflictArray.at(i);
                minIndex = i;
                same = false;
            }
            else if(n->conflictArray.at(i) == minimum){
                same = true;
            }
        }
        
    }

    /*if(same){
        minIndex = findSameCostAwayFromAvg(n, minIndex);
    }*/

    //std::cout << "collapsedNode: " << minIndex << std::endl;
    n->colour = minIndex;
    if(minIndex != -2){
        g->currentBigCost += minimum;
        return true;
    }
    return false;
}

void collapse(Node * n1){
    //n1->setFirstNum();
    n1->colour = 0;
    //std::cout << "NODE " << n1->examID << " Collapsed with colour " << n1->colour <<std::endl;
}

bool graphIncomplete(Graph * graph, bool * zeroEnt){
    bool stillUncoloured = false;

    //TODO could potentially change this so that is not the 
    //zero is not checked here.
    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->getEntropy() == 0){
            *zeroEnt = true;
            return false;
        }
        if(graph->nodes->at(i)->colour == -1)stillUncoloured = true;
    }
    return stillUncoloured;
    
}

bool graphIncomplete(Graph * graph){
    bool stillUncoloured = false;

    //TODO could potentially change this so that is not the 
    //zero is not checked here.
    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->colour == -1)return true;
    }
    return false;
}


Node * observe(Graph * graph){
    int lowestEntropy = 1000000;
    int index = 0;
    bool anyZeroEnts = false;

    for(int i = 0; i < graph->numberOfExams; ++i){

        Node * currentNode = graph->nodes->at(i);

        int entropy = currentNode->getEntropy();

        if(currentNode->colour == -1){

            if(entropy < lowestEntropy){
                if(currentNode->getEntropy() == 0){
                    return nullptr;
                    //std::cout << "ZERRO ENT" << std::endl;
                }
                lowestEntropy = currentNode->getEntropy();
                index = i;
            }
            //currentNode->degree/currentNode->numberOfStudents < graph->nodes->at(index)->degree/graph->nodes->at(index)->numberOfStudents
            else if(entropy == lowestEntropy && 
                (float)currentNode->degree/(float)currentNode->numberOfStudents < (float)graph->nodes->at(index)->degree/(float)graph->nodes->at(index)->numberOfStudents){
                    
                //std::cout << (float)currentNode->degree/(float)currentNode->numberOfStudents << " " << (float)graph->nodes->at(index)->degree/(float)graph->nodes->at(index)->numberOfStudents << std::endl;
                index = i;
            }
        }
    }
    //if(anyZeroEnts)return nullptr;  
        
    return graph->nodes->at(index);
}


Node * propagateAndObserve(Node * n, Graph * graph){
    int lowestEntropy = 1000000;
    Node * currentLowest = nullptr;

    for(auto edge: n->conflicts){
        Node * otherNode = edge->getOtherNode(n);
        int entropy = otherNode->getEntropy();

        if(otherNode->colour == -1){

            otherNode->updateConflictArray(n, edge->numberOfConflicts);

            if(entropy == 0){
                //std::cout << "Zero entropy in prop" << std::endl;
                return nullptr;
            }

            if(entropy < lowestEntropy){
                //std::cout << "ENTORPY " << entropy << std::endl;
                lowestEntropy = entropy;
                currentLowest = otherNode;
            }
            else if(entropy == lowestEntropy && 
                (float)otherNode->degree/(float)otherNode->numberOfStudents < (float)currentLowest->degree/(float)currentLowest->numberOfStudents){
                    currentLowest = otherNode;
            }
        }

    }

    if(currentLowest == nullptr)return observe(graph);
    return currentLowest;
}
/*
Propagate - returns true if there is a node with zero entropy
*/
bool propagate(Graph * g, Node * n){
    for(auto edge: n->conflicts){
        Node * otherNode = edge->getOtherNode(n);
        if(otherNode->colour == -1){
            otherNode->updateConflictArray(n, edge->numberOfConflicts);
            if(otherNode->getEntropy() == 0){
                return true;
            }
        }
    }
    return false;
}

Node * observeNeighbours(Node * n, Graph * graph){
    int lowestEntropy = 1000000;
    Node * currentLowest = nullptr;
    bool anyZeroEnts = false;

    for(auto edge: n->conflicts){

        Node * currentNode = edge->getOtherNode(n);

        int entropy = currentNode->getEntropy();

        if(currentNode->colour == -1){

            if(entropy < lowestEntropy){
                if(entropy == 0){
                    return nullptr;
                    //std::cout << "ZERRO ENT" << std::endl;
                }
                lowestEntropy = currentNode->getEntropy();
                currentLowest = currentNode;
            }
            //currentNode->degree/currentNode->numberOfStudents < graph->nodes->at(index)->degree/graph->nodes->at(index)->numberOfStudents
            else if(entropy == lowestEntropy && 
                (float)currentNode->degree/(float)currentNode->numberOfStudents < (float)currentLowest->degree/(float)currentLowest->numberOfStudents){
                    currentLowest = currentNode;
            }
        }
    }
        
    if(currentLowest == nullptr)return observe(graph);

    return currentLowest;
}



bool WFC(Graph * graph, Node * startNode){
    
    bool zeroEntropy = false;

    Node * currentNode;

    std::vector<int> * placedNodes = new std::vector<int>();

    currentNode = startNode;
    currentNode->colour = 0;
    int placed = 1;
    placedNodes->push_back(currentNode->examID);

    
    while(placed < graph->nodes->size()){
        //std::cout << placed << "/" << graph->nodes->size() << std::endl;
        /*
            Current WFC Loop
            Find Node (or choose already placed node) (OBSERVE)
            See if it can collapse, if not, return false
            Propagate

            New?
            Propagate
            Find Node (or choose already placed node) (OBSERVE) 
            See if it can collapse, if not, return false
        */

        currentNode = propagateAndObserve(currentNode, graph);

        if(currentNode == nullptr){
            //std::cout << placed << "/" << graph->nodes->size() << std::endl;
            return false;
        }

        bool found = collapseNodeWithConflictArraySearch(currentNode, graph);
        if(!found)return false;

        ++placed;

        if(graph->currentBigCost > graph->lastBigCost){
            //std::cout << "Aborting run due to cost. Current " << graph->currentBigCost << " best " << graph->lastBigCost << " with " << placed << "/" << graph->nodes->size() << std::endl;
            return false;
        }
        placedNodes->push_back(currentNode->examID);        
    }

    
    return true;
}

int WFCAll(Graph * graph, int currentCheck){
    
    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->conflicts.size() == 0)continue;

        graph->resetGraph(currentCheck);
        bool done = WFC(graph, graph->nodes->at(i));
        if(done && graph->getChromaticNumber() <= currentCheck){
            graph->saveGraphNums();
            return graph->getChromaticNumber();
        }
        
    }
    return currentCheck + 1;
}




#endif