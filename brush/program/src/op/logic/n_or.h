/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef NODE_OR
#define NODE_OR

#include "../node.h"

namespace FT{


    namespace Pop{
        namespace Op{
        	class NodeOr : public CNode
            {
            	public:
            	
            		NodeOr();
            		
                    /// Evaluates the node and updates the state states. 
                    void evaluate(const CData& data, State& state);
                    
                    /// Evaluates the node symbolically
                    void eval_eqn(State& state);
                    
                protected:
                    NodeOr* clone_impl() const override;

                    NodeOr* rnd_clone_impl() const override;
            };
        }
    }    
}	

#endif
