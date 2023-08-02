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

#include "node.hpp"
#include "edge.hpp"

int getRandomFromMax2(int max){
    int random = (std::rand() % max);
    return random;
}
//NODE--------------------------------------------------------------------------------------------------------NODE
Node::Node(int examID, int numberOfStudents){
    this->examID = examID;
    this->numberOfStudents = numberOfStudents;
    this->colour = -1; //Setting as -1 as it hasn't been set yet
    this->degree = 0;
    this->possibleColours = new std::set<int>();
    this->actualPlacement = -1;
    this->conflictArray = std::vector<long long>();
}

Node::Node(int examID){
    this->examID = examID;
    this->numberOfStudents = 0;
    this->colour = -1; //Setting as -1 as it hasn't been set yet
    this->degree = 0;
    this->possibleColours = new std::set<int>();
    this->actualPlacement = -1;
    this->conflictArray = std::vector<long long>();
}

Edge * Node::isConflicted(int conflictedNode){
    
    for(int i = 0; i < conflicts.size(); ++i){
        if(this->conflicts.at(i)->containsNode(conflictedNode)){
            return this->conflicts.at(i);
        }
    }
    return nullptr;
}

std::string Node::getExamAndPeriod(){
    return std::to_string(this->examID) + " " + std::to_string(this->colour) + "; ";
}

void Node::addEdge(Edge * edge){
    this->conflicts.push_back(edge);
    this->degree = this->degree + 1;
}

std::vector<Node *> * Node::getUncolouredNeighboursWithEntOver1(){
    std::vector<Node *> * output = new std::vector<Node *>();

    for(int i = 0; i < this->conflicts.size(); ++i){
        Node * n2 = this->conflicts.at(i)->getOtherNode(this);
        if(n2->colour == -1 && n2->getEntropy() > 1){
            output->push_back(n2);
        }
    }
    return output;
}

void Node::removeColourFromEntropy(int colour){

    if(this->possibleColours->count(colour) > 0){
        this->possibleColours->erase(colour);
    }
    /*
    int index = -1;
    for(int i = 0; i < this->possibleColours->size(); ++i){
        if(this->possibleColours->at(i) == colour){
            index = i;
            break;
        }
    }

    if(index != -1){
        //std::cout << "Colour " << colour << " removed from node " << this->examID << std::endl;
        this->possibleColours->erase(this->possibleColours->begin() + index);
        this->possibleColours->shrink_to_fit();
    } else{
        //std::cout << "Node " << this->examID << " has already removed the colour " << colour << std::endl;
    }*/

}

std::vector<Node *> * Node::getUncolouredNeighbours(){
    std::vector<Node *> * output = new std::vector<Node *>();

    for(int i = 0; i < this->conflicts.size(); ++i){
        output->push_back(this->conflicts.at(i)->getOtherNode(this));
    }
    return output;
}

int Node::getEntropy(){
    //std::cout << "ENTROPY OF NODE " << this->examID << " IS " << this->possibleColours->size() << "\n";
    //return this->possibleColours->size();

    //int count = 0;
    //for(int i = 0; i < this->conflictArray.size(); ++i){
    //    if(this->conflictArray.at(i) == -1)++count;
    //}

    //return this->conflictArray.size() - count;

    return this->possibleColours->size();
}

void Node::setEntropy(int ent){ 
    this->possibleColours->clear();
    for(int j = 0; j < ent; ++j){
        this->possibleColours->insert(j);
    }
}

void Node::resetConflictArray(int ent){
    
    this->conflictArray.clear();
    //this->conflictArray.shrink_to_fit();
    this->conflictArray.resize(ent, 0);
    //this->printConflictArray();
}

void Node::redoConflictArray(){
    this->resetConflictArray(this->conflictArray.size());

    for(auto edge: this->conflicts){
        if(edge->getOtherNode(this)->colour != -1){
            this->updateConflictArray(edge->getOtherNode(this)->colour, edge->numberOfConflicts);
        }
    }
}

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

void Node::changeColour(int desiredColour){
    this->possibleColours->erase(this->colour);
    this->possibleColours->insert(desiredColour);

    for(auto edge: this->conflicts){
        edge->getOtherNode(this)->redoConflictArray();
    }

    this->colour = desiredColour;
}

void Node::updateConflictArray(Node * otherNode, int conflicts){
    int colour = otherNode->colour;
    int size = this->conflictArray.size();

    this->conflictArray.at(colour) = -1;

    this->removeColourFromEntropy(colour);
    //TODO For added cost in the simulation, I could change the base of the power to be a higher number than 2
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
                //this->conflictArray.at(colour - i) += conflicts * pow(2, 5-i);
            }
        }

    }
   //this->printConflictArray();
}

