#pragma once
#include <vector>
#include <chrono>
#include "problem.hpp"


class chaseSequence: public solver_t{
public:
    std::vector<int> anwsers_;
    std::vector<int> work_;

    int j_,r_,n_,s_,t_;

    chaseSequence(solution_t problem);
    chaseSequence();

    void init();
    void gimmeSolution();
private:
    void checkAnwsers();
    int findAnwsers();


    void moveRightOne();
    void moveRightTwo();

    void moveLeftOne();
    void moveLeftTwo();
};