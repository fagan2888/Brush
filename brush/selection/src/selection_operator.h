/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#ifndef SELECTION_OPERATOR_H
#define SELECTION_OPERATOR_H

#include "../../population/src/population.h"

namespace FT{

    namespace Sel{

        /*!
         * @class SelectionOperator
         * @brief base class for selection operators.
         */ 
        struct SelectionOperator 
        {
            bool survival; 
            string name;

            //SelectionOperator(){}

            virtual ~SelectionOperator();
            
            virtual vector<size_t> select(CPopulation& pop, const CParameters& p);
            
            virtual vector<size_t> survive(CPopulation& pop, const CParameters& p);
        };
    }
}

#endif
