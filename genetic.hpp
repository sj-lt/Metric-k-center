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
    typedef std::vector<bool> altSolContainer;
    typedef std::vector<altSolContainer> altSolGroupContainer;
    typedef std::vector<double> fitContainer;

    fitContainer fitnesses_;
    altSolGroupContainer population_;
    altSolGroupContainer parents_;
    altSolGroupContainer children_;
    int iterationsCounter_ = 0;
    int initPopulation_;
    double crossover_probability_;
    double mutation_probability_;
    std::pair<solContainer, double> bestSolEver_;
    double bestScore_;
    genetic(solution_t problem);
    genetic(){};
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
            {"twoPointSwapMutation", &genetic::twoPointSwapMutation},
            {"generateRandomMutation", &genetic::generateRandomMutation}};
    std::unordered_map<std::string, geneticVoidFunction>
        selectionMap_{
            {"tournamentSelection", &genetic::tournamentSelection},
            {"rouletteSelection", &genetic::rouletteSelection}};
    std::unordered_map<std::string, geneticVoidFunction>
        crossoverMap_{
            {"twoPointCrossover", &genetic::twoPointCrossover},
            {"onePointCrossover", &genetic::onePointCrossover}};
    std::unordered_map<std::string, geneticBoolFunction>
        terminationMap_{
            {"iterationTerminator", &genetic::iterationTerminator},
            {"standardDeviationTerminator", &genetic::standardDeviationTerminator}
            };
    geneticDoubleFunction fitnessFuncPtr_;
    geneticVoidFunction mutationFuncPtr_;
    geneticVoidFunction selectionFuncPtr_;
    geneticVoidFunction crossoverFuncPtr_;
    geneticBoolFunction terminationFuncPtr_;
    solContainer getBest();

protected:
    nlohmann::json buildLogMessage();
    void generatePopulation();
    void printPopulation();
    void calculateFitnesses();
    altSolContainer genRandSolution();

    void twoPointSwapMutation();
    void generateRandomMutation();

    double fitness(double score);

    void tournamentSelection();
    void rouletteSelection();

    void twoPointCrossover();
    void onePointCrossover();

    bool iterationTerminator();
    bool standardDeviationTerminator();

    solContainer parseSolutionBool(const altSolContainer &sol);
    altSolContainer parseSolutionInt(solContainer &sol);
};
