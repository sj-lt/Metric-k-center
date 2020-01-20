#include "geneticIslands.hpp"

void geneticWorld::gimmeSolution()
{
    init();
    while (iterationTerminator())
    {

        for (unsigned int i = 0; i < exchangePeriod_; i++)
        {
            #pragma omp parallel for
            for (unsigned int j = 0; j < numberOfIslands_; j++)
                world_.at(j).gimmeSolution();
        }
        std::cout << "asd" << std::endl;
        //sortPopulation();
        sendShips();
        iterationsCounter_++;
        //std::cout<<fitnesses_.at(std::distance(fitnesses_.begin(),std::max_element(fitnesses_.begin(), fitnesses_.end())))<<std::endl;
    }
    //----------------------TODO GET BEST OD ISLANDS
    // std::vector<std::vector<int>> bestSolutions(numberOfIslands_);
    // for (unsigned int i = 0; i < numberOfIslands_; i++)
    // {
    //     bestSolutions.at(i) = world_.at(i).getBest();
    // }

    //problem_.warehouses = parseSolutionBool(population_.at(std::distance(fitnesses_.begin(), std::max_element(fitnesses_.begin(), fitnesses_.end()))));
    //problem_.bestScore = problem_.score();
    //problem_.warehouses = bestSolEver_.first;
    //problem_.bestScore = bestSolEver_.second;
}
//-----------------------------------------------GENETIC WORLD-----------------------------------------

void geneticWorld::init()
{
    std::cout << "start world init" << std::endl;
    iterationsCounter_ = 0;
    numberOfPeriods_ = problem_.config_json["numberOfPeriods"];
    exchangePeriod_ = problem_.config_json["exchangePeriod"];
    numberOfIslands_ = problem_.config_json["numberOfIslands"];
    numberOfEmigrants_ = problem_.config_json["numberOfEmigrants"];
    randomEmigrant_ = problem_.config_json["randomEmigrant"] == "false" ? false : true;
    randomReplace_ = problem_.config_json["randomReplace"] == "false" ? false : true;
    world_ = islandContainer(numberOfIslands_, geneticIsland());
    for (unsigned int i = 0; i < numberOfIslands_; i++)
    {
        world_.at(i).problem_ = problem_;
        world_.at(i).init();
    }
    std::cout << "finish world init" << std::endl;
}

void geneticWorld::sendShips()
{
    populationContainer ships(numberOfIslands_, solGroupContainer(numberOfIslands_));
    for (unsigned int i = 0; i < numberOfIslands_; i++)
    {
        if (randomEmigrant_)
        {
            for (unsigned int j = 0; j < numberOfIslands_; j++)
            {
                if (i != j)
                    ships.at(i).at(j) = world_.at(j).population_.at(0);
            }
        }
    }
    for (unsigned int i = 0; i < numberOfIslands_; i++)
    {
        if (randomReplace_)
        {
            for (unsigned int j = 0; j < (numberOfIslands_); j++)
            {
                if (i != j)
                    world_.at(i).population_.at(j) = ships.at(i).at(j);
            }
        }
    }
}

void geneticWorld::sortPopulation()
{
    for (unsigned int i = 0; i < numberOfIslands_; i++)
    {
        //sort
    }
}

bool geneticWorld::iterationTerminator()
{
    return iterationsCounter_ < numberOfPeriods_ ? true : false;
};
geneticWorld::geneticWorld(solution_t problem)
{
    problem_ = problem;
}

//-----------------------------------------------GENETIC ISLAND-----------------------------------------
void geneticIsland::gimmeSolution()
{
    (this->*selectionFuncPtr_)();
    (this->*crossoverFuncPtr_)();
    (this->*mutationFuncPtr_)();
    population_ = children_;
    calculateFitnesses();
}
void geneticIsland::init()
{
    std::cout << "start island init" << std::endl;
    initPopulation_ = problem_.config_json["initPopulation"];
    crossover_probability_ = problem_.config_json["crossoverProbability"];
    mutation_probability_ = problem_.config_json["mutationProbability"];
    fitnessFuncPtr_ = fitnessMap_[problem_.config_json["fitFunc"]];
    mutationFuncPtr_ = mutationMap_[problem_.config_json["mutFunc"]];
    selectionFuncPtr_ = selectionMap_[problem_.config_json["selFunc"]];
    crossoverFuncPtr_ = crossoverMap_[problem_.config_json["crosFunc"]];
    terminationFuncPtr_ = terminationMap_[problem_.config_json["termFunc"]];
    fitnesses_ = genetic::fitContainer(initPopulation_, 0.0);
    children_ = genetic::solGroupContainer(initPopulation_, genetic::solContainer(false));
    parents_ = children_;
    generatePopulation();
    calculateFitnesses();
    std::cout << "finish island init" << std::endl;
}
std::vector<int> geneticIsland::getBest()
{
    return std::vector<int>(1, 0);
}
geneticIsland::geneticIsland(solution_t problem)
{
    problem_ = problem;
}
