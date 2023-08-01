#ifndef wfchpp
#define wfchpp

#include <vector>
#include <stack>
#include <iostream>
#include <thread>

#include "../Graph/graph.hpp"
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"

int getRandomNumberFrom100(){
    int random = (std::rand() % 100);
    return random;
}

int getRandomFromMax(int max){
    int random = (std::rand() % max);
    return random;
}

Node * getRandomUncolouredNode(std::vector<Node *> * nodes){
    Node * node = nullptr;

    while(node == nullptr){
        node = nodes->at(getRandomFromMax(nodes->size()));
        if(node->colour != -1)node = nullptr;
    }
    return node;
}

Node * biggestConflictNode(std::vector<Edge *> * edges){
    int biggestConflict = 0;
    int index = 0;
    for(int i = 1; i < edges->size(); ++i){
        if(edges->at(i)->numberOfConflicts > biggestConflict){
            index = i;
            biggestConflict = edges->at(i)->numberOfConflicts;
        }
    }

    if(edges->at(index)->node1->degree >= edges->at(index)->node2->degree){
        return edges->at(index)->node1;
    }
    else{
        return edges->at(index)->node2;
    }
}

Node * biggestDegreeNode(std::vector<Node *> * nodes){
    int biggestDegree = 0;
    int index = 0;
    for(int i = 1; i < nodes->size(); ++i){
        if(nodes->at(i)->degree > biggestDegree){
            biggestDegree = nodes->at(i)->degree;
            index = i;
        }
    }

    return nodes->at(index);
}

bool itemInStack(std::stack<Node *> stack, Node * node){
    while(!stack.empty()){
        if(node == stack.top())return true;
        else stack.pop();
    }
    return false;
}

void propagateSingleNode(Graph * g, std::vector<int> * nodes, std::vector<int> * conflicts, int thread, int currentNode){
    //std::cout << "Started thread " << thread << std::endl; 
    for(int i = 0; i < nodes->size(); ++i){
        Node * n = g->nodes->at(currentNode);
        //std::cout << g->nodes->at(i)->examID << " is colour " << n->colour << " on thread " << thread << std::endl;
        g->nodes->at(nodes->at(i)-1)->updateConflictArray(n->colour, conflicts->at(i));
    }
    //std::cout << "Ended thread " << thread << std::endl;
}

void propagateThreads(Graph * graph, Node * node){
    std::vector<int> nodes1 = std::vector<int>();
    std::vector<int> nodes1Conflict = std::vector<int>();
    std::vector<int> nodes2 = std::vector<int>();
    std::vector<int> nodes2Conflict = std::vector<int>();
    std::vector<int> nodes3 = std::vector<int>();
    std::vector<int> nodes3Conflict = std::vector<int>();
    std::vector<int> nodes4 = std::vector<int>();
    std::vector<int> nodes4Conflict = std::vector<int>();

    
    int thread = 1;
    for(auto edge: node->conflicts){
        if(edge->getOtherNode(node)->colour == -1){
            switch(thread){
                case 1:
                    nodes1.push_back(edge->getOtherNode(node)->examID);
                    nodes1Conflict.push_back(edge->numberOfConflicts);
                    thread = 2;
                    break;
                case 2:
                    nodes2.push_back(edge->getOtherNode(node)->examID);
                    nodes2Conflict.push_back(edge->numberOfConflicts);
                    thread = 3;
                case 3:
                    nodes3.push_back(edge->getOtherNode(node)->examID);
                    nodes3Conflict.push_back(edge->numberOfConflicts);
                    thread = 4;
                default:
                    nodes4.push_back(edge->getOtherNode(node)->examID);
                    nodes4Conflict.push_back(edge->numberOfConflicts);
                    thread = 1;
            }
            //edge->getOtherNode(node)->updateConflictArray(node, edge->numberOfConflicts);
        }
    }

    //std::cout << nodes1.size() << " " << nodes1Conflict.size() << " " << nodes2.size() << " " << nodes2Conflict.size() << std::endl;
    
    std::thread thread1(propagateSingleNode, graph, &nodes1, &nodes1Conflict, 1, node->examID-1);
    std::thread thread2(propagateSingleNode, graph, &nodes2, &nodes2Conflict, 2, node->examID-1);
    std::thread thread3(propagateSingleNode, graph, &nodes3, &nodes3Conflict, 3, node->examID-1);
    std::thread thread4(propagateSingleNode, graph, &nodes4, &nodes4Conflict, 4, node->examID-1);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

}

void propagate(Graph * g, Node * n){
    for(auto edge: n->conflicts){
        if(edge->getOtherNode(n)->colour == -1){
            edge->getOtherNode(n)->updateConflictArray(n, edge->numberOfConflicts);
        }
    }
}



/*void collapseNodeWithSearch(Node * n){
    int avgNum = n->getAvgConflictNumber();

    int furthestDif = -1;
    std::vector<int> equalPossible = std::vector<int>();
    int biggestCol = -1;
    for(int i= 0; i < n->possibleColours->size(); ++i){
        if(abs(n->possibleColours->at(i) - avgNum) > furthestDif){
            furthestDif = abs(n->possibleColours->at(i) - avgNum);
            biggestCol = n->possibleColours->at(i);
            equalPossible.clear();
            equalPossible.push_back(biggestCol);
        }
        else if(abs(n->possibleColours->at(i) - avgNum) == furthestDif){
            equalPossible.push_back(n->possibleColours->at(i));
        }
    }

    if(equalPossible.size() == 1)n->colour = biggestCol;

    else{
        n->colour = equalPossible.at(getRandomFromMax(equalPossible.size()));
    }
}*/

