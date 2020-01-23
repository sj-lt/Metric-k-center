#pragma once
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <map>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include "problem.hpp"
#include "json.hpp"
#include "genetic.hpp"

class geneticIsland  : public genetic
{
public:
    void init();
    void gimmeSolution();
    geneticIsland(solution_t problem);
    geneticIsland(){};
};
class geneticWorld : public solver_t
{
public:
    typedef std::vector<geneticIsland> islandContainer;
    typedef std::vector<std::vector<bool>> altSolGroupContainer;
    typedef std::vector<altSolGroupContainer> populationContainer;

    islandContainer world_;
    int iterationsCounter_ = 0;

    int numberOfIslands_;
    int numberOfPeriods_;
    int numberOfEmigrants_;
    int exchangePeriod_;
    bool randomEmigrant_;
    bool randomReplace_;
    std::pair<solContainer, double> bestSolEver_;
    double bestScore_;
    geneticWorld(solution_t problem);
    geneticWorld();

    void init();
    void gimmeSolution();

private:
    nlohmann::json buildLogMessage();
    void sendShips();
    void sortPopulation();
    bool iterationTerminator();

};
