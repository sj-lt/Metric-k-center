#pragma once
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <algorithm>
#include "problem.hpp"



class hillClimber{
public:
    std::list<std::vector<int>> neighbours_;
    solution_t problem_;
    
    hillClimber(solution_t problem);
    hillClimber();

    void init();
    void gimmeSolution();
    auto mesureTime();
private:
    void getNeighbours();
    void checkNeighbours();
    void parseSolution();
    
};