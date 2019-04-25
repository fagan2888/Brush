/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef VARIATION_H
#define VARIATION_H

#include <iostream>
using namespace std;

#include "../../program/src/nodevector.h"
#include "../../population/src/population.h"
#include "../../params/src/params.h"

namespace FT{

    /**
     * @namespace FT::Vary
     * @brief namespace containing various variation methods for cross and mutation in Feat
     */
    namespace Vary{

        //struct CIndividual;  // forward declarations
        //struct CParameters;
        //struct CPopulation;
        //Rnd r;
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        /*!
         * @class CVariation
         */ 
        class CVariation 
        {
            /*!
             * methods for crossover and mutation of programs.
             */

            public:
            
            	CVariation();
            
                /// constructor
                CVariation(float cr);
                           
                /// update cross rate
                void set_cross_rate(float cr);
                
                /// return current cross rate
                float get_cross_rate();
                
                 /// destructor
                ~CVariation();

                /// method to handle variation of CPopulation
                void vary(CPopulation& pop, const vector<size_t>& parents, const CParameters& params,
                          const CData& d);
                
            private:
            
                /// crossover
                bool cross(CIndividual& mom, CIndividual& dad, CIndividual& child,
                           const CParameters& params, const CData& d);
                
                /// mutation
                bool mutate(CIndividual& mom, CIndividual& child, const CParameters& params);
                void point_mutate(CIndividual& child, const CParameters& params);
                void insert_mutate(CIndividual& child, const CParameters& params);
                void delete_mutate(CIndividual& child, const CParameters& params);
     
                /// splice two programs together
                void splice_programs(CNodeVector& vnew, 
                                     const CNodeVector& v1, size_t i1, size_t j1, 
                                     const CNodeVector& v2, size_t i2, size_t j2);
                /// debugging printout of crossover operation.
                void print_cross(CIndividual&,size_t,size_t,CIndividual&, size_t, size_t, CIndividual&,
                                 bool after=true);       
                
                float cross_rate;     ///< fraction of crossover in total variation
        };

        std::unique_ptr<CNode> random_node(const CNodeVector & v);
    }
}
#endif
