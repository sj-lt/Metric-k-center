#pragma once
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <map>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include "json.hpp"
#include <iostream>

class evolutionStrategies
{
public:
    typedef std::pair<double, double> solution; //(x,y)
    typedef std::vector<solution> SolContainer;

    typedef std::vector<double> fitContainer;

    double tempFitnesses_;
    fitContainer fitnesses_;
    solution tempPopulation_;
    SolContainer population_;
    double ksi_;
    double cD_ = 0.82;
    double cI_ = 1 / 0.82;
    int successCounter_ = 0;
    int iterationsCounter_ = 0;
    int iterations_ = 0;
    evolutionStrategies(){};
    evolutionStrategies(int iterations);
    void init();
    solution gimmeSolution();

    double calculateTime();

protected:
    void modifyKsi();
    nlohmann::json buildLogMessage();
    void generatePopulation();
    void printPopulation();
    void calculateFitnesses();
    SolContainer genRandSolution();

    void mutation();
    double fitness(double score);

    bool iterationTerminator();

    double boothFunction(solution sol);
    double bealeFunction(solution sol);
};
