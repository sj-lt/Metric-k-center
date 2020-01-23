#include "genetic.hpp"
std::random_device rd;
std::mt19937 eng(rd());

void genetic::gimmeSolution()
{
    init();
    while ((this->*terminationFuncPtr_)())
    {
        if (problem_.config_json["debug"] == "true")
            printPopulation();
        if (problem_.config_json["debug"] == "true")
            std::cout << "starting selection" << std::endl;
        (this->*selectionFuncPtr_)();
        if (problem_.config_json["debug"] == "true")
            std::cout << "starting cross" << std::endl;
        (this->*crossoverFuncPtr_)();
        if (problem_.config_json["debug"] == "true")
            std::cout << "starting mutation" << std::endl;
        (this->*mutationFuncPtr_)();
        population_ = children_;
        calculateFitnesses();

        if (problem_.config_json["logIteration"] == "true")
        {
            logger(buildLogMessage());
        }
        iterationsCounter_++;
	//std::cout<<fitnesses_.at(std::distance(fitnesses_.begin(),std::max_element(fitnesses_.begin(), fitnesses_.end())))<<std::endl;
    }
    problem_.warehouses = getBest();
    problem_.bestScore = problem_.score();
    //problem_.warehouses = bestSolEver_.first;
    //problem_.bestScore = bestSolEver_.second;
}
void genetic::printPopulation()
{
    int i = 0;
    std::cout << iterationsCounter_ << std::endl;
    for (auto p : population_)
    {
        for (auto c : p)
        {
            std::cout << c << "|";
        }
        std::cout << fitnesses_.at(i) << std::endl;
        i++;
    }
}
nlohmann::json genetic::buildLogMessage()
{
    using json = nlohmann::json;
    json logMsg = {};
    logMsg["iteration"] = iterationsCounter_;
    json fitnesses(fitnesses_);
    logMsg["fitnesses"] = fitnesses;
    logMsg["score"] = bestScore_;
    logMsg["bestScore"] = bestSolEver_.second;
    logMsg["config"] = problem_.config_json;
    return logMsg;
}
void genetic::init()
{
    std::cout << "start init" << std::endl;
    iterationsCounter_ = 0;
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
    std::cout << "finish init" << std::endl;
}
//-----------------------------------------------FITNESS-----------------------------------------
double genetic::fitness(double goal)
{
    return 100000.0 / (1.0 + goal);
}
//-----------------------------------------------SELECTION-----------------------------------------
void genetic::tournamentSelection()
{
    std::uniform_int_distribution<> distr(0, initPopulation_ - 1);
    for (unsigned int i = 0; i < initPopulation_; i++)
    {
        int first = distr(eng);
        int second = distr(eng);
        fitnesses_[first] > fitnesses_[second] ? parents_.at(i) = population_.at(first)
                                               : parents_.at(i) = population_.at(second);
    }
};
void genetic::rouletteSelection()
{
    double sumFit = std::accumulate<>(fitnesses_.begin(),fitnesses_.end(),0.0);
    std::uniform_real_distribution<> distr(0, sumFit);

    for (unsigned int i = 0; i < initPopulation_; i++)
    {
        double rand = distr(eng);
        double sumTemp = 0.0;
        for(unsigned  int j = 0;j<fitnesses_.size();j++){
            sumTemp+=fitnesses_.at(j);
            if(rand<sumTemp){
                parents_.at(i)=population_.at(j);
                break;
            }
        }
    }
};
//-----------------------------------------------CROSSOVER-----------------------------------------
void genetic::twoPointCrossover()
{
    std::uniform_int_distribution<> distrInt(0, problem_.problem->cities.size() - 1);
    std::uniform_real_distribution<> distrReal(0, 1);

    for (unsigned int i = 0; i < parents_.size(); i += 2)
    {
        double u = distrReal(eng);
        if (crossover_probability_ > u)
        {
            int p1, p2;
            bool isEqual = false;
            while (!isEqual) //check number of warehouses inside need to mach else wrong solution
            {
                p1 = distrInt(eng);
                p2 = distrInt(eng);
                int pc1 = 0, pc2 = 0;

                if (p1 > p2)
                {
                    std::swap(p1, p2);
                }
                for (unsigned int j = p1; j < p2; j++)
                {
                    if (parents_.at(i).at(j))
                    {
                        pc1++;
                    }
                    if (parents_.at(i + 1).at(j))
                    {
                        pc2++;
                    }
                }
                if (pc1 == pc2)
                {
                    isEqual = true;
                }
            }
            altSolContainer newBorn1=parents_.at(i);
            altSolContainer newBorn2=parents_.at(i + 1);
            for (unsigned int j = p1; j < p2; j++)
            {
                newBorn1.at(j) = parents_.at(i + 1).at(j);
                newBorn2.at(j) = parents_.at(i).at(j);
            }
            children_.at(i) = newBorn1;
            children_.at(i + 1) = newBorn2;
        }
        else
        {
            children_.at(i) = parents_.at(i);
            children_.at(i + 1) = parents_.at(i + 1);
        }
    }
};
void genetic::onePointCrossover()
{
    std::uniform_int_distribution<> distrInt(0, problem_.problem->cities.size() - 1);
    std::uniform_real_distribution<> distrReal(0, 1);

    for (unsigned int i = 0; i < parents_.size(); i += 2)
    {
        double u = distrReal(eng);
        if (crossover_probability_ > u)
        {
            int p;
            bool isEqual = false;
            while (!isEqual) //check number of warehouses inside need to mach else wrong solution
            {
                p = distrInt(eng);
                int  pc1 = 0, pc2 = 0;

                for (unsigned int j = 0; j < p; j++)
                {
                    if (parents_.at(i).at(j))
                    {
                        pc1++;
                    }
                    if (parents_.at(i + 1).at(j))
                    {
                        pc2++;
                    }
                }
                if (pc1 == pc2)
                {
                    isEqual = true;
                }
            }
            altSolContainer newBorn1 = parents_.at(i);
            altSolContainer newBorn2 = parents_.at(i + 1);
            for (unsigned int j = 0; j < p; j++)
            {
                newBorn1.at(j) = parents_.at(i + 1).at(j);
                newBorn2.at(j) = parents_.at(i).at(j);
            }
            children_.at(i) = newBorn1;
            children_.at(i + 1) = newBorn2;
        }
        else
        {
            children_.at(i) = parents_.at(i);
            children_.at(i + 1) = parents_.at(i + 1);
        }
    }
};
//-----------------------------------------------MUTATION-----------------------------------------
//move warehouse to different location(random)
void genetic::twoPointSwapMutation()
{
    std::uniform_int_distribution<> distrNew(0, problem_.problem->cities.size() - 1);
    std::uniform_int_distribution<> distrOld(0, problem_.numberOfWarehouses - 1);

    std::uniform_real_distribution<> distrReal(0, 1);
    for (unsigned int i = 0; i < children_.size(); i++)
    {
        double u = distrReal(eng);
        if (mutation_probability_ > u)
        {
            int new_point = distrNew(eng);
            int old_point = distrOld(eng);
            int opc = 0, opi;
            for (unsigned int j = 0; j < problem_.problem->cities.size(); j++)
            {
                if (children_.at(i).at(j))
                {
                    if (opc == old_point)
                    {
                        children_.at(i).at(j) = false;
                        break;
                    }
                    opc++;
                }
            }
            while(children_.at(i).at(new_point)){
                new_point=(new_point+1)%(problem_.problem->cities.size() - 1);
            }
            children_.at(i).at(new_point) = true;
        }
    }
};
void genetic::generateRandomMutation()
{
    std::uniform_real_distribution<> distrReal(0, 1);
    for (unsigned int i = 0; i < children_.size(); i++)
    {
        double u = distrReal(eng);
        if (mutation_probability_ > u)
        {
            children_.at(i) = genRandSolution();
        }
    }
};
//-----------------------------------------------ITERATION-----------------------------------------
bool genetic::iterationTerminator()
{
    return iterationsCounter_ < problem_.config_json["iterations"] ? true : false;
};
bool genetic::standardDeviationTerminator()
{
    double expected = 1.0;
    double mean = std::accumulate(fitnesses_.begin(), fitnesses_.end(), 0.0) / fitnesses_.size();
    double sq_sum =
        inner_product(fitnesses_.begin(), fitnesses_.end(), fitnesses_.begin(), 0.0, std::plus<>(),
                      [mean](double const &x, double const &y) { return (x - mean) * (x - mean); });
    double sd = std::sqrt(sd / fitnesses_.size());
    return sd > expected ? true : false;
}
//-----------------------------------------------HELPERS-----------------------------------------
genetic::solContainer genetic::getBest()
{
    return parseSolutionBool(population_.at(std::distance(fitnesses_.begin(), std::max_element(fitnesses_.begin(), fitnesses_.end()))));
}
void genetic::generatePopulation()
{
    for (unsigned int i = 0; i < initPopulation_; i++)
    {
        population_.push_back(genRandSolution());
        if (!i)
        {
            problem_.warehouses = parseSolutionBool(population_.back());
            bestSolEver_.second = problem_.score();
            bestSolEver_.first = problem_.warehouses;
        }
    }
}
void genetic::calculateFitnesses()
{ 
    #pragma omp parallel for
    for (unsigned int i = 0; i < population_.size(); i++)
    {
        double score = problem_.scoreParallel(parseSolutionBool(population_.at(i)));
        fitnesses_.at(i) = (this->*fitnessFuncPtr_)(score);
    }

}
genetic::altSolContainer genetic::genRandSolution()
{
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size() - 1);
    solContainer sol(problem_.numberOfWarehouses, 0);
    int drawned = 0;
    solContainer drawn; 
    bool ifValid;
    for (unsigned int i = 0; i < problem_.numberOfWarehouses; i++)
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
genetic::solContainer genetic::parseSolutionBool(const altSolContainer &sol)
{
    int i = 0;
    solContainer new_sol;
    for (bool s : sol)
    {
        if (s)
            new_sol.push_back(i);
        i++;
    }
    return new_sol;
}
genetic::altSolContainer genetic::parseSolutionInt(solContainer &sol)
{
    altSolContainer new_sol(problem_.problem->cities.size(), false);
    for (int s : sol)
    {
        new_sol.at(s) = true;
    }
    return new_sol;
}
genetic::genetic(solution_t problem)
{
    problem_ = problem;
}
