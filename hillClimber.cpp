#include "hillClimber.hpp"
#include "json.hpp"
hillClimber::hillClimber(solution_t problem)
{

    problem_ = problem;
}

void hillClimber::gimmeSolution()
{
    init();
    bool exit = false;

    for (int i = 0; i < problem_.config_json["iterations"]; i++)
    {
        using json = nlohmann::json;
	    json logMsg = {}; 

        if(problem_.config_json["random"] == "true")
            getRandomNeighbours();
        else
            getNeighbours();
        checkNeighbours();
        auto score = problem_.score();


        logMsg["iteration"]=i;

        json neighbours(neighbours_);
        logMsg["neighbours"]=neighbours;
        logMsg["score"]=score;
        logMsg["bestScore"]=problem_.bestScore;
        logMsg["config"]=problem_.config_json;

        logger(logMsg);

    }
}

void hillClimber::init()
{
    std::cout << "start init" << std::endl;
    std::random_device rd;                                                         // obtain a random number from hardware
    std::mt19937 eng(rd());                                                        // seed the generator
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size() - 1); // define the range

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

    std::cout << "finish init" << std::endl;
}
void hillClimber::checkNeighbours()
{
    std::vector<int> prevSolution = problem_.warehouses;
    double newScore;

    for (auto neighbour : neighbours_)
    {

        problem_.warehouses = neighbour;
        //check score for new warehouses

        newScore = problem_.score();
        if (newScore > problem_.bestScore)
            problem_.warehouses = prevSolution;

        else
            problem_.bestScore = newScore;
    }
}

void hillClimber::getNeighbours()
{
    std::vector<int> sol = problem_.warehouses;
    neighbours_.clear();
    
    for (int i = 0; i < problem_.numberOfWarehouses; i++)
    {

    std::cout << i<< std::endl;
        //sol=problem_.warehouses;
        sol.at(i) = ((problem_.warehouses.at(i) + 1) % problem_.problem->cities.size());
        neighbours_.push_back(sol);
        sol.at(i) = ((problem_.warehouses.at(i) - 1 + problem_.problem->cities.size()) % problem_.problem->cities.size());
        neighbours_.push_back(sol);
    }

}

void hillClimber::getRandomNeighbours()
{
    std::random_device rd;                                                         // obtain a random number from hardware
    std::mt19937 eng(rd());                                                        // seed the generator
    std::uniform_int_distribution<> distr(0, problem_.problem->cities.size() - 1); // define the range
    std::vector<int> sol;
    neighbours_.clear();

    int drawned = 0;
    sol = problem_.warehouses;
    std::vector<int> drawn = problem_.warehouses;
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
    neighbours_.push_back(sol);

}
