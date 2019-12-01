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
    int iterationsCounter_= 0;
    int k_;
    double t_ = 4000000.0;
    hillClimber(solution_t problem);
    hillClimber();

    void init();
    void gimmeSolution();

    
private:
    void getNeighbours();
    void getRandomNeighbours();
    void checkNeighbours();
    void checkNeighboursAnnealing();
    void parseSolution();
    double T();
    
};