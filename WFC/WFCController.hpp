#ifndef wfccontrollerhpp
#define wfccontrollerhpp

#include <iostream>

#include "../Graph/graph.hpp"
#include "WFC.hpp"

void WFCController(Graph * graph){
    graph->biggestDegreeAmount();
    int checking = graph->numberOfPeriods;
    int lastChecked = 1000;
    bool isChecked = false;
    graph->resetGraph(checking);

    while(!isChecked){
        bool successful = WFC(graph, nullptr);

        if(successful){
            //get chromatic number
            int chromNumber = graph->getChromaticNumber();
            //std::cout << "Successful VCP check on number " << checking << " with chromatics number of " << chromNumber << std::endl;
            lastChecked = chromNumber;
            
            //subtract one from chromatic number
            checking = chromNumber - 1;
            //reset graphs
            graph->saveGraphNums();
            graph->resetGraph(checking);
        }
        else{
            graph->resetGraph(lastChecked - 1);

            int amount = WFCAll(graph, lastChecked - 1);

            if(amount >= lastChecked){
                isChecked = true;
                break;
            } 
            
        }
    }
    
    if(!(graph->savedGraph.size() == 0))graph->loadGraph();
    
}


void WFCController2(Graph * graph){
    float bestNormCost = 10000;
    int numberOfIters = 1;
    

 
    for(auto node: *graph->nodes){ 
        
        //std::cout << "Trying run start with node " << node->examID << std::endl;
        graph->resetGraph(graph->numberOfPeriods);

        bool success = WFC(graph, node);
        
        if(success && bestNormCost > graph->normalisedCost(true)){
            bestNormCost = graph->lastNormCost;
            graph->saveGraphNums();
            graph->lastBigCost = graph->totalCost(true);
        }
        
    
    }

    graph->loadGraph();
}

void WFCController3(Graph * graph){

    
    int numberOfIters = 1;

    float bestNormCost = 10000;
    graph->biggestDegreeAmount();

    int degreeOffset = 6;
    int maxDegreeThreshold = graph->biggestDegree/degreeOffset;

    for(auto node: *graph->nodes){ 

        if(node->degree < 15){
            continue;
        }
        //std::cout << "Trying run start with node " << node->examID << std::endl;
        graph->resetGraph(graph->numberOfPeriods);

        bool success = WFC(graph, node);
        
        if(success){
            if(bestNormCost > graph->normalisedCost(true)){
                graph->debugDegree = node->degree;
                bestNormCost = graph->lastNormCost;
                graph->saveGraphNums();
                graph->lastBigCost = graph->totalCost(true);
                //std::cout << bestNormCost << std::endl;
            }
        }
    }

    graph->loadGraph();
}

/*void WFCController3(Graph * graph){
    float bestNormCost = 10000;

    for(auto node: *graph->nodes){ 
        if(node->conflicts.size() != 0){
        
            for(int i = 0; i < graph->numberOfPeriods; ++i){
                graph->resetGraph(graph->numberOfPeriods);
                bool success = WFC2(graph, node, i);
                
                if(success && bestNormCost > graph->normalisedCost(true)){
                    bestNormCost = graph->lastNormCost;
                    graph->lastBigCost = graph->totalCost(true);
                    graph->saveGraphNums();
                    std::cout << "New Best: " << bestNormCost << " on Node " << node->examID << ", First placed: " << i << std::endl;
                }
            }
        }
    }

    graph->loadGraph();
}*/

#endif