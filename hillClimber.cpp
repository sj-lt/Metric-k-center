#include "hillClimber.hpp"

hillClimber::hillClimber(solution_t problem)
{

    problem_ = problem;
}

auto hillClimber::mesureTime(){
 	auto t1 = std::chrono::high_resolution_clock::now();
    gimmeSolution();//change function
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    return duration;
};

void hillClimber::gimmeSolution ()
{
    init();
    bool exit = false;
    for(int i=0 ;i<10;i++){
        getNeighbours();
        checkNeighbours();
    }
    
    
}

void hillClimber::init()
{
    std::cout<<"start init"<<std::endl;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size()-1); // define the range
    

    problem_.warehouses.clear();
    problem_.warehouses.push_back(distr(eng));

    while(problem_.warehouses.size()!=problem_.numberOfWarehouses){
        int newRandom = distr(eng);
        if(std::find(problem_.warehouses.begin(), problem_.warehouses.end(), newRandom) == problem_.warehouses.end()) {   
        problem_.warehouses.push_back(newRandom);
        }
    }
    problem_.bestScore = problem_.score();
std::cout<<"finish init"<<std::endl;

}
void hillClimber::checkNeighbours()
{
    std::vector<int> prevSolution = problem_.warehouses;
    double newScore;
for(auto neighbour : neighbours_){
        
    //std::cout<<"  neighbour: " <<neighbour.at(0)<<" "<<neighbour.at(1)<<std::endl;
}
    for(auto neighbour : neighbours_){
        
        
        problem_.warehouses = neighbour;
            //check score for new warehouses
        
        newScore = problem_.score();

        //std::cout<<"  Score: " <<newScore<<std::endl;
        if (newScore > problem_.bestScore) 
        {
            problem_.warehouses = prevSolution;
        }
        else
            problem_.bestScore = newScore;

    }

}


void hillClimber::getNeighbours()
{
    std::vector<int> sol;
    std::cout<<"  ------------------------------------------------------ " <<std::endl;
    std::cout<<"  warehouse: " <<problem_.warehouses.at(0)<<" "<<problem_.warehouses.at(1)<<std::endl;
    neighbours_.clear();
    for(int i=0;i<problem_.numberOfWarehouses;i++){
        sol=problem_.warehouses;
        sol.at(i)=((problem_.warehouses.at(i)+1)%problem_.problem->cities.size());
        neighbours_.push_back(sol);
        sol.at(i)=((problem_.warehouses.at(i)-1 + problem_.problem->cities.size())%problem_.problem->cities.size());
        neighbours_.push_back(sol);
       
    }

    for(auto neighbour : neighbours_){
        
    std::cout<<"  neighbour: " <<neighbour.at(0)<<" "<<neighbour.at(1)<<std::endl;
}
}




