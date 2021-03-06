/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef NODE_CONSTANT
#define NODE_CONSTANT

#include "../node.h"

namespace FT{

    namespace Pop{
        namespace Op{
	        class NodeConstant : public CNode
            {
            	public:
            		
            		float d_value;           ///< value, for k and x types
            		bool b_value;
            		
            		NodeConstant();
            		
                    /// declares a boolean constant
            		NodeConstant(bool& v);

                    /// declares a float constant
            		NodeConstant(const float& v);
            		
                    /// Evaluates the node and updates the state states. 
                    void evaluate(const CData& data, State& state);

                    /// Evaluates the node symbolically
                    void eval_eqn(State& state);

                    // Make the derivative 1
            		
                protected:
                        NodeConstant* clone_impl() const override;
              
                        NodeConstant* rnd_clone_impl() const override;
            };
        }
    }
}	

#endif
