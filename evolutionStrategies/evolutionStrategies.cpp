#include "evolutionStrategies.hpp"
#include <cmath>
evolutionStrategies::solution operator+(const evolutionStrategies::solution s, double f)
{
    evolutionStrategies::solution ret = s;
    ret.first += f;
    ret.second += f;
    return ret;
}
evolutionStrategies::solution evolutionStrategies::gimmeSolution()
{
    init();
    while (iterationTerminator())
    {
        //std::cout << "Start while" << std::endl;

        mutation();
        calculateFitnesses();
        if (tempFitnesses_ < fitnesses_.at(iterationsCounter_))
        {
            population_.at(iterationsCounter_ + 1) = tempPopulation_;
            fitnesses_.at(iterationsCounter_ + 1) = tempFitnesses_;
            successCounter_++;
            std::cout<<"better"<<std::endl;

        }
        else
        {
            population_.at(iterationsCounter_ + 1) = population_.at(iterationsCounter_);
            fitnesses_.at(iterationsCounter_ + 1) = fitnesses_.at(iterationsCounter_);
        }
        modifyKsi();
        iterationsCounter_++;
        //std::cout << "finish while" << std::endl;
    }
}
void evolutionStrategies::modifyKsi()
{
    if (!(iterationsCounter_ % 4))
    {
        if (successCounter_ > 1)
        {
            ksi_ *= cI_;
        }
        else if (successCounter_ < 1)
        {
            ksi_ *= cD_;
        }
        successCounter_ = 0;
            std::cout<<ksi_<<std::endl;

    }
}
void evolutionStrategies::init()
{
    std::cout << "Start init" << std::endl;
    if(!iterations_)
        iterations_ = 100;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd());
    std::uniform_real_distribution<double> distr(-10.0, 10.0);
    population_ = evolutionStrategies::SolContainer(iterations_);
    fitnesses_ = evolutionStrategies::fitContainer(iterations_, 0.0);
    iterationsCounter_ = 0;
    ksi_ = 0.1;
    population_.at(iterationsCounter_) = std::pair<double, double>(distr(eng), distr(eng));

    calculateFitnesses();
    fitnesses_.at(iterationsCounter_)=tempFitnesses_;
    std::cout<<population_.at(iterationsCounter_).first<<" | "
                <<population_.at(iterationsCounter_).second<<std::endl;
    std::cout << fitnesses_.at(iterationsCounter_) << std::endl;
    std::cout << "Finish init" << std::endl;
}

void evolutionStrategies::mutation()
{
    //std::cout << "Start mut" << std::endl;
    std::random_device rd;
    std::mt19937 eng(rd());
    std::normal_distribution<double> distr(0.0, 1.0);

    tempPopulation_ = population_.at(iterationsCounter_) + (ksi_ * distr(eng));

    if (tempPopulation_.first > 10)
        tempPopulation_.first = 10;
    else if (tempPopulation_.first < -10)
        tempPopulation_.first = -10;

    if (tempPopulation_.second > 10)
        tempPopulation_.first = 10;
    else if (tempPopulation_.second < -10)
        tempPopulation_.first = -10;
}

void evolutionStrategies::calculateFitnesses()
{
    //std::cout << "Start calcfit" << std::endl;
    tempFitnesses_ = boothFunction(tempPopulation_);

}

double evolutionStrategies::boothFunction(const evolutionStrategies::solution sol)
{
    //std::cout << "Start bootfunc" << std::endl;

    double x = sol.first;
    double y = sol.second;
    return ( pow((x + 2 * y - 7), 2) + pow((2 * x + y - 5), 2));
}
double evolutionStrategies::bealeFunction(const evolutionStrategies::solution sol)
{
    //std::cout << "Start bealeFunction" << std::endl;

    double x = sol.first;
    double y = sol.second;
    return (pow((1.5 - x + x * y), 2) + pow((2.25 - x + x * pow(y, 2)), 2) + pow((2.625 - x + x * pow(y, 3)), 2));
}

double evolutionStrategies::calculateTime()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    gimmeSolution();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000000.0;
}

bool evolutionStrategies::iterationTerminator()
{
    return (iterationsCounter_ < iterations_ - 1);
}
    evolutionStrategies::evolutionStrategies(int iterations){
        this->iterations_ = iterations;
    };
