/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef RANDOM_H
#define RANDOM_H

#include "selection_operator.h"

namespace FT{

    namespace Sel{
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        /*!
         * @class Random
         */
        struct Random : SelectionOperator
        {
            /** Random based selection and survival methods. */

            Random(bool surv);
            
            ~Random();
           
            vector<size_t> select(CPopulation& pop, const MatrixXf& F, const CParameters& params);
            vector<size_t> survive(CPopulation& pop, const MatrixXf& F, const CParameters& params);
            /// replaces worst individual in selected with best individual in Pop.
            void enforce_elite(CPopulation& pop, vector<size_t>& selected);
            bool elitism;       //< whether or not to keep the best individual.

        };
        
    }
    
}
#endif