void Node::updateConflictArray(int nodeColour, int conflicts){
    int size = this->conflictArray.size();

    this->conflictArray.at(nodeColour) = -1;

    //TODO For added cost in the simulation, I could change the base of the power to be a higher number than 2
    for(int i  = 1; i <= 5; ++i){


        if(nodeColour + i < size){
            if(this->conflictArray.at(nodeColour + i) != -1)this->conflictArray.at(nodeColour + i) += conflicts * pow(2, 5-i);
        }

        if(nodeColour - i >= 0){
            if(this->conflictArray.at(nodeColour - i) != -1)this->conflictArray.at(nodeColour - i) += conflicts * pow(2, 5-i);
        }

    }
   //this->printConflictArray();
}

void Node::printConflictArray(){
    
    std::cout << this->examID << "(" << this->colour << "): ";
    for(int i = 0; i < this->conflictArray.size(); ++i){
        std::cout << this->conflictArray.at(i) <<", ";
    }
    std::cout << std::endl;
}

int Node::getAvgConflictNumber(){
    float avgNumber = 0;
    int total = 0;

    for(int i = 0; i < this->conflicts.size(); ++i){
        Edge * ed = this->conflicts.at(i);
        if(ed->getOtherNode(this)->colour != -1){
            avgNumber += ed->numberOfConflicts * ed->getOtherNode(this)->colour;
            total += ed->numberOfConflicts;
        }
    }

    return round(avgNumber/total);
}

void Node::debugOutput(){
    std::cout << this->possibleColours->size() << std::endl;
}

Node * Node::getHighestConflictNode(){

    Node * best = nullptr;
    int bestConflict = 0;
    for(auto edge: this->conflicts){
        if(best == nullptr && edge->getOtherNode(this)->colour == -1){
            best = edge->getOtherNode(this);
            bestConflict = edge->numberOfConflicts;
            std::cout << "HERE" << std::endl;
        }
        else{
            if(edge->numberOfConflicts > bestConflict){
                best = edge->getOtherNode(this);
                bestConflict = edge->numberOfConflicts;
            }
        }
    }
    
    return best;
}

void Node::setSecondStagePossible(int graphPeriods){
    //create set that contains every colour except those that are next to
    std::set<int> * sspc = new std::set<int>();

    for(int i = 0; i < graphPeriods; ++i){sspc->insert(i);}

    for(Edge * e: this->conflicts){
        Node * otherNode = e->getOtherNode(this);
        sspc->erase(otherNode->colour);
    }

    this->secondStagePossibleColours = sspc;
}

void Node::thirdStageColourChange(int desiredColour){
    if(this->secondStagePossibleColours->count(desiredColour) == 1){
        this->secondStagePossibleColours->erase(desiredColour);
        this->secondStagePossibleColours->insert(this->colour);
        this->colour = desiredColour;

    } 
    else{
        std::cout << "Not an available colour" << std::endl;
    }
}

bool Node::thirdStageActualPlacementChange(int desiredColour){

    if(this->secondStagePossibleColours->count(desiredColour) == 1){
        this->actualPlacement = desiredColour;
        return true;
    }
    return false;
}

std::string Node::nodeAndNeighbours(){
    std::string output = "Node " + std::to_string(this->examID);
    output = output + "(" + std::to_string(this->colour) + "): ";
    for(int i = 0; i < this->conflicts.size(); ++i){
        Node * otherNode = this->conflicts.at(i)->getOtherNode(this);
        output += std::to_string(otherNode->examID) + "(" + std::to_string(otherNode->colour) + "), ";
    }
    return output;
}

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

int Node::getCurrentCost(){
    int cost = 0;

    for(auto edge: this->conflicts){
        if(edge->getOtherNode(this)->colour != -1){
            cost += edge->getConflict();
        }
    }

    return cost;
}
//EDGE--------------------------------------------------------------------------------------------------------EDGE

Edge::Edge(Node * n1, Node * n2){
    this->numberOfConflicts = 1;
    this->node1 = n1;
    this->node2 = n2;
}

Edge::Edge(Node * n1, Node * n2, int numOfConflicts){
    this->numberOfConflicts = numOfConflicts;
    this->node1 = n1;
    this->node2 = n2;
}

bool Edge::containsNode(int nodeId){
    if(this->node1->examID == nodeId)return true;
    if(this->node2->examID == nodeId)return true;
    return false;
}
void Edge::addEdgesToNode(){
    this->node1->addEdge(this);
    this->node2->addEdge(this);
}

Node * Edge::getOtherNode(Node * n1){
    if(n1 == this->node1)return this->node2;
    else return this->node1;
}

void Edge::addNumberOfConflicts(){
    this->numberOfConflicts = this->numberOfConflicts + 1;   
}

