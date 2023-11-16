#ifndef graphhpp
#define graphhpp


#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <cmath>
#include <map>
#include <random>
#include <climits>
#include "node.hpp"
#include "edge.hpp"


//NODE--------------------------------------------------------------------------------------------------------NODE
Node::Node(int examID, int numberOfStudents){
    this->examID = examID;
    this->numberOfStudents = numberOfStudents;
    this->colour = -1; //Setting as -1 as it hasn't been set yet
    this->degree = 0;
    this->possibleColours = new std::set<int>();
    this->actualPlacement = -1;
    this->conflictArray = std::vector<long long>();
    this->currentConflictOptStage = 0;
}

/*
    Returns the edge with the conflicting node
*/
Edge * Node::isConflicted(int conflictedNode){
    
    for(int i = 0; i < conflicts.size(); ++i){
        if(this->conflicts.at(i)->containsNode(conflictedNode)){
            return this->conflicts.at(i);
        }
    }
    return nullptr;
}

/*
    Gets exam and period in the output string format
*/
std::string Node::getExamAndPeriod(){
    return std::to_string(this->examID) + " " + std::to_string(this->colour) + "; ";
}

/*
    Add the edge to the nodes vector that contains the edges
*/
void Node::addEdge(Edge * edge){
    this->conflicts.push_back(edge);
    this->degree = this->degree + 1;
}

/*
    Removes designated colour from the set of possible colours that the
    node can be
*/
void Node::removeColourFromEntropy(int colour){

    if(this->possibleColours->count(colour) > 0){
        this->possibleColours->erase(colour);
    }
}

/*
    Returns the amount of possible colours
    a node can be coloured (a.k.a placed)
*/
int Node::getEntropy(){
    return this->possibleColours->size();
}

/*
    Set the initial entropy of a node
*/
void Node::setEntropy(int ent){ 
    this->possibleColours->clear();
    for(int j = 0; j < ent; ++j){
        this->possibleColours->insert(j);
    }
}

/*
    Reset the conflict array to make every value 0
*/
void Node::resetConflictArray(int ent){
    this->conflictArray.clear();
    this->conflictArray.resize(ent, 0);
}
/*
    Recalculate the conflict array of a node
*/
void Node::redoConflictArray(){
    this->resetConflictArray(this->conflictArray.size());

    for(auto edge: this->conflicts){
        if(edge->getOtherNode(this)->colour != -1){
            this->updateConflictArray(edge->getOtherNode(this)->colour, edge->numberOfConflicts);
        }
    }

    currentConflictOptStage = this->conflictArray.at(this->colour);
}

/*
    Find the index of the smallest value in the conflict array
*/
int Node::findSmallestInConflictArray(){
    int smallest = 100000;
    int smallestIndex =-1;
    for(int i = 0; i < this->conflictArray.size(); ++i){
        if(this->conflictArray.at(i) < smallest && this->conflictArray.at(i) != -1){
            smallest = this->conflictArray.at(i);
            smallestIndex = i;
        }
    }
    return smallestIndex;
}

/*
    Change the colour of this node, putting the original colour
    back into the entropy
*/
void Node::changeColour(int desiredColour){
    this->possibleColours->erase(this->colour);
    this->possibleColours->insert(desiredColour);

    for(auto edge: this->conflicts){
        edge->getOtherNode(this)->redoConflictArray();
    }

    this->colour = desiredColour;
}

/*
    Update conflict array of the chosen node
*/
void Node::updateConflictArray(Node * otherNode, int conflicts){
    int colour = otherNode->colour;
    int size = this->conflictArray.size();

    this->conflictArray.at(colour) = -1;

    this->removeColourFromEntropy(colour);
    for(int i  = 1; i <= 5; ++i){


        if(colour + i < size){
            if(this->conflictArray.at(colour + i) != -1){
                switch(i){
                    case 1: 
                        this->conflictArray.at(colour + i) += conflicts * 16;
                        break;
                    case 2:
                        this->conflictArray.at(colour + i) += conflicts * 8;
                        break;
                    case 3:
                        this->conflictArray.at(colour + i) += conflicts * 4;
                        break;
                    case 4:
                        this->conflictArray.at(colour + i) += conflicts * 2;
                        break;
                    case 5:
                        this->conflictArray.at(colour + i) += conflicts;
                        break;

                }
                //this->conflictArray.at(colour + i) += conflicts * pow(2, 5-i);

            }
        }

        if(colour - i >= 0){
            if(this->conflictArray.at(colour - i) != -1){
                switch(i){
                    case 1: 
                        this->conflictArray.at(colour - i) += conflicts * 16;
                        break;
                    case 2:
                        this->conflictArray.at(colour - i) += conflicts * 8;
                        break;
                    case 3:
                        this->conflictArray.at(colour - i) += conflicts * 4;
                        break;
                    case 4:
                        this->conflictArray.at(colour - i) += conflicts * 2;
                        break;
                    case 5:
                        this->conflictArray.at(colour - i) += conflicts;
                        break;

                }
            }
        }

    }
}

