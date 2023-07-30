#ifndef THIRDSTAGEHPP
#define THIRDSTAGEHPP

#include "../Graph/graph.hpp"


void ThirdStage(Graph * graph){
    for(int i = 0; i < graph->nodes->size(); ++i){
        graph->nodes->at(i)->setSecondStagePossible(graph->numberOfPeriods);
        graph->nodes->at(i)->actualPlacement = graph->nodes->at(i)->colour;
    }

    int count = 1; 
    int amountOfIterations = 1000;
    int clearReset = 20;
    
    std::set<Node *> alreadyChanged = std::set<Node *>();
    
    while(count < amountOfIterations){
        Node * biggestClasher = graph->getBiggestClashNode(&alreadyChanged);

        biggestClasher->setSecondStagePossible(graph->numberOfPeriods);
        
        int bestCost = biggestClasher->getClashWithNeighbours(true);
        int bestCostElement = -1;

        for (const int& element : *biggestClasher->secondStagePossibleColours){
            
            bool isPossible = biggestClasher->thirdStageActualPlacementChange(element);
            
            if(isPossible && element != biggestClasher->colour){
                float cost = biggestClasher->getClashWithNeighbours(false);
                if(cost < bestCost){
                    
                    bestCost = cost;
                    bestCostElement = element;
                } /*else if(cost == bestCost){

                    bestCost = cost;
                    bestCostElement = element;
                    //NEED TO DO SOMETHING HERE BECAUSE 
                    //   TOO MANY ARE THE SAME COST MOVE
                    
                }*/
            }
            biggestClasher->actualPlacement = biggestClasher->colour;
            
        }
        //std::cout << biggestClasher->examID <<" " << bestCost << " " << bestCostElement << std::endl;

        if(bestCostElement != -1){
            biggestClasher->thirdStageColourChange(bestCostElement);
            biggestClasher->colour = bestCostElement;

            for(int i = 0; i < biggestClasher->conflicts.size(); ++i){
                Node * other = biggestClasher->conflicts.at(i)->getOtherNode(biggestClasher);
                other->setSecondStagePossible(graph->numberOfExams);
            }
        }

        alreadyChanged.insert(biggestClasher);

        
        ++count;    
        if(count%(graph->numberOfExams/(clearReset))==0){
            alreadyChanged.clear();
            //std::cout << "EMPTIED ALREADY CHANGED" << std::endl;
            if(clearReset > 6){
                --clearReset;
            }
        }

    }
    //graph->rotate();
    graph->printNormCost();
}

#endif