bool collapseNodeWithConflictArraySearch(Node * n, Graph * g){
    long long minimum = LLONG_MAX;
    int minIndex = -2;

    for(int i = 0; i < n->conflictArray.size(); ++i){
        if(n->conflictArray.at(i) > -1 && n->conflictArray.at(i) < minimum){
            //std::cout << "Node " << n->examID << ". " << n->conflictArray.at(i) << " " << minimum << std::endl;
            minimum = n->conflictArray.at(i);
            minIndex = i;
        }
    }


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

    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->getEntropy() == 0){
            *zeroEnt = true;
            return false;
        }
        if(graph->nodes->at(i)->colour == -1)stillUncoloured = true;
    }
    return stillUncoloured;
    
}

Node * observe(Graph * graph){
    int lowestEntropy = 1000000;
    int index = 0;
    bool anyZeroEnts = false;
    for(int i = 0; i < graph->numberOfExams; ++i){
        Node * currentNode = graph->nodes->at(i);
        if(currentNode->colour == -1 && currentNode->getEntropy() < lowestEntropy){
            if(currentNode->getEntropy() == 0){
                anyZeroEnts = true;
                //std::cout << "ZERRO ENT" << std::endl;
            }
            lowestEntropy = currentNode->getEntropy();
            index = i;
        }
    }

    if(anyZeroEnts)return nullptr;  
        
    return graph->nodes->at(index);

}

Node * getLowestEntropyNeighbour(Node * n, Graph * g){
    int lowestEntropy = 1000000;
    int index = -1;

    for(int i = 0; i < n->conflicts.size(); ++i){
        if(n->conflicts.at(i)->getOtherNode(n)->colour == -1){
            if(n->conflicts.at(i)->getOtherNode(n)->getEntropy() < lowestEntropy){
                index = i;
            }
        }
    }

    if(index != -1)return n->conflicts.at(index)->getOtherNode(n);

    return observe(g);
}



bool WFC(Graph * graph, Node * startNode){

    bool zeroEntropy = false;

    Node * currentNode;

    if(startNode == nullptr){
        currentNode = biggestDegreeNode(graph->nodes);
    } else {
        currentNode = startNode;
    }


    collapse(currentNode);

    propagate(graph, currentNode);

    currentNode = observe(graph);

    while(graphIncomplete(graph, &zeroEntropy)){
        

        bool found = collapseNodeWithConflictArraySearch(currentNode, graph);
        if(!found)return false;
        propagate(graph, currentNode);

        if(graph->currentBigCost > graph->lastBigCost){
            //std::cout << "Aborting run due to cost. Current " << graph->currentBigCost << " best " << graph->lastBigCost << std::endl;
            return false;
        }
        
        currentNode = observe(graph);
        
    }

    if(zeroEntropy)return false;
    
    return true;
}

bool WFC2(Graph * graph, Node * startNode, int startColour){

    bool zeroEntropy = false;

    Node * currentNode;

    if(startNode == nullptr){
        currentNode = biggestDegreeNode(graph->nodes);
    } else {
        currentNode = startNode;
    }
    

    startNode->colour=startColour;

    propagate(graph, currentNode);

    currentNode = observe(graph);

    while(graphIncomplete(graph, &zeroEntropy)){

        collapseNodeWithConflictArraySearch(currentNode, graph);

        propagate(graph, currentNode);

        if(graph->currentBigCost > graph->lastBigCost){
            //std::cout << "Aborting run due to cost. Current " << graph->currentBigCost << " best " << graph->lastBigCost << std::endl;
            return false;
        }
        currentNode = observe(graph);
    }

    if(zeroEntropy)return false;
    
    return true;
}

/*bool WFC2(Graph * graph, Node * startNode, int startColour){

    bool zeroEntropy = false;

    Node * currentNode;

    if(startNode == nullptr){
        currentNode = biggestDegreeNode(graph->nodes);
    } else {
        currentNode = startNode;
    }
    

    currentNode->colour = startColour;
    propagate(graph, currentNode);
    currentNode = observe(graph);

    while(graphIncomplete(graph, &zeroEntropy)){
        collapseNodeWithSearch(currentNode);
        propagate(graph, currentNode);
        graph->currentBigCost += currentNode->getCurrentCost();
        if(graph->currentBigCost > graph->lastBigCost){
            //std::cout << "Aborting run due to cost. Current " << graph->currentBigCost << " best " << graph->lastBigCost << std::endl;
            return false;
        }
        currentNode = observe(graph);
    }

    if(zeroEntropy)return false;
    
    return true;
}*/

/*
bool WFC3(Graph * graph, Node * startNode, int refreshRate){

    bool zeroEntropy = false;

    Node * currentNode;

    if(startNode == nullptr){
        currentNode = biggestDegreeNode(graph->nodes);
    } else {
        currentNode = startNode;
    }

    int count = 1;
    Node * lastMoved = nullptr;

    collapse(currentNode);

    propagate(graph, currentNode);

    currentNode = observe(graph);

    while(graphIncomplete(graph, &zeroEntropy)){
        if(count % refreshRate == 0){
            Node * bestCost = graph->getHighestConflictNode(lastMoved);
            if(bestCost != nullptr){
                checkForOptimisation(bestCost, graph);
            }
            lastMoved = bestCost;
        }
        else{
            bool found = collapseNodeWithConflictArraySearch(currentNode, graph);
            if(!found)return false;
            propagate(graph, currentNode);

            if(graph->currentBigCost > graph->lastBigCost){
                //std::cout << "Aborting run due to cost. Current " << graph->currentBigCost << " best " << graph->lastBigCost << std::endl;
                return false;
            }
            currentNode = observe(graph);
        }
        
        
        ++count;
    }

    if(zeroEntropy)return false;
    
    return true;
}*/

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