/*
    Update conflict array
*/
void Node::updateConflictArray(int nodeColour, int conflicts){
    int size = this->conflictArray.size();

    this->conflictArray.at(nodeColour) = -1;

    for(int i  = 1; i <= 5; ++i){

        if(nodeColour + i < size){
            if(this->conflictArray.at(nodeColour + i) != -1)this->conflictArray.at(nodeColour + i) += conflicts * pow(2, 5-i);
        }

        if(nodeColour - i >= 0){
            if(this->conflictArray.at(nodeColour - i) != -1)this->conflictArray.at(nodeColour - i) += conflicts * pow(2, 5-i);
        }
    }

}

/*
    Gets the conflict of an individual Node
*/
float Node::getClashWithNeighbours(bool real){
    if(real)int colour = this->colour;
    else int colour = this->actualPlacement;
    
    int total = 0;
    for(int i = 0; i < this->conflicts.size(); ++i){
        Node * otherNode = this->conflicts.at(i)->getOtherNode(this);
        if(abs(colour - otherNode->colour) == 1)total += this->conflicts.at(i)->numberOfConflicts * 16;
        else if(abs(colour - otherNode->colour) == 2)total += this->conflicts.at(i)->numberOfConflicts * 8;
        else if(abs(colour - otherNode->colour) == 3)total += this->conflicts.at(i)->numberOfConflicts * 4;
        else if(abs(colour - otherNode->colour) == 4)total += this->conflicts.at(i)->numberOfConflicts * 2;
    }

    this->clashWithNeighbours = total;
    return total;
}



//EDGE--------------------------------------------------------------------------------------------------------EDGE

Edge::Edge(Node * n1, Node * n2){
    this->numberOfConflicts = 1;
    this->node1 = n1;
    this->node2 = n2;
}

/*
    Check if the edge contains node with the specific nodeId
*/
bool Edge::containsNode(int nodeId){
    if(this->node1->examID == nodeId)return true;
    if(this->node2->examID == nodeId)return true;
    return false;
}

/*
    Adds this edge to the vector within the node that stores connections
*/
void Edge::addEdgesToNode(){
    this->node1->addEdge(this);
    this->node2->addEdge(this);
}

/*
    Given one node, return the adjoining node in the edge
*/
Node * Edge::getOtherNode(Node * n1){
    if(n1 == this->node1)return this->node2;
    else return this->node1;
}

/*
    Get conflict of a single edge
    If both nodes are the same, return big cost so that the algorithm does not use this run.
    This should never be the case though.
*/
int Edge::getConflict(){
    int colour1 = this->node1->colour;
    int colour2 = this->node2->colour;

    switch(abs(colour1 - colour2)){
        case 0:
            return 10000000;
        case 1:
            return 16 * this->numberOfConflicts;
        case 2:
            return 8 * this->numberOfConflicts;
        case 3:
            return 4 * this->numberOfConflicts;
        case 4:
            return 2 * this->numberOfConflicts;
        case 5:
            return this->numberOfConflicts;
        default:
            return 0;
    }
}


//GRAPH--------------------------------------------------------------------------------------------------------GRAPH
class Graph{
    public:
        std::vector<Node *> * nodes;
        std::vector<Edge *> * edges;

        int numberOfStudents;
        int numberOfNoiseStudents;

        int numberOfPeriods;

        int numberOfExams;

        int biggestDegree;

        std::vector<int> savedGraph;

        //Second stage
        std::vector<Node *> * noiseExams;
        std::vector<Node *> * prunedExams;

        float lastNormCost;
        long long lastCalculatedBigCost;
        long long lastBigCost;
        long long currentBigCost;

       
        Graph(){
            this->nodes = new std::vector<Node *>();
            this->edges = new std::vector<Edge *>();

            this->numberOfNoiseStudents = 0;
            this->numberOfStudents = 0;
            this->numberOfPeriods = 0;
            this->numberOfExams = 0;

            this->lastNormCost = 1000;
            this->lastBigCost = 100000000;
            this->lastCalculatedBigCost = 100000000;
            this->currentBigCost = 0;

            this->biggestDegreeAmount();
        }

        /*
            Add an edge to the graph
        */
        void addEdge(int n1, int n2){
            Node * node1 = (this->nodes->at(n1-1));
            Node * node2 = (this->nodes->at(n2-1));

            Edge * e = new Edge(node1, node2);
            
            this->edges->push_back(e);
            e->addEdgesToNode();
        }

        /*
            Add node to the Graph
        */
        void addNode(Node * node){
            this->nodes->push_back(node);
        }

        /*
            Get output string for validation
        */
        std::string outputString(){
            std::string output = "";
            for(int i = 0; i < this->nodes->size(); ++i){
                output+= this->nodes->at(i)->getExamAndPeriod();
            }
            return output;
        }

