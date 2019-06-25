/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "lexicase.h"

namespace FT{


    namespace Sel{

        Lexicase::Lexicase(bool surv){ name = "lexicase"; survival = surv; }
        
        Lexicase::~Lexicase(){}
        
        vector<size_t> Lexicase::select(CPopulation& pop, const CParameters& params)
        {
            /*! Selection according to lexicase selection for classification and 
             * epsilon-lexicase selection for regression. 
             * @param pop: CPopulation
             * @param params: parameters.
             *
             * @return selected: vector of indices corresponding to pop that are 
             * selected.
             *
             */            

            unsigned int N = pop.individuals.at(0).error.size(); //< no. of samples
            unsigned int P = pop.individuals.size(); //< no. of individuals
            
            // define epsilon
            ArrayXf epsilon = ArrayXf::Zero(N);
          
            // if output is continuous, use epsilon lexicase            
            if (!params.classification || params.scorer.compare("log")==0 
                    || params.scorer.compare("multi_log")==0)
            {
                // for each sample, calculate epsilon
                for (int i = 0; i<epsilon.size(); ++i)
                {
                    VectorXf case_errors(pop.individuals.size());
                    for (int j = 0; j<pop.individuals.size(); ++j)
                    {
                        case_errors(j) = pop.individuals.at(j).error(i);
                    }
                    epsilon(i) = mad(case_errors);
                }
            }

            // pool of individuals
            vector<size_t> starting_pool;
            /* for (const auto& p : pop.individuals) */
            for (int i = 0; i < pop.individuals.size(); ++i)
            {
            	starting_pool.push_back(i);
            }
            assert(starting_pool.size() == P);     
            
            vector<size_t> selected(P,0); // selected individuals
            #pragma omp parallel for 
            for (unsigned int i = 0; i<P; ++i)  // selection loop
            {
                vector<size_t> cases; // cases (samples)
                if (params.classification && !params.class_weights.empty()) 
                {
                    // for classification problems, weight case selection by class weights
                    vector<size_t> choices(N);
                    std::iota(choices.begin(), choices.end(),0);
                    vector<float> sample_weights = params.sample_weights;
                    for (unsigned i = 0; i<N; ++i)
                    {
                        vector<size_t> choice_idxs(N-i);
                        std::iota(choice_idxs.begin(),choice_idxs.end(),0);
                        size_t idx = r.random_choice(choice_idxs,sample_weights);
                        cases.push_back(choices.at(idx));
                        choices.erase(choices.begin() + idx);
                        sample_weights.erase(sample_weights.begin() + idx);
                    }
                }
                else
                {   // otherwise, choose cases randomly
                    cases.resize(N); 
                    std::iota(cases.begin(),cases.end(),0);
                    r.shuffle(cases.begin(),cases.end());   // shuffle cases
                }
                vector<size_t> pool = starting_pool;    // initial pool   
                vector<size_t> winner;                  // winners
        
                bool pass = true;     // checks pool size and number of cases
                unsigned int h = 0;   // case count

                while(pass){    // main loop

                  winner.resize(0);   // winners                  
                  float minfit = std::numeric_limits<float>::max();   // minimum error on case
                  
                  // get minimum
                  for (size_t j = 0; j<pool.size(); ++j)
                      if (pop.individuals.at(pool[j]).error(cases[h]) < minfit) 
                          minfit = pop.individuals.at(pool[j]).error(cases[h]);
                  
                  // select best
                  for (size_t j = 0; j<pool.size(); ++j)
                      if (pop.individuals.at(pool[j]).error(cases[h]) <= minfit+epsilon[cases[h]])
                        winner.push_back(pool[j]);                 
                 
                  ++h; // next case
                  pass = (winner.size()>1 && h<cases.size()); // only keep going if needed
                  
                  if(winner.size() == 0)
                  {
                  	if(h >= cases.size())
                  		winner.push_back(r.random_choice(pool));
                  	else
                  		pass = true;
                  }
                  else
                  	pool = winner;    // reduce pool to remaining individuals
              
                }       
			
			
                assert(winner.size()>0);
                //if more than one winner, pick randomly
                selected[i] = r.random_choice(winner);   
            }               

            if (selected.size() != pop.individuals.size()){
                std::cout << "selected: " ;
                for (auto s: selected) std::cout << s << " "; std::cout << "\n";
                HANDLE_ERROR_THROW("Lexicase did not select correct number of "+
                        "parents");
            }
            return selected;
        }

        vector<size_t> Lexicase::survive(CPopulation& pop, const CParameters& params)
        {
            /*TODO: Lexicase survival */
        }
        
    }

}
