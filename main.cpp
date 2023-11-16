#include <string>
#include <vector>
#include <ctime>
#include <utility>

#include "Graph/graph.hpp"
#include "inputReader.hpp"
#include "WFC/WFCController.hpp"
#include "OptStage.hpp"

/*
    Running of the designated instance. 
*/
float TestRun(std::string url){
    Graph * graph = readTextFile(url);
    WFCController(graph);
    optstage(graph);
    return graph->normalisedCost(true);
}

/*
    Runs single instance once
*/
std::pair<float,float> benchMark(std::string url){
    std::pair<float, float> pair;
    int start = clock();
    pair.first = TestRun(url);
    int stop = clock();
    pair.second = (stop-start)/double(CLOCKS_PER_SEC) * 1000; 
    return pair;
}
   
/*
    Runs single instance N times and puts the results in file "filename"
*/
std::pair<float, float> benchmarkN(std::string url, int N, std::string fileName){
    float totalTime = 0;
    float totalCost = 0;
    float best = 10000;

    std::ofstream resultsFile("results/" + fileName);

    for(int i = 0; i < N; ++i){
        std::pair<float, float> res = benchMark(url);

        totalCost += res.first;
        totalTime += res.second;

        resultsFile << res.first << " " << res.second << std::endl;
    }
    std::pair<float, float> ret;
    ret.first = totalCost/N;
    ret.second = totalTime/N;
    return ret;
}

/*
    Runs all tests n times and returns an average cost and time
    Carter et al. Toronto Instances
    Bellio et al. ITC Instances
    Bellio et al. DS Instances
*/
void benchmarkAll(int n){

    std::pair<float, float> results;
    /*Classic Instances*/

    std::string CLASSICurl= "ExamSchedulingTestData/CarterEtAlInstances/";
    std::vector<std::string> classicInstances = {"car91", "car92", "ear83", "hec92", "kfu93", "lse91","pur93", "rye93", "sta83","tre92","uta92","ute92","yor83"};
    for(int j =0; j<13; ++j){
        //std::cout << "-----------------------------------" << std::endl;
        results = benchmarkN(CLASSICurl + classicInstances[j], n, classicInstances[j]);
        std::cout << classicInstances[j] << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }
    

    /*BELLIO ET ALL NEW ITC2007 INSTANCES*/
    std::string ITCurl = "ExamSchedulingTestData/BellioEtAlINstances/ITC2007_";
    for(int j = 1; j<= 12; ++j){
        results = benchmarkN(ITCurl + std::to_string(j) , n, "ITC2007_" + std::to_string(j));
        std::cout << "ITC2007_" << std::to_string(j) << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }
	
    /*BELLIO ET ALL NEW DC INSTANCES*/
    std::string DSurl= "ExamSchedulingTestData/BellioEtAlINstances/";
    std::vector<std::string> dsInstances = {"D1-2-17", "D5-1-17", "D5-1-18", "D5-2-17", "D5-2-18","D5-3-18", "D6-1-18", "D6-2-18"};
    for(int j = 0; j< 8; ++j){
        results = benchmarkN(DSurl + dsInstances[j], n, dsInstances[j]);
        std::cout << dsInstances[j] << " " << std::to_string(results.first) << " "  << std::to_string(results.second) << std::endl;
    }
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    benchmarkAll(50);

    return 0;
}
