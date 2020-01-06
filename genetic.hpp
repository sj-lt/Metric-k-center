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
    std::pair<std::vector<int>,double> bestSolEver_;
    double bestScore_;
    genetic(solution_t problem);
    genetic();
    void init();
    void gimmeSolution();
    typedef void (genetic::*geneticVoidFunction)() ;
    typedef double (genetic::*geneticDoubleFunction)(double);

    std::unordered_map<std::string, geneticDoubleFunction>
        fitnessMap_{
            {"fitness", &fitness}};
    std::unordered_map<std::string, geneticVoidFunction>
        mutationMap_{
            {"mutation", &mutation}};
    ;
    std::unordered_map<std::string, geneticVoidFunction>
        selectionMap_{
            {"selection", &selection}};
    ;
    std::unordered_map<std::string, geneticVoidFunction>
        crossoverMap_{
            {"crossover", &crossover}};
    ;
    geneticDoubleFunction fitnessFuncPtr_;
    geneticVoidFunction mutationFuncPtr_;
    geneticVoidFunction selectionFuncPtr_;
    geneticVoidFunction crossoverFuncPtr_;

private:
    double fitness(double score);
    void generatePopulation();
    void calculateFitnesses();
    solContainer genRandSolution();
    void mutation();
    void selection();
    void crossover();
    std::vector<int> parseSolutionBool(const solContainer &sol);
    solContainer parseSolutionInt(std::vector<int> &sol);
};
