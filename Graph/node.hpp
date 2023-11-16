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

        int currentConflictOptStage;

        std::vector<Edge *> conflicts;

        Node(int examID, int numberOfStudents);

        std::string getExamAndPeriod();

        Edge * isConflicted(int conflictedNode);

        void addEdge(Edge * edge);

        void removeColourFromEntropy(int colour);

        int getEntropy();

        void setEntropy(int ent);

        std::vector<long long> conflictArray;

        void resetConflictArray(int ent);

        void redoConflictArray();

        int findSmallestInConflictArray();

        void changeColour(int desiredColour);

        void updateConflictArray(Node * otherNode, int conflicts);

        void updateConflictArray(int nodeColour, int conflicts);

        int actualPlacement;

        int clashWithNeighbours;

        float getClashWithNeighbours(bool real);

};

#endif