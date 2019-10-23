#pragma once
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <algorithm>
#include "problem.hpp"



class tabuSearcher{
public:
    std::list<std::vector<int>> neighbours_;
    std::list<std::vector<int>> tabu_;
    solution_t problem_;
    std::vector<int> bestWarehouses_;
    double bestGlobalScore_;
    int max_tabu_size_=10;
    tabuSearcher(solution_t problem);
    tabuSearcher();

    void init();
    void gimmeSolution();
    auto mesureTime();
private:
    void getNeighbours();
    void checkNeighbours();
    void checkTabu();
    void parseSolution();
    
};