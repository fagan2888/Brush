/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../../data/src/state.h"
#ifdef USE_CUDA
    #include "cuda-op/cuda_utils.h"
#endif
#include "../../data//src/data.h"
#include "../../params/src/params.h"
#include "../../util/src/utils.h"

namespace FT{
    
    namespace Pop{
    
    	class CPopulation;
        
        /////////////////////////////////////////////////////////////// Declarations

        /*!
         * @class CIndividual
         * @brief individual programs in the population
         */
        class CIndividual{
        public:        
            CNodeVector program;      ///< executable data structure
            MatrixXf Phi;             ///< transformation output of program 
            VectorXf yhat;            ///< current output
            VectorXf error;           ///< current error
            float fitness;            ///< aggregate fitness score
            float fitness_v;          ///< aggregate validation fitness score
            float CN;
            size_t loc;               ///< index of individual in semantic matrix F
            string eqn;               ///< symbolic representation of program
            vector<float> w;          ///< weights from ML training on program output
            vector<float> p;          ///< probability of variation of subprograms
            unsigned int dim;         ///< dimensionality of individual
            vector<float> obj;        ///< objectives for use with Pareto selection
            unsigned int dcounter;    ///< number of individuals this dominates
            vector<unsigned int> dominated; ///< individual indices this dominates
            unsigned int rank;        ///< pareto front rank
            float crowd_dist;         ///< crowding distance on the Pareto front
            unsigned int c;           ///< the complexity of the program.    
            vector<char> dtypes;      ///< the data types of each column of the 
                                            // program output
            unsigned id;              ///< tracking id
            vector<int> parent_id;    ///< ids of parents
           
            CIndividual();

            /// calculate program output matrix Phi
            MatrixXf out(const CData& d, const CParameters& params, 
                    bool predict=false);

            /// calculate program output while maintaining stack trace
            MatrixXf out_trace(const CData& d,
                         const CParameters& params, vector<Trace>& stack_trace);
                         
            /// fits an ML model to the data after transformation
            VectorXf fit(const CData& d, const CParameters& params, bool& pass);
            
            /*! generates prediction on data using transformation and ML predict. 
             *  @param drop_idx if specified, the phi output at drop_idx is set to 
             *  zero, effectively removing its output from the transformation. 
             *  used in semantic crossover.
             */
            VectorXf predict(const CData& d, const CParameters& params);
            
            //TODO discuss implementation and uncomment later
//            VectorXf predict_vector(const CData& d, const CParameters& params);
//            VectorXf predict_drop(const CData& d, const CParameters& params, int drop_idx);
            /// return symbolic representation of program
            string get_eqn();

            /// return vectorized representation of program
            vector<string> get_features();

            /// setting and getting from individuals vector
            /* const std::unique_ptr<Node> operator [](int i) const {return program.at(i);} */ 
            /* const std::unique_ptr<Node> & operator [](int i) {return program.at(i);} */

            /// set rank
            void set_rank(unsigned r);
            
            /// return size of program
            int size() const;
            
            /// get number of params in program
            int get_n_params();
            
            /// grab sub-tree locations given starting point.
            /* size_t subtree(size_t i, char otype) const; */

            // // get program depth.
            // unsigned int depth();

            /// get program dimensionality
            unsigned int get_dim();
            
            /// check whether this dominates b. 
            int check_dominance(const CIndividual& b) const;
            
            /// set obj vector given a string of objective names
            void set_obj(const vector<string>&); 
            
            /// calculate program complexity. 
            unsigned int complexity();
            
            unsigned int get_complexity() const;
          
            /// clone this individual 
            void clone(CIndividual& cpy, bool sameid=true);
            
            void set_id(unsigned i);

            /// set parent ids using parents  
            void set_parents(const vector<CIndividual>& parent);
            
            /// set parent ids using id values 
            void set_parents(const vector<int>& parents){ parent_id = parents; }

            /// set yhat
            void set_yhat(const Map<VectorXf>& arr){this->yhat = arr;}

            /// set error
            void set_error(const Map<VectorXf>& arr){this->error = arr;}

            /// get probabilities of variation
            vector<float> get_p() const;
            
            /// get inverted weight probability for pogram location i
            float get_p(const size_t i, bool normalize=true) const;
            
            /// get probability of variation for program locations locs
            vector<float> get_p(const vector<size_t>& locs, bool normalize=false) const; 

            /// set probabilities
            void set_p(const vector<float>& weights, const float& fb, 
                       const bool softmax_norm=false);

            /// placeholder probability setter
            void set_p();
            /// get maximum stack size needed for evaluation.
            std::map<char,size_t> get_max_state_size();
            
            typedef Array<bool, Dynamic, Dynamic, RowMajor> ArrayXXb;
            typedef Array<float, Dynamic, Dynamic, RowMajor> ArrayXXf;
        };
    }
}

#endif
