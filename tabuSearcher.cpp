#include "tabuSearcher.hpp"

tabuSearcher::tabuSearcher(solution_t problem)
{
    problem_ = problem;
}


void tabuSearcher::gimmeSolution()
{
    init();
    bool exit = false;
    tabu_.push_back(problem_.warehouses);
    for (int i = 0; i < problem_.config_json["iterations"]; i++)
    {
        
        using json = nlohmann::json;
	    json logMsg = {}; 
        getNeighbours();
        checkTabu();
        checkNeighbours();
        auto score = problem_.score();
        logMsg["iteration"]=i;
        json neighbours(neighbours_);
        logMsg["neighbours"]=neighbours;
        logMsg["score"]=score;
        logMsg["bestScore"]=bestGlobalScore_;
        logMsg["config"]=problem_.config_json;

        logger(logMsg);
    }

    std::cout<<max_tabu_size_<<std::endl;
    problem_.warehouses=bestWarehouses_;
    problem_.bestScore=bestGlobalScore_;
}
void tabuSearcher::init()
{
    std::cout << "start init" << std::endl;
    std::random_device rd;                                                         // obtain a random number from hardware
    std::mt19937 eng(rd());                                                        // seed the generator
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size() - 1); // define the range
    max_tabu_size_=problem_.config_json["tabuSize"];
    problem_.warehouses.clear();
    problem_.warehouses.push_back(distr(eng));

    while (problem_.warehouses.size() != problem_.numberOfWarehouses)
    {
        int newRandom = distr(eng);
        if (std::find(problem_.warehouses.begin(), problem_.warehouses.end(), newRandom) == problem_.warehouses.end())
        {
            problem_.warehouses.push_back(newRandom);
        }
    }
    problem_.bestScore = problem_.score();
    bestGlobalScore_ = problem_.score();
    bestWarehouses_ = problem_.warehouses;
    std::cout << "finish init" << std::endl;
}

bool operator==(const std::vector<int> &a,
                const std::vector<int> &b)
{
    for (int i = 0; i < a.size(); i++)
    {
        if (a.at(i) != b.at(i))
            return false;
    }
    return true;
}
void tabuSearcher::checkTabu()
{

    neighbours_.erase(std::remove_if(neighbours_.begin(), neighbours_.end(),
                                     [&](const auto &te) {
                                         for (auto &e : tabu_)
                                             if (e == te)
                                                 return true;
                                         return false;
                                     }),
                      neighbours_.end());

}

void tabuSearcher::checkNeighbours()
{

    std::vector<int> newSolution;

    if (neighbours_.size() <= 0)
    {
            //finish
    }
    else
    {
        // find best neighbour
        problem_.warehouses = neighbours_.back();
        newSolution = problem_.warehouses;
        auto newScore = problem_.score();
        for (auto neighbour : neighbours_)
        {

            problem_.warehouses = neighbour;

           if (problem_.score() < newScore)
            {
                newScore = problem_.score();
                newSolution = problem_.warehouses;

            }
        }

        tabu_.push_back(newSolution);
      

        if (newScore < bestGlobalScore_)
        {
            bestGlobalScore_ = newScore;
            bestWarehouses_ = newSolution;
        }

        problem_.warehouses = newSolution;
        // update tabu
        if (tabu_.size() >= max_tabu_size_)
            tabu_.pop_front();
    }

}

void tabuSearcher::getNeighbours()
{
    
    std::vector<int> sol;
    neighbours_.clear();
    for (int i = 0; i < problem_.numberOfWarehouses; i++)
    {
        sol = problem_.warehouses;
        sol.at(i) = ((problem_.warehouses.at(i) + 1) % problem_.problem->cities.size());
        neighbours_.push_back(sol);
        sol.at(i) = ((problem_.warehouses.at(i) - 1 + problem_.problem->cities.size()) % problem_.problem->cities.size());
        neighbours_.push_back(sol);
    }


}
