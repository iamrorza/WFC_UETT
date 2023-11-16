#ifndef wfchpp
#define wfchpp

#include <vector>
#include <stack>
#include <iostream>
#include <thread>

#include "../Graph/graph.hpp"
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"


/*
    Finds and collapses node with smallest colour/placement in the conflict array
    Returns false if no colour can be found (Entropy is 0)
*/

bool collapseNodeWithConflictArraySearch(Node * n, Graph * g){
    long long minimum = LLONG_MAX;
    int minIndex = -2;
    for(int i = 0; i < n->conflictArray.size(); ++i){
        if(n->conflictArray.at(i) > -1){
            if(n->conflictArray.at(i) < minimum){
                minimum = n->conflictArray.at(i);
                minIndex = i;
            }
        }
        
    }
    n->colour = minIndex;
    if(minIndex != -2){
        g->currentBigCost += minimum;
        return true;
    }
    return false;
}

/*
    Observes if there are no uncoloured neighbours in 'propagateAndObserve'
    This happens when there are weakly connceted components

    Returns lowest entropy node
    Returns nullptr if there is a node with entropy 0
*/
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
                }
                lowestEntropy = currentNode->getEntropy();
                index = i;
            }
            else if(entropy == lowestEntropy && 
                (float)currentNode->degree/(float)currentNode->numberOfStudents < (float)graph->nodes->at(index)->degree/(float)graph->nodes->at(index)->numberOfStudents)
            {
                   index = i;
            }
        }
    }

    return graph->nodes->at(index);
}

/*
    Propagates changes and returns lowest entropy node 
    Will return nullptr if there are any with 0 entropy
*/

Node * propagateAndObserve(Node * n, Graph * graph){
    int lowestEntropy = 1000000;
    Node * currentLowest = nullptr;

    for(auto edge: n->conflicts){
        Node * otherNode = edge->getOtherNode(n);
        int entropy = otherNode->getEntropy();

        if(otherNode->colour == -1){

            otherNode->updateConflictArray(n, edge->numberOfConflicts);

            if(entropy == 0){
                return nullptr;
            }

            if(entropy < lowestEntropy){
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
    Main WFC Function. Returns true for a successful run.
    Returns false if:
        Node with 0 entropy
        Cumulative Cost is larger than that of the last successful run
*/
bool WFC(Graph * graph, Node * startNode){
    
    bool zeroEntropy = false;

    Node * currentNode;

    std::vector<int> * placedNodes = new std::vector<int>();

    currentNode = startNode;
    currentNode->colour = 0;
    int placed = 1;
    placedNodes->push_back(currentNode->examID);

    
    while(placed < graph->nodes->size()){
        currentNode = propagateAndObserve(currentNode, graph);
        
        /*
            Will be nullptr if there is a node with entropy 0
        */
        if(currentNode == nullptr){
            return false;
        }

        bool found = collapseNodeWithConflictArraySearch(currentNode, graph);
        if(!found)return false;

        ++placed;

        /*
            If cumulative cost is greater than previous successful run, return false
        */
        if(graph->currentBigCost > graph->lastBigCost){
            return false;
        }
        placedNodes->push_back(currentNode->examID);        
    }

    return true;
}
#endif