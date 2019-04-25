/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef CNodeVector_H
#define CNodeVector_H
#include <memory>
#include "op/node.h"
#include "op/n_Dx.h"

#include "error.h"
namespace FT{
    
    namespace Pop{
    
        using namespace Op;
        ////////////////////////////////////////////////////////////////////////////////// Declarations

        /*!
         * @class CNodeVector
         * @brief an extension of a vector of unique pointers to nodes 
         */
        class CNodeVector : public std::vector<std::unique_ptr<CNode>>
        {
            public :
            
            CNodeVector();
            
            ~CNodeVector();
            
            CNodeVector(const CNodeVector& other);
            
            CNodeVector(CNodeVector && other);
            /* { */
            /*     std::cout<<"in CNodeVector(CNodeVector&& other)\n"; */
            /*     for (const auto& p : other) */
            /*         this->push_back(p->clone()); */
            /* } */
            
            CNodeVector& operator=(CNodeVector const& other);
            
            CNodeVector& operator=(CNodeVector && other);
            
            /// returns vector of raw pointers to nodes in [start,end], or all if both are zero
            vector<CNode*> get_data(int start=0,int end=0);

            /// returns indices of root nodes 
            vector<size_t> roots() const;

            size_t subtree(size_t i, char otype='0') const;
            
            void set_weights(vector<vector<float>>& weights);
            
            vector<vector<float>> get_weights();
            
            bool is_valid_program(unsigned num_features, 
                                  vector<string> longitudinalMap);
       
            void make_tree(const CNodeVector& functions, 
                           const CNodeVector& terminals, int max_d,  
                           const vector<float>& term_weights, char otype, const vector<char>& term_types);

            void make_program(const CNodeVector& functions, 
                              const CNodeVector& terminals, int max_d, 
                              const vector<float>& term_weights, int dim, char otype, 
                              vector<string> longitudinalMap, const vector<char>& term_types);
            
        }; //CNodeVector
        
    }
} // FT
#endif
