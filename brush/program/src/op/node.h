/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef CNode_H
#define CNode_H

#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include "../../../init.h"
#include "../../../data/src/state.h"
#include "../../../data/src/data.h"
#include "../../../util/src/rnd.h"
#include "../../../util/src/error.h"
using std::vector;
using std::string;
using std::map;
using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::ArrayXf;
typedef Eigen::Array<bool,Eigen::Dynamic,1> ArrayXb;
#define MAX_FLT std::numeric_limits<float>::max()
#define MIN_FLT std::numeric_limits<float>::lowest()

#define MAX_INT std::numeric_limits<int>::max()
#define MIN_INT std::numeric_limits<int>::lowest()

#ifdef USE_CUDA
    #include "../cuda-op/kernels.h"
#endif


namespace FT{

    using namespace Util;
    using namespace Dat;

    namespace Pop{
        /**
         * @namespace FT::Pop::Op
         * @brief namespace representing various operations on population individuals used in Feat
         */
        namespace Op{
            //////////////////////////////////////////////////////////////////////////////// Declarations
             /*!
             * @class CNode
             * @brief Represents CNodes in a program.
             */
            class CNode
            {       
                public:
                    string name;              				///< CNode type
                    char otype;             				///< output type
                    std::map<char, unsigned int> arity;		///< floating arity of the operator 
                    int complexity;         ///< complexity of CNode
                    int visits = 0;
                    
                    CNode();

                    virtual ~CNode(){}
                   
                    /// Evaluates the CNode and updates the state states. 
                    virtual void evaluate(const CData& data, State& state) = 0; 

                    /// evaluates the CNode symbolically
                    virtual void eval_eqn(State& state) = 0;

                    // total arity
                    unsigned int total_arity();

                    /// limits CNode output to be between MIN_FLT and MAX_FLT
                    ArrayXf limited(ArrayXf x);

                    /// evaluates complexity of this CNode in the context of its child CNodes.
                    void eval_complexity(map<char, vector<unsigned int>>& cstate);
                    
                    /// evaluates complexity of this CNode in the context of its child CNodes.
                    void eval_complexity_db(map<char, vector<string>>& cstate);

                    /// check of CNode type
                    virtual bool isNodeDx() {return false;};
                    virtual bool isNodeTrain() {return false;};

                    /// makes a unique copy of this CNode
                    std::unique_ptr<CNode> clone() const;
                    
                    /// makes a randomized unique copy ofCNode
                    std::unique_ptr<CNode> rnd_clone() const;
                
                protected:
                    virtual CNode* clone_impl() const = 0;
                    virtual CNode* rnd_clone_impl() const = 0;
            };
            
        }
    }
}
#endif
