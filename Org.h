#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double points;
        double coop_prob;
        double mut_rate = 0.2;
        emp::Ptr<emp::Random> random;

    public:
    Organism(emp::Ptr<emp::Random> _random, double _coop=0.0, double _points=0.0) :
        points(_points), coop_prob(_coop), random(_random) {;}

    void SetPoints(double _in) {points = _in;}
    void AddPoints(double _in) {points += _in;}

    double GetCoopProb() {return coop_prob;}
    void SetCoopProb(double _in) {coop_prob = _in;}

    void mutate() {
        coop_prob += random->GetRandNormal(0.0, mut_rate);
        if(coop_prob < 0) coop_prob = 0;
        else if (coop_prob > 1) coop_prob = 1;
    }

    emp::Ptr<Organism> CheckReproduction() {
        if(points>1000) {
            emp::Ptr<Organism> offspring = new Organism(*this);
            offspring->SetPoints(0);
            offspring->mutate();
            AddPoints(-1000);
            return offspring;
        }
        return nullptr;
    }

    void Process(int _points) {
        AddPoints(_points);
       
    }


};
#endif