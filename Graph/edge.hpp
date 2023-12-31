#ifndef edgehpp
#define edgehpp

#include <vector>
#include <string>

class Node;

class Edge{
    public:

        int numberOfConflicts;
        Node * node1;
        Node * node2;

        Edge(Node * n1, Node * n2);

        bool containsNode(int nodeId);

        void addEdgesToNode();

        Node * getOtherNode(Node * n1);

        int getConflict();
};
#endif