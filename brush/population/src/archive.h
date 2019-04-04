/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef ARCHIVE_H
#define ARCHIVE_H

//#include "node.h" // including node.h since definition of node is in the header
#include "individual.h"
#include "../../selection/src/nsga2.h"
using std::vector;
using std::string;
using Eigen::Map;

namespace FT{

    using namespace Sel;
    ////////////////////////////////////////////////////////////////////////////////// Declarations
    /*!
     * @class Archive 
     * @brief Defines a Pareto archive of programs.
     */
     
    namespace Pop{
    
        struct Archive  
        {
            vector<CIndividual> archive;         ///< individual programs in the archive

            NSGA2 selector;                     ///< nsga2 selection operator used for getting the front

            Archive();

            /// Sort population in increasing complexity.
            static bool sortComplexity(const CIndividual& lhs, const CIndividual& rhs);

            static bool sameFitComplexity(const CIndividual& lhs, const CIndividual& rhs);

            void init(Population& pop);

            void update(const Population& pop, const CParameters& params);
           
        };
    }
}
#endif
