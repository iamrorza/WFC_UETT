#include <string>
#include <vector>
#include <ctime>
#include <utility>

#include "Graph/graph.hpp"
#include "inputReader.hpp"
#include "WFC/WFCController.hpp"
#include "OptStage.hpp"
#include "recordKeeper.hpp"

void Run(std::string url){
    Graph * graph = readTextFile(url);
    WFCController2(graph);
    graph->printNormCost();
    //ThirdStage(graph);
    delete graph;
}

float TestRun(std::string url){
    Graph * graph = readTextFile(url);
    WFCController3(graph);
    optstage(graph);
    return graph->normalisedCost(true);
}

std::pair<float,float> benchMark(std::string url, bool cost){
    std::pair<float, float> pair;
    int start = clock();
    pair.first = TestRun(url);
    int stop = clock();
    pair.second = (stop-start)/double(CLOCKS_PER_SEC) * 1000; 
    return pair;
}
   

std::pair<float, float> benchmarkN(std::string url, int N){
    float totalTime = 0;
    float best = 10000;
    for(int i = 0; i < N; ++i){
        std::pair<float, float> res = benchMark(url, false);

        if(res.first < best){
            best = res.first;
        }
        totalTime += res.second;
    }
    std::pair<float, float> ret;
    ret.first = best;
    ret.second = totalTime/N;
    return ret;
}



void benchmarkAll(int n){

    std::pair<float, float> results;
    /*Classic Instances*/

    std::string CLASSICurl= "ExamSchedulingTestData/CarterEtAlInstances/";
    std::vector<std::string> classicInstances = {"car91", "car92", "ear83", "hec92", "kfu93", "lse91","pur93", "rye93", "sta83","tre92","uta92","ute92","yor83"};
    for(int j = 0; j< 13; ++j){
        //std::cout << "-----------------------------------" << std::endl;
        results = benchmarkN(CLASSICurl + classicInstances[j], n);
        std::cout << classicInstances[j] << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }
    

    /*BELLIO ET ALL NEW ITC2007 INSTANCES*/
    std::string BETurl = "ExamSchedulingTestData/BellioEtAlInstances/ITC2007_";
    for(int j = 1; j<= 12; ++j){
        results = benchmarkN(BETurl + std::to_string(j) , n);
        std::cout << "ITC2007_" << std::to_string(j) << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }

    /*BELLIO ET ALL NEW DC INSTANCES*/
    std::string DSurl= "ExamSchedulingTestData/BellioEtAlInstances/";
    std::vector<std::string> dsInstances = {"D1-2-17", "D5-1-17", "D5-1-18", "D5-2-17", "D5-2-18","D5-3-18", "D6-1-18", "D6-2-18"};
    for(int j = 0; j< 8; ++j){
        results = benchmarkN(DSurl + dsInstances[j], n);
        std::cout << dsInstances[j] << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }

    
}


int main(int argc, char *argv[]){
    srand(time(NULL));

    std::pair<float, float> results;
    /*for(int  i = 0; i < argc; ++i)std::cout << argv[i] << std::endl;
    
    std::string url = argv[1];
    int number = std::stoi(argv[2]);
    */

    results = benchmarkN("ExamSchedulingTestData/CarterEtAlInstances/kfu93", 1);
    std::cout << results.first << " " << results.second << std::endl;
    
   
    //benchmarkDS(1);
    //benchmarkAll(1);

    return 0;
}