        /*
            Resets every node and return currentBigCost (Cumulative total) to 0
        */
        void resetGraph(int entropy){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->colour = -1;
                this->nodes->at(i)->resetConflictArray(entropy);
                this->nodes->at(i)->setEntropy(entropy);
            }
            this->currentBigCost = 0;
        }
    
        /*
            Set the entropy of a node with 'number'
        */
        void setNodesEntropy(int number){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->setEntropy(number);
            }
        }

        /*
            Sets the biggestDegree amount in the graph
        */
        void biggestDegreeAmount(){
            int biggest = 0;
            for(int i = 0; i < this->nodes->size(); ++i){
                if(this->nodes->at(i)->degree > biggest){
                    biggest = this->nodes->at(i)->degree;
                }
            }
            this->biggestDegree = biggest;
        }


        /*
            Return cost of solution before it is normalised. 
            If setcolour, uses the allocated colour/placement of the node: 'colour' variable
            Otherwise uses the 'actualColour variable
        */
        float totalCost(bool setColour){
            int totalCost = 0;

            for(int i= 0; i < this->edges->size(); ++i){
                Node * n1 = this->edges->at(i)->node1;
                Node * n2 = this->edges->at(i)->node2;

                int colour1, colour2;
                if(setColour){
                    colour1 = this->edges->at(i)->node1->colour;
                    colour2 = this->edges->at(i)->node2->colour;
                } else {
                    colour1 = this->edges->at(i)->node1->actualPlacement;
                    colour2 = this->edges->at(i)->node2->actualPlacement;
                }

                if(abs(colour1-colour2) == 0){
                    return MAXFLOAT;
                }
                else if(abs(colour1-colour2) == 1) totalCost += 16 * this->edges->at(i)->numberOfConflicts;
                else if(abs(colour1-colour2) == 2) totalCost += 8 * this->edges->at(i)->numberOfConflicts;
                else if(abs(colour1-colour2) == 3) totalCost += 4 * this->edges->at(i)->numberOfConflicts;
                else if(abs(colour1-colour2) == 4) totalCost += 2 * this->edges->at(i)->numberOfConflicts;
                else if(abs(colour1-colour2) == 5) totalCost += 1 * this->edges->at(i)->numberOfConflicts;
            }
            return totalCost;
        }

        /*
            Return normalised cost. 
            If setcolour, uses the allocated colour/placement of the node: 'colour' variable
            Otherwise uses the 'actualColour variable
        */

        float normalisedCost(bool setColour){
            float total = this->totalCost(setColour);
            float normCost = total/this->numberOfStudents;

            this->lastNormCost = normCost;
            return normCost;
        }

        /*
            Save graph numbers in a vector stored in the graph
        */
        void saveGraphNums(){
            std::vector<int> nums = std::vector<int>();
            nums.reserve(this->nodes->size());

            for(int i = 0; i < this->nodes->size(); ++i){
                nums.push_back(this->nodes->at(i)->colour);
            }

            this->savedGraph = nums;
        }

        /*
            Loads graph from the saved graph numbers
        */
        void loadGraph(){
            for(int i = 0 ; i < this->numberOfExams; ++i){
                this->nodes->at(i)->colour = this->savedGraph.at(i);
            }
        }

        /*
            Checks that Graph is valid
        */
        bool validGraph(){
            for(int i = 0; i < this->edges->size(); ++i){
                if(this->edges->at(i)->node1->colour == this->edges->at(i)->node2->colour || 
                    this->edges->at(i)->node1->colour == -1 ||
                    this->edges->at(i)->node2->colour == -1
                )return false;
            }
            return true;
        }

         /*
            Sets the value of nodes to their placeholder (This should already be the case
            but in some instances, without it it doesn't work)
        */
        void setAllNodesActualAsColour(){
            for(auto node: *this->nodes){
                node->actualPlacement = node->colour;
            }
        }

        /*
            Get random node from graph that is not in the already changed set.
        */
        Node * getRandomNode(std::set<Node *> * alreadyChanged){
            std::vector<Node *> notChanged = std::vector<Node *>();

            for(auto node: *this->nodes){
                if(alreadyChanged->count(node) == 0){
                    notChanged.push_back(node);
                }
            }
            if(notChanged.size() == 0)return nullptr;
            int random = rand()%notChanged.size();
            Node * randomNode = notChanged.at(random);
            return randomNode;
            
        }
};


/*
    Adds edges between nodes in the conflict classes. If a node already exists, increase its sise by one
*/
void addEdgesToConflictingClasses(std::vector<int> * conflictClasses, Graph * graph){
    int numConflicts = conflictClasses->size();
    for(int i = 0; i < numConflicts; ++i){
        for(int j = i + 1; j < numConflicts; ++j){

            Edge * conflictedEdge = graph->nodes->at(conflictClasses->at(i)-1)->isConflicted(conflictClasses->at(j));

            if(conflictedEdge == nullptr){
                graph->addEdge(conflictClasses->at(i),conflictClasses->at(j));
            }
            else {
                int currentConflicts = conflictedEdge->numberOfConflicts;
                conflictedEdge->numberOfConflicts = currentConflicts + 1;
            }
        }
    }
}

#endif
