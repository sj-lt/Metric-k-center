#include "genetic.hpp"
#include "json.hpp"
std::random_device rd;
std::mt19937 eng(rd());

void genetic::gimmeSolution()
{
    init();
    for (int i = 0; i < problem_.config_json["iterations"]; i++)
    {
        //randoms from firsdt try lost in bestscore
        using json = nlohmann::json;
        json logMsg = {};
        //select parents
        (this->*selectionFuncPtr_)();
        (this->*crossoverFuncPtr_)();
        (this->*mutationFuncPtr_)();
        population_ = children_;
        calculateFitnesses();

        logMsg["iteration"] = i;
        json fitnesses(fitnesses_);
        logMsg["fitnesses"] = fitnesses;
        logMsg["score"] = bestScore_;
        logMsg["bestScore"] = bestSolEver_.second;
        logMsg["config"] = problem_.config_json;
        if (logIteration_ == "true")
            logger(logMsg);
    }

    problem_.warehouses = bestSolEver_.first;
    problem_.bestScore = bestSolEver_.second;
}

void genetic::init()
{
    std::cout << "start init" << std::endl;

    initPopulation_ = problem_.config_json["initPopulation"];
    crossover_probability_ = problem_.config_json["crossover_probability"];
    mutation_probability_ = problem_.config_json["mutation_probability"];

    fitnessFuncPtr_ = fitnessMap_[problem_.config_json["fitFunc"]];
    mutationFuncPtr_ = mutationMap_[problem_.config_json["mutFunc"]];
    selectionFuncPtr_ = selectionMap_[problem_.config_json["selFunc"]];
    crossoverFuncPtr_ = crossoverMap_[problem_.config_json["crosFunc"]];
    generatePopulation();
    calculateFitnesses();
    std::cout << "finish init" << std::endl;
}
void genetic::generatePopulation()
{
    for (unsigned int i = 0; i < problem_.config_json["initPopulation"]; i++)
    {
        population_.push_back(genRandSolution());
        if (!i)
        {
            problem_.warehouses = parseSolutionBool(population_.back);
            bestSolEver_.second=problem_.score();
            bestSolEver_.first=problem_.warehouses;
        }
    }
}
void genetic::calculateFitnesses()
{

    for (int i = 0; i < population_.size(); i++)
    {
        problem_.warehouses = parseSolutionBool(population_[i]);
        fitnesses_.push_back((this->*fitnessFuncPtr_)(problem_.score()));
        if (fitnesses_.back < bestSolEver_.second)
        {
            bestSolEver_.second = problem_.score();
            bestSolEver_.first = problem_.warehouses;
        }
    }
}
std::vector<bool> genetic::genRandSolution()
{
    // seed the generator
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size() - 1); // define the range
    std::vector<int> sol;
    int drawned = 0;
    std::vector<int> drawn;
    bool ifValid;
    for (int i = 0; i < problem_.numberOfWarehouses; i++)
    {
        do
        {
            ifValid = true;
            drawned = distr(eng);
            for (int number : drawn)
                if (number == drawned)
                    ifValid = false;

        } while (!ifValid);
        drawn.push_back(drawned);
        sol.at(i) = drawned;
    }
    return parseSolutionInt(sol);
};
double genetic::fitness(double goal)
{
    return 1000.0 / (1.0 + goal);
}


void genetic::selection()
{
    std::uniform_int_distribution<> distr(0, problem_.config_json["initPopulation"].size() - 1);
    for (unsigned int i = 0; i < problem_.config_json["initPopulation"]; i++)
    {
        int first = distr(eng);
        int second = distr(eng);
        fitnesses_[first] > fitnesses_[second] ? parents_.push_back(population_.at(first))
                                               : parents_.push_back(population_.at(second));
    }
};

void genetic::crossover()
{
    std::uniform_int_distribution<> distrInt(0, problem_.problem->cities.size() - 1);
    std::uniform_real_distribution<> distrReal(0, 1);
    for (unsigned int i = 0; i < parents_.size(); i += 2)
    {
        //TODO
        double u = distrReal(eng);
        if (problem_.config_json["crossover_probability"] < u)
        {
            int p1 = distrInt(eng);
            int p2 = distrInt(eng);
            bool isEqual = false;
            while (!isEqual) //check number of warehouses inside need to mach else wrong solution
            {
                if (p1 > p2)
                    std::swap(p1, p2);
                int p1 = 0, p2 = 0;
                for (int j = p1; j < p2; i++)
                {
                    if (parents_[i][j])
                        p1++;
                    if (parents_[i + 1][j])
                        p2++;
                }
                if (p1 == p2)
                    isEqual = true;
            }

            solContainer newBorn1 = parents_[i];
            solContainer newBorn2 = parents_[i + 1];
            for (int j = p1; j < p2; i++)
            {
                newBorn1[j] = parents_[i][j];
                newBorn2[j] = parents_[i][j];
            }

            children_.push_back(newBorn1);
            children_.push_back(newBorn2);
        }
        else
        {
            children_.push_back(parents_[i]);
            children_.push_back(parents_[i + 1]);
        }
    }
};
//move warehouse to different location(random)
void genetic::mutation()
{
    std::uniform_int_distribution<> distrNew(0, problem_.problem->cities.size() - 1);
    std::uniform_int_distribution<> distrOld(0, problem_.numberOfWarehouses - 1);
    std::uniform_real_distribution<> distrReal(0, 1);
    for (unsigned int i = 0; i < parents_.size(); i++)
    {
        double u = distrReal(eng);
        if (problem_.config_json["mutation_probability"] < u)
        {
            int new_point = distrNew(eng);
            int old_point = distrOld(eng);
            int opc = 0, opi;
            for (unsigned int j = 0; j < problem_.problem->cities.size(); i++)
            {
                if (parents_[i][j])
                {
                    if (opc == old_point)
                    {
                        parents_[i][j] = false;
                        break;
                    }
                    opc++;
                }
            }
            parents_[i][new_point] = true;
        }
    }
};
std::vector<int> genetic::parseSolutionBool(const solContainer &sol)
{
    int i = 0;
    std::vector<int> new_sol;
    for (bool s : sol)
    {
        if (s)
            new_sol.push_back(i);
        i++;
    }
    return new_sol;
}
genetic::solContainer genetic::parseSolutionInt(std::vector<int> &sol)
{
    int i = 0;
    std::sort(sol.begin(), sol.end());
    solContainer new_sol;
    for (unsigned int i = 0; i < problem_.problem->cities.size(); i++)
    {
        i == sol.front ? new_sol.at(i) = true : new_sol.at(i) = false;
    }
    return new_sol;
}