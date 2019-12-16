#include "genetic.hpp"
#include "json.hpp"
std::random_device rd;
std::mt19937 eng(rd());

void genetic::gimmeSolution()
{
    init();
    for(auto p : population_){
        problem_.warehouses=parseSolutionBool(p);
        fitnesses_.push_back((this->*fitFuncPtr_)(problem_.score()));
    }
    for (int i = 0; i < problem_.config_json["iterations"]; i++)
    {

        using json = nlohmann::json;
        json logMsg = {};

        (this->*selFuncPtr_)();


        for(auto p : population_){
            problem_.warehouses=parseSolutionBool(p);
            fitnesses_.push_back((this->*fitFuncPtr_)(problem_.score()));
        }

        auto score = problem_.score();
        logMsg["iteration"] = i;
        json neighbours(neighbours_);
        logMsg["neighbours"] = neighbours;
        logMsg["score"] = score;
        logMsg["bestScore"] = bestGlobalScore_;
        logMsg["config"] = problem_.config_json;
        if (logIteration_ == "true")
            logger(logMsg);
    }

    std::cout << max_tabu_size_ << std::endl;
    problem_.warehouses = bestWarehouses_;
    problem_.bestScore = bestGlobalScore_;
}

void genetic::init()
{
    std::cout << "start init" << std::endl;

    initPopulation_ = problem_.config_json["initPopulation"];
    crossover_probability_ = problem_.config_json["crossover_probability"];
    mutation_probability_ = problem_.config_json["mutation_probability"];

    fitFuncPtr_= fitnessMap_[problem_.config_json["fitFunc"]];
    mutFuncPtr_= mutationMap_[problem_.config_json["mutFunc"]];
    selFuncPtr_= selectionMap_[problem_.config_json["selFunc"]];
    crosFuncPtr_ = crossoverMap_[problem_.config_json["crosFunc"]];
    generatePopulation();

    std::cout << "finish init" << std::endl;
}
void genetic::generatePopulation()
{
    for (unsigned int i = 0; i < problem_.config_json["initPopulation"]; i++)
    {
        std::vector<int> new_sol;
        population_.push_back(genRandSolution());
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

// selection function from fitnesses
void genetic::selection()
{
    std::uniform_int_distribution<> distr(0,problem_.config_json["initPopulation"].size() - 1);
    for(unsigned int i = 0; i<problem_.config_json["initPopulation"];i++){
            int first = distr(eng);
            int second = distr(eng);
            fitnesses_[first] > fitnesses_[second] ? 
                parents_.push_back(population_.at(first)) 
                : parents_.push_back(population_.at(second));
    }
};

// crossover function from
void genetic::crossover(){
    std::uniform_int_distribution<> distrInt(0,problem_.problem->cities.size() - 1);
    std::uniform_real_distribution<> distrReal(0,1);
    for(unsigned int i = 0 ;i<parents_.size();i++){
        //TODO check number of warehouses on side need to mach else wrong solution
        double u = distrReal(eng);
        if(problem_.config_json["crossover_probability"]< u){
        int p1 = distrInt(eng);
        int p2 = distrInt(eng);
        if(p1 > p2)
         std::swap(p1,p2);
        solContainer newBorn1 = parents_[i];
        solContainer newBorn2 = parents_[i+1];
        for (int j = p1; j < p2; i++) {
            newBorn1[j] = parents_[i][j];
            newBorn2[j] = parents_[i][j];
        }

        children_.push_back();
        children_.push_back();
        }else
        {
        children_.push_back(parents_[i]);
        children_.push_back(parents_[i+1]);
        }
        

        i++;
    }


    int p1 = distrInt(eng);
    int p2 = distrInt(eng);
      auto new_a = a;
      auto new_b = b;
  
      for (int i = cross_point; i < (int)a.solution.size(); i++) {
        new_a.solution[i] = b.solution[i];
        new_b.solution[i] = a.solution[i];
      }
      return pair<alternative_solution_t, alternative_solution_t>(new_a, new_b);
};
// mutation function working on the specimen
void genetic::mutation()
{

    std::uniform_int_distribution<> distr(0, fitnesses_.size() - 1);
    int mut_point = distr(eng);
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
    solContainer  new_sol;
    for (unsigned int i = 0; i < problem_.problem->cities.size(); i++)
    {
        i == sol.front ? new_sol.at(i) = true : new_sol.at(i) = false;
    }
    return new_sol;
}