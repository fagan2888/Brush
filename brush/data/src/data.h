/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#ifndef DATA_H
#define DATA_H

#include <string>
#include <Eigen/Dense>
#include <vector>
#include <map>

using std::vector;
using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::ArrayXf;
using Eigen::VectorXi;
using Eigen::Dynamic;
using Eigen::Map;
typedef Eigen::Array<bool,Eigen::Dynamic,1> ArrayXb;
using namespace std;
// internal includes
//#include "params.h"
#include "../../util/src/utils.h"
//#include "node/node.h"
//external includes

namespace FT
{
    /**
    * @namespace FT::Dat
    * @brief namespace containing CData structures used in Feat
    */
    namespace Dat{
        /*!
         * @class CData
         * @brief data holding X, y, and Z data
         */
        class CData
        {
            //CData(MatrixXf& X, VectorXf& y, std::map<string, 
            //std::pair<vector<ArrayXf>, vector<ArrayXf>>>& Z): X(X), y(y), Z(Z){}
            public:
     
                MatrixXf X;
                VectorXf y;
                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z;
                bool classification;
                bool validation; 
                
                CData();

                CData(MatrixXf& X, VectorXf& y, 
                        std::map<string, std::pair<vector<ArrayXf>, 
                        vector<ArrayXf>>>& Z, bool c = false);
                        
                CData(float * X, int rowsX, int colsX,
                	  float * Y, int lenY,
                	  bool c=false);
                        
                CData(float * X, int rowsX, int colsX,
                	  float * Y, int lenY,
                	  string s, int * idx, int idx_size,
                	  bool c = false);
                 
                MatrixXf& get_X() { return this->X; }
                VectorXf& get_y() { return this->y; }
                void set_X(const Map<MatrixXf> & mat);
                void set_y(const Map<VectorXf> & ynew);

                void set_validation(bool v=true);
                
                /// select random subset of data for training weights.
                void get_batch(CData &db, int batch_size) const;
                
                std::map<string, std::pair<vector<ArrayXf>, 
                    vector<ArrayXf>>> get_Z(string s, int * idx, int idx_size);
        };
        
        /* !
         * @class CCVData
         * @brief Holds training and validation splits of data, with pointers to each.
         * */
        class CCVData
        {
            private:
                bool oCreated;
                bool tCreated;
                bool vCreated;
                // training and validation data
                MatrixXf X_t;
                MatrixXf X_v;
                VectorXf y_t;
                VectorXf y_v;
                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > Z_t;
                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > Z_v;
                
                bool classification;
                CData dbatch;        //< permanent class used to handle batches

            public:
                CData *o = NULL;     //< pointer to original data
                CData *v = NULL;     //< pointer to validation data
                CData *t = NULL;     //< pointer to training data
                
                CCVData();
                
                ~CCVData();
                
        
                CCVData(MatrixXf X, VectorXf y, 
                   std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > Z, 
                   bool c=false);
                                 
				CCVData(float * X, int rowsX, int colsX,
                	  	float * Y, int lenY,
                	  	bool c=false);
                	  	                                 
				CCVData(float * X, int rowsX, int colsX,
                	  	float * Y, int lenY,
                	  	string s, int * idx, int idx_size,
                	  	bool c = false);
                        
                void setOriginalData(MatrixXf X, VectorXf y, 
                   std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z, 
                   bool c=false);
                
                void setOriginalData(CData *d);
                
                void setTrainingData(MatrixXf X_t, VectorXf y_t, 
                  std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z_t,
                                   bool c = false);
                
                void setTrainingData(CData *d, bool toDelete = false);
                
                void setValidationData(MatrixXf X_v, VectorXf y_v, 
                  std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z_v,
                                   bool c = false);
                
                void setValidationData(CData *d);
                
                /// shuffles original data
                void shuffle_data();
                
                /// split classification data as stratas
                void split_stratified(float split);
                
                /// splits data into training and validation folds.
                void train_test_split(bool shuffle, float split);

                void split_longitudinal(
                 std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z,
                 std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z_t,
                 std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z_v,
                            float split);
                            
                /// reordering utility for shuffling longitudinal data.
                void reorder_longitudinal(vector<ArrayXf> &vec1,
                                          vector<ArrayXf> &vec2,
                                          vector<int> order);
                                          
                /// gets a batch 
                void get_batch(CCVData* cvbatch, int batch_size);

        };
    }
}

#endif
