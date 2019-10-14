
#include <vector>
#include "combinations.hpp"

chaseSequence::chaseSequence(int n,int k,solution_t problem){
    n_=n;
    t_=k;
    s_=n-k;
    problem_=problem;
}

void chaseSequence::init(){
    problem_.bestScore=problem_.score();
    for(int j=0 ;j<s_;j++){
        anwsers_.push_back(0);
    }
    for(int j=s_ ;j<n_;j++){
        anwsers_.push_back(1);
    }
    for(int j=0 ;j<=n_;j++){
        work_.push_back(1);
    }
    if(s_>0)
        r_=s_;
    else
        r_=t_;    
    checkAnwsers();
}
void chaseSequence::checkAnwsers(){
    std::vector<int> tempSolution = problem_.warehouses;;
    double tempScore ;
    for(int i=0;i<anwsers_.size();i++){
        if(anwsers_.at(i))
            problem_.warehouses.at(i);
    }
    if(tempScore=problem_.score()>problem_.bestScore) //check if buggy
        problem_.warehouses=tempSolution;
    else
        problem_.bestScore=tempScore;

    findAnwsers();
}
void chaseSequence::findAnwsers(){
    j_=r_;
    while(work_.at(0)!=1){
        work_.at(j_) = 1;
        j_++;
    }
    if(j_==n_){
        exit();
    }
    work_.at(j_) = 0;
    if(j_%2==0){
        if(anwsers_.at(j_)==0)
            moveLeftOne();
        else
            moveRightTwo();
        
    }
    else if(anwsers_.at(j_)==0){
        moveLeftTwo();
    }
    else
        moveRightOne();
    
}

void chaseSequence::exit(){
    using namespace std;
    cout<<problem_.bestScore<<endl;
}

void chaseSequence::moveRightOne(){
    anwsers_.at(j_-1) = 1;
    anwsers_.at(j_) = 0;

    if(r_==j_ && j_>1)
        r_=j_-1;
    else if(r_>(j_-1))
        r_=j_;
    
    checkAnwsers();
}
void chaseSequence::moveRightTwo(){
    if(anwsers_.at(j_-2)!=0){
        moveLeftOne();
    }
    else{
    anwsers_.at(j_-2) = 1;
    anwsers_.at(j_) = 0;

    if(r_==j_ )
        r_=std::max(j_-2,1);
    else if(r_==(j_-2))
        r_=j_-1;
    
    checkAnwsers();
    }
}

void chaseSequence::moveLeftOne(){
    anwsers_.at(j_) = 1;
    anwsers_.at(j_-1) = 0;

    if(r_==j_ && j_>1 )
        r_=j_-1;
    else if(r_==(j_-1))
        r_=j_;
    
    checkAnwsers();
}
void chaseSequence::moveLeftTwo(){
    if(anwsers_.at(j_-1)!=0){
        moveLeftOne();
    }
    else{
    anwsers_.at(j_) = 1;
    anwsers_.at(j_-2) = 0;

    if(r_==(j_-2) )
        r_=j_;
    else if(r_==(j_-1))
        r_=j_-2;
    
    checkAnwsers();
    }
}