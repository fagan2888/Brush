/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "population.h"

namespace FT{   


    namespace Pop{
        
        int last; 

        CPopulation::CPopulation(){}
        
        CPopulation::CPopulation(int p)
        {
            individuals.resize(p); 
            for (unsigned i = 0; i < individuals.size(); ++i)
            {
                individuals[i].set_id(i);
                individuals[i].set_parents(vector<int>(1,-1));
           }
        }
        
        CPopulation::~CPopulation(){}
        
        /// update individual vector size 
        void CPopulation::resize(int pop_size, bool resize_locs)
        {	
            individuals.resize(pop_size); 
            if (resize_locs)        // if this is an initial pop size, locs should be resized
            {
                locs.resize(2*pop_size); 
                std::iota(locs.begin(),locs.end(),0);
            }
        }
        
        /// returns CPopulation size
        int CPopulation::size(){ return individuals.size(); }

        const CIndividual CPopulation::operator [](size_t i) const 
        {
            return individuals.at(i);
        }
        
        const CIndividual & CPopulation::operator [](size_t i) 
        {
            return individuals.at(i);
        }


        void CPopulation::init(const CParameters& params, bool random)
        {
            /*!
             *create random programs in the CPopulation, seeded by initial model 
             weights 
             */
            /* individuals[0] = starting_model; */
            /* individuals[0].loc = 0; */
            cout << "individuals.size(): " << individuals.size() << "\n";
            cout << "terminals: ";
            for (int i = 0; i < params.terminals.size(); ++i)
                cout << params.terminals.at(i)->name << ",";
            cout << "\n";

            /* #pragma omp parallel for */
            for (unsigned i = 0; i< individuals.size(); ++i)
            {          
                // pick a dimensionality for this individual
                int dim = r.rnd_int(1,params.max_dim);      
                // pick depth from [params.min_depth, params.max_depth]
                int depth =  r.rnd_int(1, params.max_depth);
                // make a program for each individual
                cout << "make program\n";
                char ot = r.random_choice(params.otypes);
                individuals[i].program.make_program(params.functions, 
                        params.terminals, depth, params.term_weights,
                        dim, ot, params.longitudinalMap, params.ttypes);
                
                /* std::cout << individuals[i].program_str() + " -> "; */
                std::cout << individuals[i].get_eqn() + "\n";
               
            }
        }
       
       void CPopulation::update(vector<size_t> survivors)
       {

           /*!
            * cull CPopulation down to survivor indices.
            */
           vector<size_t> pop_idx(individuals.size());
           std::iota(pop_idx.begin(),pop_idx.end(),0);
           std::reverse(pop_idx.begin(),pop_idx.end());
           for (const auto& i : pop_idx)
           {
               if (!in(survivors,i))
                   individuals.erase(individuals.begin()+i);                         
           }
       }

       void CPopulation::add(CIndividual& ind)
       {
           /*!
            * adds ind to individuals, giving it an open location and bookeeping.
            */

           individuals.push_back(ind);
       }

       string CPopulation::print_eqns(bool just_offspring, string sep)
       {
           string output = "";
           int start = 0;
           
           if (just_offspring)
               start = individuals.size()/2;

           for (unsigned int i=start; i< individuals.size(); ++i)
               output += individuals[i].get_eqn() + sep;
           
           return output;
       }

        vector<size_t> CPopulation::sorted_front(unsigned rank=1)
        {
            /* Returns individuals on the Pareto front, sorted by increasign complexity. */
            vector<size_t> pf;
            for (unsigned int i =0; i<individuals.size(); ++i)
            {
                if (individuals[i].rank == rank)
                    pf.push_back(i);
            }
            std::sort(pf.begin(),pf.end(),SortComplexity(*this)); 
            auto it = std::unique(pf.begin(),pf.end(),SameFitComplexity(*this));
            pf.resize(std::distance(pf.begin(),it));
            return pf;
        }
        
        vector<CIndividual>& CPopulation::get_individuals()
        {
        	return individuals;
        }
        
    }
    
}
