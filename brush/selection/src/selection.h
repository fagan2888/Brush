/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#ifndef SELECTION_H
#define SELECTION_H

#include "selection_operator.h"
#include "lexicase.h"
#include "nsga2.h"
#include "offspring.h"
#include "random.h"
#include "simulated_annealing.h"

namespace FT{

    
    struct CParameters; // forward declaration of CParameters      
    
    /**
     * @namespace FT::Sel
     * @brief namespace containing Selection methods for best individuals used in Feat
     */
    namespace Sel{ 
        ////////////////////////////////////////////////////////////////////////////////// Declarations
	
        
        /*!
         * @class Selection
         * @brief interfaces with selection operators. 
         */
        struct Selection
        {
            shared_ptr<SelectionOperator> pselector; 
            
            Selection(string type="lexicase", bool survival=false);
            
            ~Selection();
            
            /// return type of selectionoperator
            string get_type();
            
            /// perform selection 
            vector<size_t> select(CPopulation& pop, const MatrixXf& F, const CParameters& params);
            
            /// perform survival
            vector<size_t> survive(CPopulation& pop, const MatrixXf& F,  const CParameters& params);
        };
        
    }
    
}
#endif
