#include "erlangdistribution.h"
#include "cmath"
#include "QDebug"

ErlangDistribution::ErlangDistribution(int kk, double llambda, QObject *parent) : k{kk}, lambda{llambda}, QObject(parent)
{

}
ErlangDistribution::~ErlangDistribution()
{

}

double ErlangDistribution::generate()
{
//    srand(time(NULL));

    double rand_value;
    int precision {6};
    double min {0.00001};
    double max {0.99999};
    rand_value = rand() % (int)pow(10, precision); // получить случайное число как целое число с порядком precision
    rand_value = min + (rand_value / pow(10, precision)) * (max - min); // получить вещественное число

    double sum = 0.0;
    for(int i = 1; i <= k; i++)
    {
        sum+=std::log(1-rand_value);
    }
    return 1.0/(lambda*k)*sum;
}
