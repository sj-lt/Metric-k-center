#include "evolutionStrategies.cpp"
#include <cstdlib>
/*
	build --->  
				mkdir build | g++ main.cpp -std=c++17 -o ./build/eS
	run evolutionStrategy --> 
				./build/eS 
*/
void plotHistogram(evolutionStrategies *solver);
int main(int argc, char **argv)
{
    evolutionStrategies solver;
    if(argc>1)
        solver = evolutionStrategies(atoi(argv[1]));
    else
        solver = evolutionStrategies();
    
    double spend = solver.calculateTime();
    std::cout<<spend<<std::endl;
    std::cout<<solver.fitnesses_.back()<<std::endl;
    std::cout<<solver.population_.back().first<<" | "<<solver.population_.back().second<<std::endl;
};



void plotHistogram(evolutionStrategies *solver)
{

};
