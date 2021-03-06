/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#ifndef LEXICASE_H
#define LEXICASE_H

#include "selection_operator.h"


namespace FT{

    namespace Sel{
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        /*!
         * @class Lexicase
         * @brief Lexicase selection operator.
         */
        struct Lexicase : SelectionOperator
        {
            Lexicase(bool surv);
            
            ~Lexicase();

            /// function returns a set of selected indices from pop. 
            vector<size_t> select(CPopulation& pop, const CParameters& params); 
            
            /// lexicase survival
            vector<size_t> survive(CPopulation& pop, const CParameters& params); 

        };
    }

}

#endif
