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
        sendShips();
        iterationsCounter_++;
    }

    std::vector<solContainer> bestSolutions(numberOfIslands_);
    std::vector<double> bestScore(numberOfIslands_);
    #pragma omp parallel for
    for (unsigned int i = 0; i < numberOfIslands_; i++)
    {
        bestSolutions.at(i) = world_.at(i).getBest();
        bestScore.at(i) = problem_.scoreParallel(bestSolutions.at(i));
    }
    int i = std::distance(bestScore.begin(), std::max_element(bestScore.begin(), bestScore.end()));
    problem_.warehouses = bestSolutions.at(i);
    problem_.bestScore = bestScore.at(i);
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
    populationContainer ships(numberOfIslands_, altSolGroupContainer(numberOfIslands_));
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
    //std::cout << "start island init" << std::endl;
    initPopulation_ = problem_.config_json["initPopulation"];
    crossover_probability_ = problem_.config_json["crossoverProbability"];
    mutation_probability_ = problem_.config_json["mutationProbability"];
    fitnessFuncPtr_ = fitnessMap_[problem_.config_json["fitFunc"]];
    mutationFuncPtr_ = mutationMap_[problem_.config_json["mutFunc"]];
    selectionFuncPtr_ = selectionMap_[problem_.config_json["selFunc"]];
    crossoverFuncPtr_ = crossoverMap_[problem_.config_json["crosFunc"]];
    terminationFuncPtr_ = terminationMap_[problem_.config_json["termFunc"]];
    fitnesses_ = genetic::fitContainer(initPopulation_, 0.0);
    children_ = genetic::altSolGroupContainer(initPopulation_, genetic::altSolContainer(false));
    parents_ = children_;
    generatePopulation();
    calculateFitnesses();
    //std::cout << "finish island init" << std::endl;
}

geneticIsland::geneticIsland(solution_t problem)
{
    problem_ = problem;
}
