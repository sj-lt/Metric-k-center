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

class genetic : public solver_t
{
public:
    typedef std::vector<bool> solContainer;
    typedef std::vector<std::vector<bool>> solGroupContainer;
    typedef std::vector<double> fitContainer;

    fitContainer fitnesses_;
    solGroupContainer population_;
    solGroupContainer parents_;
    solGroupContainer children_;
    int iterationsCounter_ = 0;
    int initPopulation_;
    double crossover_probability_;
    double mutation_probability_;
    std::pair<std::vector<int>, double> bestSolEver_;
    double bestScore_;
    genetic(solution_t problem);
    genetic();
    void init();
    void gimmeSolution();
    typedef void (genetic::*geneticVoidFunction)();
    typedef bool (genetic::*geneticBoolFunction)();
    typedef double (genetic::*geneticDoubleFunction)(double);

    std::unordered_map<std::string, geneticDoubleFunction>
        fitnessMap_{
            {"fitness", &genetic::fitness}};
    std::unordered_map<std::string, geneticVoidFunction>
        mutationMap_{
            {"twoPointSwapMutation", &genetic::twoPointSwapMutation}};
    std::unordered_map<std::string, geneticVoidFunction>
        selectionMap_{
            {"tournamentSelection", &genetic::tournamentSelection}};
    std::unordered_map<std::string, geneticVoidFunction>
        crossoverMap_{
            {"twoPointCrossover", &genetic::twoPointCrossover}};
    std::unordered_map<std::string, geneticBoolFunction>
        terminationMap_{
            {"iterationTerminator", &genetic::iterationTerminator}};
    geneticDoubleFunction fitnessFuncPtr_;
    geneticVoidFunction mutationFuncPtr_;
    geneticVoidFunction selectionFuncPtr_;
    geneticVoidFunction crossoverFuncPtr_;
    geneticBoolFunction terminationFuncPtr_;

private:
    nlohmann::json buildLogMessage();
    double fitness(double score);
    void generatePopulation();
    void calculateFitnesses();
    solContainer genRandSolution();
    void twoPointSwapMutation();
    void tournamentSelection();
    void twoPointCrossover();
    bool iterationTerminator();
    bool standardDeviationTerminator();
    std::vector<int> parseSolutionBool(const solContainer &sol);
    solContainer parseSolutionInt(std::vector<int> &sol);
};
