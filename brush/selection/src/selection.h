/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#ifndef CSelection_H
#define CSelection_H

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
     * @brief namespace containing CSelection methods for best individuals used in Feat
     */
    namespace Sel{ 
        ////////////////////////////////////////////////////////////////////////////////// Declarations
	
        
        /*!
         * @class CSelection
         * @brief interfaces with CSelection operators. 
         */
        class CSelection
        {
        	public:
        	
            shared_ptr<SelectionOperator> pselector; 
            
            CSelection(string type="lexicase", bool survival=false);
            
            ~CSelection();
            
            /// return type of CSelectionoperator
            string get_type();
            
            /// perform CSelection 
            vector<size_t> select(CPopulation& pop, const CParameters& params);
            
            /// perform survival
            vector<size_t> survive(CPopulation& pop,  const CParameters& params);
        };
        
    }
    
}
#endif
