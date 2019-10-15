
#include <vector>
#include "combinations.hpp"

chaseSequence::chaseSequence(solution_t problem)
{
    n_ = problem.problem->cities.size();
    t_ = problem.numberOfWarehouses;
    s_ = n_ - t_;
    problem_ = problem;
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
    checkAnwsers();
}
void chaseSequence::checkAnwsers()
{
    //std::cout<<"STARTING checkAnwsers*-------------------------------"<<std::endl;
    std::vector<int> prevSolution = problem_.warehouses;
    ;
    double newScore;

    //std::cout<<std::endl<<"best score: "<<problem_.bestScore<<std::endl; 
    int j = 0;
    for (int i = 0; i < anwsers_.size(); i++)
    {
        //std::cout<<anwsers_.at(i);
        if (anwsers_.at(i))
        {
            problem_.warehouses.at(j) = i;
            j++;
        }
    }
    for (auto w : problem_.warehouses){
        std::cout<<w;
    }
    
    //check score for new warehouses
    newScore = problem_.score();
    std::cout<<"  Score: " <<newScore<<std::endl;
    if (newScore > problem_.bestScore) 
    {
        problem_.warehouses = prevSolution;
    }
    else
        problem_.bestScore = newScore;

    findAnwsers();
}
void chaseSequence::findAnwsers()
{
    j_ = r_;
    while (work_.at(j_) != 1)
    {
        work_.at(j_) = 1;
        j_++;
    }
    if (j_ == n_)
    {
        exit();
    }
    else
    {
        work_.at(j_) = 0;
        if (j_ % 2 == 0)
        {
            if (anwsers_.at(j_) == 0)
                moveLeftOne();
            else
                moveRightTwo();
        }
        else if (anwsers_.at(j_) == 0)
        {
            moveLeftTwo();
        }
        else
            moveRightOne(); /**/
    }
}

void chaseSequence::exit()
{
    using namespace std;
    //cout<<problem_.bestScore<<endl;
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

    checkAnwsers();
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

        checkAnwsers();
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

    checkAnwsers();
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

        checkAnwsers();
    }
}