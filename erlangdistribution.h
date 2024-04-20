#ifndef ERLANGDISTRIBUTION_H
#define ERLANGDISTRIBUTION_H

#include <QObject>

class ErlangDistribution : public QObject
{
    Q_OBJECT
public:
    explicit ErlangDistribution(int kk, double llambda, QObject *parent = nullptr);
    ~ErlangDistribution();
    double generate();
private:
    int k;
    double lambda;

signals:

};

#endif // ERLANGDISTRIBUTION_H
