/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef NODE_MIN
#define NODE_MIN

#include "../node.h"

namespace FT{

    namespace Pop{
        namespace Op{
        	class NodeMin : public CNode
            {
            	public:
            	
            		NodeMin();
            		    		
                    /// Evaluates the node and updates the state states. 
                    void evaluate(const CData& data, State& state);

                    /// Evaluates the node symbolically
                    void eval_eqn(State& state);
                    
                protected:
                    NodeMin* clone_impl() const override;

                    NodeMin* rnd_clone_impl() const override;
            };
        }
    }
}	

#endif
