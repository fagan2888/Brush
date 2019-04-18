/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef CPopulation_H
#define CPopulation_H

//#include "node.h" // including node.h since definition of node is in the header
#include "individual.h"
using std::vector;
using std::string;
using Eigen::Map;

namespace FT{ 

    /**
     * @namespace FT::Pop
     * @brief namespace containing CPopulation representations used in Feat
     */
    namespace Pop{   
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        extern int last;
        /*!
         * @class CPopulation
         * @brief Defines a CPopulation of programs and functions for constructing them. 
         */
        class CPopulation
        {
            public :
            
            vector<CIndividual> individuals;     ///< individual programs
            vector<size_t> open_loc;            ///< unfilled matrix positions
            vector<size_t> locs;
            

            CPopulation();
            
            CPopulation(int p);
            
            ~CPopulation();
            
            /// initialize CPopulation of programs. 
            void init(const CIndividual& starting_model, const CParameters& params, bool random = false);
            
            /// update individual vector size 
            void resize(int pop_size, bool resize_locs=false);
            
            /// reduce programs to the indices in survivors. 
            void update(vector<size_t> survivors);
            
            /// returns CPopulation size
            int size();

            /// returns an open location 
            size_t get_open_loc(); 
            
            /// updates open locations to reflect CPopulation.
            void update_open_loc();

            /// adds a program to the CPopulation. 
            void add(CIndividual&);
            
            /// setting and getting from individuals vector
            const CIndividual operator [](size_t i) const;
            
            const CIndividual & operator [](size_t i);

            /// return CPopulation equations. 
            string print_eqns(bool just_offspring=false, string sep="\n");

            /// return complexity-sorted Pareto front indices. 
            vector<size_t> sorted_front(unsigned);
            
            vector<CIndividual>& get_individuals();
            
            /// Sort CPopulation in increasing complexity.
            struct SortComplexity
            {
                CPopulation& pop;
                SortComplexity(CPopulation& p): pop(p){}
                bool operator()(size_t i, size_t j)
                { 
                    return pop.individuals[i].complexity() < pop.individuals[j].complexity();
                }
            };
            
            /// check for same fitness and complexity to filter uniqueness. 
            struct SameFitComplexity
            {
                CPopulation & pop;
                SameFitComplexity(CPopulation& p): pop(p){}
                bool operator()(size_t i, size_t j)
                {
                    return (pop.individuals[i].fitness == pop.individuals[j].fitness &&
                           pop.individuals[i].complexity() == pop.individuals[j].complexity());
                }
            };

        };        
    }
}//FT    
#endif
