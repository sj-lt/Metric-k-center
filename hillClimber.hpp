#pragma once
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <algorithm>
#include "problem.hpp"



class hillClimber : public solver_t{
public:
    std::list<std::vector<int>> neighbours_;
    
    hillClimber(solution_t problem);
    hillClimber();

    void init();
    void gimmeSolution();
private:
    void getNeighbours();
    void checkNeighbours();
    void parseSolution();
    
};