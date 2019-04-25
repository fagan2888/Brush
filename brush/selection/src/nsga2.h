/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef PARETO_H
#define PARETO_H

#include "selection_operator.h"

namespace FT{

    namespace Sel{
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        /*!
         * @class NSGA2
         */
        struct NSGA2 : SelectionOperator
        {
            /** NSGA-II based selection and survival methods. */

            NSGA2(bool surv);
            
            ~NSGA2();

            /// selection according to the survival scheme of NSGA-II
            vector<size_t> select(CPopulation& pop, const CParameters& p);
            
            /// survival according to the survival scheme of NSGA-II
            vector<size_t> survive(CPopulation& pop, const CParameters& p);

            vector<vector<int>> front;                //< the Pareto fronts
            
            void fast_nds(vector<CIndividual>&);               //< Fast non-dominated sorting 
            
            void crowding_distance(CPopulation&, int); //< crowding distance of a front i
                
            private:    

                /// sort based on rank, breaking ties with crowding distance
                struct sort_n 
                {
                    const CPopulation& pop;          ///< CPopulation address
                    sort_n(const CPopulation& CPopulation) : pop(CPopulation) {};
                    bool operator() (int i, int j) {
                        const CIndividual& ind1 = pop.individuals[i];
                        const CIndividual& ind2 = pop.individuals[j];
                        if (ind1.rank < ind2.rank)
                            return true;
                        else if (ind1.rank == ind2.rank &&
                                 ind1.crowd_dist > ind2.crowd_dist)
                            return true;
                        return false;
                    };
                };

                /// sort based on objective m
                struct comparator_obj 
                {
                    const CPopulation& pop;      ///< CPopulation address
                    int m;                      ///< objective index 
                    comparator_obj(const CPopulation& CPopulation, int index) 
                        : pop(CPopulation), m(index) {};
                    bool operator() (int i, int j) { return pop[i].obj[m] < pop[j].obj[m]; };
                };
            
                size_t tournament(vector<CIndividual>& pop, size_t i, size_t j) const;
        };
        
    }
    
}
#endif
