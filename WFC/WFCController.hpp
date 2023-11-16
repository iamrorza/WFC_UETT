#ifndef wfccontrollerhpp
#define wfccontrollerhpp

#include <iostream>

#include "../Graph/graph.hpp"
#include "WFC.hpp"

void WFCController(Graph * graph){

    int numberOfIters = 1;

    float bestNormCost = 10000;
    graph->biggestDegreeAmount();

    int degreeOffset = 6;
    int maxDegreeThreshold = graph->biggestDegree/degreeOffset;

    for(auto node: *graph->nodes){ 

        if(node->degree < maxDegreeThreshold){
            continue;
        }
        graph->resetGraph(graph->numberOfPeriods);

        bool success = WFC(graph, node);
        
        if(success){
            if(graph->normalisedCost(true) < bestNormCost){
                bestNormCost = graph->lastNormCost;
                graph->saveGraphNums();
                graph->lastBigCost = graph->totalCost(true);
            }
        }
    }
    graph->loadGraph();
}

#endif
