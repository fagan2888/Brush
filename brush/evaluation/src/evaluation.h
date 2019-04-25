/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef EVALUATION_H
#define EVALUATION_H
// internal includes
#include "metrics.h"
//#include "../opt/auto_backprop.h"
//#include "../opt/hillclimb.h"
//using namespace shogun;
#include "../../population/src/individual.h"
using Eigen::Map;

// code to evaluate GP programs.
namespace FT{

    /**
     * @namespace FT::Eval
     * @brief namespace containing various CEvaluation classes used in Feat
     */
    namespace Eval{
    
        ////////////////////////////////////////////////////////////////////////////////// Declarations
        /*!
         * @class CEvaluation
         * @brief evaluation mixin class for Feat
         */
        typedef float (*funcPointer)(const VectorXf&, const VectorXf&, VectorXf&,
                                      const vector<float>&);
        
        class CEvaluation 
        {
            public:
            
                float (* score)(const VectorXf&, const VectorXf&, VectorXf&, 
                                 const vector<float>&);    // pointer to scoring function
                                 
                std::map<string, funcPointer> score_hash;

                CEvaluation(string scorer);

                ~CEvaluation();
                    
                void set_score(string scorer);

                /// fitness of population.
                void fitness(vector<CIndividual>& individuals,
                             const CData& d, 
                             const CParameters& params, 
                             bool offspring = false,
                             bool validation = false);
              
                /// assign fitness to an individual.  
                void assign_fit(CIndividual& ind, const VectorXf& yhat, 
                                const VectorXf& y, const CParameters& params,bool val=false);       
        };
    }
}
#endif
