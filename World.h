#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/data/DataFile.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Organism> {

    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;
    emp::Ptr<emp::DataMonitor<double, emp::data::Histogram>> data_node_orgcoop;
    emp::Ptr<emp::DataMonitor<int>> data_node_count;

  public:

    OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {
        random_ptr.New(_random);
    }

    ~OrgWorld() {
      if(data_node_orgcoop) data_node_orgcoop.Delete();
      if(data_node_count) data_node_count.Delete();
    }

  emp::DataMonitor<int>& GetOrgCountDataNode() {
    if(!data_node_count) {
    data_node_count.New();
    OnUpdate([this](size_t){
    data_node_count -> Reset();
    for (size_t i = 0; i < pop.size(); i++)
        if(IsOccupied(i))
        data_node_count->AddDatum((1));
    });
  }
  return *data_node_count;
  }

  emp::DataMonitor<double, emp::data::Histogram>& GetOrgCoopValDataNode() {
    if (!data_node_orgcoop) {
      data_node_orgcoop.New();
      OnUpdate([this](size_t){
          data_node_orgcoop->Reset();
          for (size_t i = 0; i< pop.size(); i++)
          if (IsOccupied(i))
              data_node_orgcoop->AddDatum(pop[i]->GetCoopProb());
        });
    }
    data_node_orgcoop->SetupBins(0.0, 1.1, 11);
    return *data_node_orgcoop;
  }

  emp::DataFile & SetupOrgFile(const std::string & filename) {
    auto & file = SetupFile(filename);
    auto & node1 = GetOrgCountDataNode();
    auto & node = GetOrgCoopValDataNode();
    file.AddVar(update, "update", "Update");
    file.AddMean(node, "mean_coopval", "Average organism cooperation value");
    file.AddTotal(node1, "count", "Total number of organisms");
    file.AddHistBin(node, 0, "Hist_0.0", "Count for histogram bin 0.0 to <0.1");
    file.AddHistBin(node, 1, "Hist_0.1", "Count for histogram bin 0.1 to <0.2");
    file.AddHistBin(node, 2, "Hist_0.2", "Count for histogram bin 0.2 to <0.3");
    file.AddHistBin(node, 3, "Hist_0.3", "Count for histogram bin 0.3 to <0.4");
    file.AddHistBin(node, 4, "Hist_0.4", "Count for histogram bin 0.4 to <0.5");
    file.AddHistBin(node, 5, "Hist_0.5", "Count for histogram bin 0.5 to <0.6");
    file.AddHistBin(node, 6, "Hist_0.6", "Count for histogram bin 0.6 to <0.7");
    file.AddHistBin(node, 7, "Hist_0.7", "Count for histogram bin 0.7 to <0.8");
    file.AddHistBin(node, 8, "Hist_0.8", "Count for histogram bin 0.8 to <0.9");
    file.AddHistBin(node, 9, "Hist_0.9", "Count for histogram bin 0.9 to 1.0");


    file.PrintHeaderKeys();

    return file;
  }

  void Update() {
      emp::World<Organism>::Update();

      emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
      for(int i : schedule) {
        if(!IsOccupied(i)) {continue;}
        pop[i]->Process(100);
      }

      schedule = emp::GetPermutation(random, GetSize());
      for(int i : schedule) {
        if(!IsOccupied(i)) {continue;}
        emp::Ptr<Organism> offspring = pop[i]->CheckReproduction();
        if(offspring) {
            DoBirth(*offspring, i);
        }
      }
      
  }

};
#endif