
#include <vector>
#include "combinations.hpp"

chaseSequence::chaseSequence(solution_t problem)
{
    n_ = problem.problem->cities.size();
    t_ = problem.numberOfWarehouses;
    s_ = n_ - t_;
    problem_ = problem;
}

void chaseSequence::gimmeSolution ()
{
    init();
    bool exit = false;
    while(!exit){
        checkAnwsers();
        switch(findAnwsers()){ 
            case 1: moveLeftOne();break; 
            case 2: moveRightTwo();break; 
            case 3: moveLeftTwo();break; 
            case 4: moveRightOne();break; 
            case 112: exit = true;break; 
        }
    }
    
}

void chaseSequence::init()
{
    problem_.bestScore = problem_.score();
    //set anwsers vector
    for (int j = 0; j < s_; j++)
    {
        anwsers_.push_back(0);
    }
    for (int j = s_; j < n_; j++)
    {
        anwsers_.push_back(1);
    }
    //set work vector
    for (int j = 0; j <= n_; j++)
    {
        work_.push_back(1);
    }
    if (s_ > 0)
        r_ = s_;
    else
        r_ = t_;
}



void chaseSequence::checkAnwsers()
{
    std::vector<int> prevSolution = problem_.warehouses;

    double newScore;

    int j = 0;
    for (int i = 0; i < anwsers_.size(); i++)
    {
        if (anwsers_.at(i))
        {
            problem_.warehouses.at(j) = i;
            j++;
        }
    }
   
    
    //check score for new warehouses
    newScore = problem_.score();
    if (newScore > problem_.bestScore) 
    {
        problem_.warehouses = prevSolution;
    }
    else
        problem_.bestScore = newScore;



}

int chaseSequence::findAnwsers()
{
    j_ = r_;
    while (work_.at(j_) != 1)
    {
        work_.at(j_) = 1;
        j_++;
    }
    if (j_ == n_)
    {
        return 112;
    }
    else
    {
        work_.at(j_) = 0;
        if (j_ % 2 == 0)
        {
            if (anwsers_.at(j_) == 0)
                return 1;//moveLeftOne();
            else
                return 2 ;//moveRightTwo();
        }
        else if (anwsers_.at(j_) == 0)
        {
            return 3 ;//moveLeftTwo();
        }
        else
            return 4 ;//moveRightOne();
    }
}



void chaseSequence::moveRightOne()
{
    anwsers_.at(j_ - 1) = 1;
    anwsers_.at(j_) = 0;

    if (r_ == j_) {
        if(j_ > 1){
            r_ = j_ - 1;}}
    else if (r_ == (j_ - 1))
        r_ = j_;

    
}
void chaseSequence::moveRightTwo()
{
    if (anwsers_.at(j_ - 2) != 0)
    {
        moveRightOne();
    }
    else
    {
        anwsers_.at(j_ - 2) = 1;
        anwsers_.at(j_) = 0;

        if (r_ == j_)
            r_ = std::max(j_ - 2, 1);
        else if (r_ == (j_ - 2))
            r_ = j_ - 1;

       
    }
}

void chaseSequence::moveLeftOne()
{
    anwsers_.at(j_) = 1;
    anwsers_.at(j_ - 1) = 0;

    if (r_ == j_) {
        if(j_ > 1){
            r_ = j_ - 1;}}
    else if (r_ == (j_ - 1))
        r_ = j_;

    
}
void chaseSequence::moveLeftTwo()
{
    if (anwsers_.at(j_ - 1) != 0)
    {
        moveLeftOne();
    }
    else
    {
        anwsers_.at(j_) = 1;
        anwsers_.at(j_ - 2) = 0;

        if (r_ == (j_ - 2))
            r_ = j_;
        else if (r_ == (j_ - 1))
            r_ = j_ - 2;

        
    }
}