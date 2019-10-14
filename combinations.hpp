#pragma once
#include <vector>
#include "problem.hpp"


class chaseSequence{
public:
    std::vector<int> anwsers_;
    std::vector<int> work_;
    solution_t problem_;

    int j_,r_,n_,s_,t_;

    chaseSequence(int n,int k,solution_t problem);
    chaseSequence();

    void init();
private:
    void checkAnwsers();
    void findAnwsers();

    void exit();

    void moveRightOne();
    void moveRightTwo();

    void moveLeftOne();
    void moveLeftTwo();
};