/* FEWTWO
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "selection_operator.h"

namespace FT{

    namespace Sel{

        SelectionOperator::~SelectionOperator(){}
        
        vector<size_t> SelectionOperator::select(CPopulation& pop, const CParameters& p) 
        {   
            HANDLE_ERROR_THROW("Undefined select() operation");
        }
        
        vector<size_t> SelectionOperator::survive(CPopulation& pop, const CParameters& p)
        {
            HANDLE_ERROR_THROW("Undefined select() operation");
        }
        
    }

}