int Edge::getConflict(){
    int colour1 = this->node1->colour;
    int colour2 = this->node2->colour;

    switch(abs(colour1 - colour2)){
        case 0:
            //std::cout << "Fucked by Node " << this->node1->examID << " and Node " << this->node2->examID << std::endl;
            return 100000;
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

bool Edge::bothPlaced(){
    return this->node1->colour != -1 && this->node2->colour != -1;
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
        }

        void addEdge(int n1, int n2){
            Node * node1 = (this->nodes->at(n1-1));
            Node * node2 = (this->nodes->at(n2-1));

            Edge * e = new Edge(node1, node2);
            
            this->edges->push_back(e);
            e->addEdgesToNode();
        }

        void addEdge2(Node * n1, Node * n2, int numberOfConflicts){

            Edge * e = new Edge(n1, n2, numberOfConflicts);
            
            this->edges->push_back(e);
            e->addEdgesToNode();
        }

        void addNode(Node * node){
            this->nodes->push_back(node);
        }

 
        std::string outputString(){
            std::string output = "";
            for(int i = 0; i < this->nodes->size(); ++i){
                output+= this->nodes->at(i)->getExamAndPeriod();
            }
            return output;
        }

        int getChromaticNumber(){
            std::set<int> ints = std::set<int>();

            for(int i = 0; i < this->nodes->size(); ++i){
                ints.insert(this->nodes->at(i)->colour);
            }
            return ints.size();
        }

        void resetGraph(int entropy){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->colour = -1;
                this->nodes->at(i)->resetConflictArray(entropy);
                this->nodes->at(i)->setEntropy(entropy);
            }
            //this->setNodesEntropy(entropy);
            this->currentBigCost = 0;
        }

        void setNodesEntropy(int number){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->setEntropy(number);
            }
        }
        void debugNodes(){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->debugOutput();
            }
        }
        void deleteGraph(){}

        void biggestDegreeAmount(){
            int biggest = 0;
            for(int i = 0; i < this->nodes->size(); ++i){
                if(this->nodes->at(i)->degree > biggest){
                    biggest = this->nodes->at(i)->degree;
                }
            }
            this->biggestDegree = biggest;
            //std::cout << "Biggest Degree of graph is " << this->biggestDegree << std::endl;
        }

        float totalCost(){
            int totalCost = 0;
            //std::cout << "Exams: " << this->numberOfExams << ", Students: " << this->numberOfStudents << ", Periods: " << this->numberOfPeriods << std::endl;
            
            for(int i= 0; i < this->edges->size(); ++i){
                Node * n1 = this->edges->at(i)->node1;
                Node * n2 = this->edges->at(i)->node2;

                if(abs(n1->colour - n2->colour) == 0)std::cout << "CANNOT OCCUR. PROGRAM BROKEN" << std::endl;
                else if(abs(n1->colour - n2->colour) == 1) totalCost += 16 * this->edges->at(i)->numberOfConflicts;
                else if(abs(n1->colour - n2->colour) == 2) totalCost += 8 * this->edges->at(i)->numberOfConflicts;
                else if(abs(n1->colour - n2->colour) == 3) totalCost += 4 * this->edges->at(i)->numberOfConflicts;
                else if(abs(n1->colour - n2->colour) == 4) totalCost += 2 * this->edges->at(i)->numberOfConflicts;
                else if(abs(n1->colour - n2->colour) == 5) totalCost += 1 * this->edges->at(i)->numberOfConflicts;
            }
            //std::cout << "Total Cost: " << totalCost << std::endl;
            return totalCost;
        }

        void printGraphInfo(){
            std::cout << "Exams: " << this->numberOfExams << ", Students: " << this->numberOfStudents << ", Periods: " << this->numberOfPeriods << std::endl;
        }
        float normalisedCost(){
            float total = this->totalCost();
            float normCost = total/this->numberOfStudents;

            this->lastNormCost = normCost;
            return normCost;
        }

        void printNormCost(){
            std::cout <<  "Normalised Cost " << normalisedCost() << std::endl;
        }

        int numOfConflicts(){
            return this->edges->size();
        }

        bool seeIfAnyColours(int col){
            for(auto node: *nodes){
                if(node->colour == col){return true;}
            }
            return false;
        }

        void setColourAsActual(){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->colour = this->nodes->at(i)->actualPlacement;
            }   
        }

        void printColourvsActual(){
            for(int i = 0; i < this->nodes->size(); ++i){
                std::cout << "NODe " << i << " " << this->nodes->at(i)->colour << " " << this->nodes->at(i)->actualPlacement << std::endl;
            }   
        }
        void printColour(){
            for(int i = 0; i < this->nodes->size(); ++i){
                std::cout << "NODe " << i << " " << this->nodes->at(i)->colour << std::endl;
            }   
        }
        void printOnlyNCost(){
            std::cout << normalisedCost() << std::endl;
        }

        std::vector<Node *> getNodesFromColour(int colour){
            std::vector<Node *> temp = std::vector<Node *>();
            for(auto node : *this->nodes){
                if(node->colour == colour)temp.push_back(node);
            }
            return temp;
        }

        void saveGraphNums(){
            std::vector<int> nums = std::vector<int>();

            for(int i = 0; i < this->nodes->size(); ++i){
                nums.push_back(this->nodes->at(i)->colour);
            }
            this->savedGraph = nums;
        }

        void loadGraph(){
            for(int i = 0 ; i < this->numberOfExams; ++i){
                this->nodes->at(i)->colour = this->savedGraph.at(i);
            }
        }

        bool validGraph(){
            for(int i = 0; i < this->edges->size(); ++i){
                if(this->edges->at(i)->node1->colour == this->edges->at(i)->node2->colour || 
                    this->edges->at(i)->node1->colour == -1 ||
                    this->edges->at(i)->node2->colour == -1
                )return false;
            }
            return true;
        }

        void putNoiseExamsInVec(){
            for(auto node: *this->nodes){
                if(node->conflicts.size() == 0){
                    this->noiseExams->push_back(node);
                    this->removeNodeFromExamVec(node);
                }
            }
        }

        void pruneExams(){
            for(auto node: *this->nodes){
                if(node->conflicts.size() == 1){
                    this->prunedExams->push_back(node);
                    this->removeNodeFromExamVec(node);
                }
            }
        }

        void removeNodeFromExamVec(Node * n){
            auto it = std::find(this->nodes->begin(), this->nodes->end(), n);
            if (it != this->nodes->end()) {
                this->nodes->erase(it);
            }
        }

        void setAllNodesActualAsColour(){
            for(auto node: *this->nodes){
                node->actualPlacement = node->colour;
            }
        }

        void printNodesAndNeigh(){
            for(auto node: *this->nodes){
                std::cout << node->nodeAndNeighbours() << std::endl;
            }
        }

        Node * getBiggestClashNode(std::set<Node *> * alreadyChanged){
            Node * biggestClasher;
            int biggestClash = 0;

            for(auto node: *this->nodes){
                if(alreadyChanged->count(node) != 1){
                    node->getClashWithNeighbours(true);
                    if(node->clashWithNeighbours >= biggestClash){
                        biggestClash = node->clashWithNeighbours;
                        biggestClasher = node;
                    }
                }
            }
            return biggestClasher;
        }

        Node * getBiggestClashNodePerStudent(std::set<Node *> * alreadyChanged){
            Node * biggestClasher;
            float biggestClash = 0;

            for(auto node: *this->nodes){
                if(alreadyChanged->count(node) != 1){
                    node->getClashWithNeighbours(true);
                    if(node->clashWithNeighbours/node->numberOfStudents >= biggestClash){
                        biggestClash = node->clashWithNeighbours/node->numberOfStudents;
                        biggestClasher = node;
                    }
                }
            }
            return biggestClasher;
        }

        void getRidOfNoiseAndOneDegreeNodes(){
            this->pruneExams();
            this->putNoiseExamsInVec();

            std::cout << "Noise: " << this->noiseExams->size() << " , Pruned: " << this->prunedExams << std::endl;
        }

        std::map<int,int> getMap(){
            std::map<int, int> newMap = std::map<int, int>();

            for(auto node: *this->nodes){
                newMap[node->examID] = node->colour;
            }

            return newMap;
        }

        void useMap(std::map<int,int> map){
            for(auto node: *this->nodes){
                node->colour = map[node->colour];
            }
        }

        void setUpNodesForOpt(){
            for(auto node: *this->nodes){
                node->actualPlacement = node->colour;
            }
        }

        Node * getHighestConflictNode(Node * othernode){
            int biggestCost = 0;
            Node * best = nullptr;

            for(auto node: *this->nodes){
                if(node->colour != -1 && node != othernode){
                    if(node->getCurrentCost() > biggestCost){
                        biggestCost = node->getCurrentCost();
                        best = node;
                    }
                }
            }
            return best;
        }

        bool invalidGraphNotCountingUnplaced(){
            for(auto edge: *this->edges){
                if(edge->bothPlaced()){
                    if(edge->node1->colour == edge->node2->colour)return true;
                }
            }
            return false;
        }

        Node * getRandomNode(std::set<Node *> * alreadyChanged){

            std::vector<Node *> notChanged = std::vector<Node *>();

            for(auto node: *this->nodes){
                if(alreadyChanged->count(node) == 0){
                    notChanged.push_back(node);
                }
            }
            int random = rand()%notChanged.size();

            if(notChanged.size() == 0)return nullptr;
            

            Node * randomNode = notChanged.at(random);
            return randomNode;
            
        }
};


//FUNCTIONS TO DO WITH THE GRAPH
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