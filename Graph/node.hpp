#ifndef nodehpp
#define nodehpp

#include <vector>
#include <string>
#include <set>

class Edge;

class Node{
    public:
        int examID;

        int colour; //Essentially the chosen period 
        
        std::set<int> * possibleColours;

        int numberOfStudents;
        
        int degree;

        std::vector<Edge *> conflicts;

        Node(int examID, int numberOfStudents);

        Node(int examID);

        std::string getExamAndPeriod();

        Edge * isConflicted(int conflictedNode);

        void addEdge(Edge * edge);

        std::vector<Node *> * getUncolouredNeighboursWithEntOver1();

        std::vector<Node *> * getUncolouredNeighbours();

        void removeColourFromEntropy(int colour);

        int getEntropy();

        void setFirstNum();
  
        void debugOutput();

        void setEntropy(int ent);

        int getAvgConflictNumber();

        Node * getHighestConflictNode();

        std::vector<long long> conflictArray;

        void resetConflictArray(int ent);

        void redoConflictArray();

        int findSmallestInConflictArray();

        void changeColour(int desiredColour);

        void updateConflictArray(Node * otherNode, int conflicts);

        void updateConflictArray(int nodeColour, int conflicts);

        void printConflictArray();

        int getCurrentCost();
        //THINGS FOR THE SECOND STAGE
        std::set<int> * secondStagePossibleColours;

        void setSecondStagePossible(int graphPeriods);

        int actualPlacement;

        void resetPossibleColours(int num);

        //THINGS FOR THIRD STAGE
        void thirdStageColourChange(int desiredColour);

        bool thirdStageActualPlacementChange(int desiredColour);

        std::string nodeAndNeighbours();

        int clashWithNeighbours;

        float getClashWithNeighbours(bool real);

};

#endif