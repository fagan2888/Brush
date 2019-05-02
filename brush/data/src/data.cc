/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "data.h"
#include "../../util/src/rnd.h"
#include "../../util/src/logger.h"
#include "../../util/src/io.h"

//#include "node/node.h"
//external includes

namespace FT{

    using namespace Util;
    
    namespace Dat{
    
    	CData::CData(){};

        CData::CData(MatrixXf X, VectorXf y, std::map<string, std::pair<vector<ArrayXf>, 
                        vector<ArrayXf>>> Z, bool c): X(X), y(y), Z(Z), classification(c) 
        {
        	this->X = Map<MatrixXf>(X.data(), X.rows(), X.cols());
        	this->y = Map<VectorXf>(y.data(), y.size());
        	this->Z = std::move(Z);
            validation=false;
            classification = c;
        }
        
        CData::CData(float * X, int rowsX, int colsX,
              		 float * Y, int lenY,
              		 bool c)
        {
        	this->X = Map<MatrixXf>(X,rowsX,colsX);
    		this->y = Map<VectorXf>(Y,lenY);
    		validation = false;
    		classification = c;
        }
        
        CData::CData(float * X, int rowsX, int colsX,
              		 float * Y, int lenY,
              		 string s, int * idx, int idx_size, bool c)
        {
        	this->X = Map<MatrixXf>(X,rowsX,colsX);
    		this->y = Map<VectorXf>(Y,lenY);
    		this->Z = get_Z(s, idx, idx_size);
    		validation = false;
    		classification = c;
        }
        
        void CData::set_validation(bool v){validation=v;}
        
        void CData::get_batch(CData &db, int batch_size) const
        {

            batch_size =  std::min(batch_size,int(y.size()));
            vector<size_t> idx(y.size());
            std::iota(idx.begin(), idx.end(), 0);
    //        r.shuffle(idx.begin(), idx.end());
            db.X.resize(X.rows(),batch_size);
            db.y.resize(batch_size);
            for (const auto& val: Z )
            {
                db.Z[val.first].first.resize(batch_size);
                db.Z[val.first].second.resize(batch_size);
            }
            for (unsigned i = 0; i<batch_size; ++i)
            {
               
               db.X.col(i) = X.col(idx.at(i)); 
               db.y(i) = y(idx.at(i)); 

               for (const auto& val: Z )
               {
                    db.Z[val.first].first.at(i) = Z.at(val.first).first.at(idx.at(i));
                    db.Z[val.first].second.at(i) = Z.at(val.first).second.at(idx.at(i));
               }
            }
        }
        
        /// get longitudinal data from file s
		std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> CData::get_Z(string s, 
				int * idx, int idx_size)
		{
			std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > Z;
			vector<int> ids(idx,idx+idx_size);
			load_partial_longitudinal(s,Z,',',ids);
			/* for (auto& z : Z){ */
			/*     reorder_longitudinal(z.second.first, ids); */
			/*     reorder_longitudinal(z.second.second, ids); */
			/* } */
			/* cout << "Z:\n"; */
			/* for (auto& z : Z) */
			/* { */
			/*     cout << z.first << "\n"; */
			/*     for (unsigned i = 0; i < z.second.first.size(); ++i) */
			/*     { */
			/*         cout << "value: " << z.second.first.at(i).matrix().transpose() << "\n"; */
			/*         cout << "time: " << z.second.second.at(i).matrix().transpose() << "\n"; */
			/*     } */
			/* } */
				
			return Z;
		}
        
        CCVData::CCVData()
        {
            oCreated = false;
            tCreated = false;
            vCreated = false;
        }
     
        CCVData::CCVData(MatrixXf X, VectorXf y, 
                         std::map<string,std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z, bool c)
        {
            o = new CData(X, y, Z, c);
            oCreated = true;
            
            t = new CData(X_t, y_t, Z_t, c);
            tCreated = true;
            
            v = new CData(X_v, y_v, Z_v, c);
            vCreated = true;
            
            classification = c;
          
            // split data into training and test sets
            //train_test_split(params.shuffle, params.split);
        }
        
        CCVData::CCVData(float * X, int rowsX, int colsX,
                	  	float * Y, int lenY,
                	  	bool c)
        {
        	o = new CData(X, rowsX, colsX,
        				  Y, lenY,
        				  c);
        				  
            oCreated = true;
            
            t = new CData(X_t, y_t, Z_t, c);
            tCreated = true;
            
            v = new CData(X_v, y_v, Z_v, c);
            vCreated = true;
            
            classification = c;
        }
        
        CCVData::CCVData(float * X, int rowsX, int colsX,
                	  	float * Y, int lenY,
                	  	string s, int * idx, int idx_size,
                	  	bool c)
        {
        	o = new CData(X, rowsX, colsX,
        				  Y, lenY,
        				  s, idx, idx_size,
        				  c);
        				  
            oCreated = true;
            
            t = new CData(X_t, y_t, Z_t, c);
            tCreated = true;
            
            v = new CData(X_v, y_v, Z_v, c);
            vCreated = true;
            
            classification = c;
        }
       
        CCVData::~CCVData()
        {
            if(o != NULL && oCreated)
            {
                delete(o);
                o = NULL;
            }
            
            if(t != NULL && tCreated)
            {
                delete(t);
                t = NULL;
            }
            
            if(v != NULL && vCreated)
            {
                delete(v);
                v = NULL;
            }
        }
        
        void CCVData::setOriginalData(MatrixXf X, VectorXf y, 
                                      std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z,
                                      bool c)
        {
            o = new CData(X, y, Z, c);
            oCreated = true;
            
            t = new CData(X_t, y_t, Z_t, c);
            tCreated = true;
            
            v = new CData(X_v, y_v, Z_v, c);
            vCreated = true;
            
            classification = c;
        }
        
        void CCVData::setOriginalData(CData *d)
        {
            o = d;
            oCreated = false;
            
            t = new CData(X_t, y_t, Z_t, d->classification);
            tCreated = true;
            
            v = new CData(X_v, y_v, Z_v, d->classification);
            vCreated = true;
            
            classification = d->classification;
        }
        
        void CCVData::setTrainingData(MatrixXf X_t, VectorXf y_t, 
                                    std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z_t,
                                    bool c)
        {
            t = new CData(X_t, y_t, Z_t, c);
            tCreated = true;
            
            classification = c;
        }
        
        void CCVData::setTrainingData(CData *d, bool toDelete)
        {
            t = d;
            if(!toDelete)
                tCreated = false;
            else
                tCreated = true;
        }
        
        void CCVData::setValidationData(MatrixXf X_v, VectorXf y_v, 
                                    std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> Z_v,
                                    bool c)
        {
            v = new CData(X_v, y_v, Z_v, c);
            vCreated = true;
        }
        
        void CCVData::setValidationData(CData *d)
        {
            v = d;
            vCreated = false;
        }
        
        void CCVData::shuffle_data()
        {
            Eigen::PermutationMatrix<Dynamic,Dynamic> perm(o->X.cols());
            perm.setIdentity();
            r.shuffle(perm.indices().data(), perm.indices().data()+perm.indices().size());
            o->X = o->X * perm;       // shuffles columns of X
            o->y = (o->y.transpose() * perm).transpose() ;       // shuffle y too
            
            if(o->Z.size() > 0)
            {
                std::vector<int> zidx(o->y.size());
                std::iota(zidx.begin(), zidx.end(), 0);
                VectorXi zw = Map<VectorXi>(zidx.data(), zidx.size());
                // shuffle z indices 
                zw = (zw.transpose()*perm).transpose();       
                // assign shuffled zw to zidx
                zidx.assign(zw.data(), zw.data() + zw.size());
                
                cout << "Permutation vector is " << zw << "\n";
                
                cout << "Zidx is \t";
                for(int x = 0 ; x < o->y.size(); x++)
                    cout << zidx[x] << "\t";
                    
                cout << "\n";
                    
                for(auto &val : o->Z)
				{
				    cout << val.first << "\n";
				    reorder_longitudinal(val.second.first, val.second.second, zidx);
                    //reorder_longitudinal(val.second.first, zidx);
                    //reorder_longitudinal(val.second.second, zidx);
				}
            }
        }
        
        void CCVData::split_stratified(float split)
        {
            logger.log("Stratify split called with initial data size as " + o->X.cols(), 3);
                            
            std::map<float, vector<int>> label_indices;
                
            //getting indices for all labels
            for(int x = 0; x < o->y.size(); x++)
                label_indices[o->y[x]].push_back(x);
                    
            std::map<float, vector<int>>::iterator it = label_indices.begin();
            
            vector<int> t_indices;
            vector<int> v_indices;
            
            int t_size;
            int x;
            
            for(; it != label_indices.end(); it++)
            {
                t_size = ceil(it->second.size()*split);
                
                for(x = 0; x < t_size; x++)
                    t_indices.push_back(it->second[x]);
                    
                for(; x < it->second.size(); x++)
                    v_indices.push_back(it->second[x]);
                
                logger.log("Label is " + to_string(it->first), 3, "\t");
                logger.log("Total size = " + it->second.size(), 3, "\t");
                logger.log("training_size = " + t_size, 3, "\t");
                logger.log("verification size = " + (it->second.size() - t_size), 3, "\t");
                
            }
            
            X_t.resize(o->X.rows(), t_indices.size());
            X_v.resize(o->X.rows(), v_indices.size());
            y_t.resize(t_indices.size());
            y_v.resize(v_indices.size());
            
            sort(t_indices.begin(), t_indices.end());
            
            for(int x = 0; x < t_indices.size(); x++)
            {
                t->X.col(x) = o->X.col(t_indices[x]);
                t->y[x] = o->y[t_indices[x]];
                
                if(o->Z.size() > 0)
                {
                    for(auto const &val : o->Z)
                    {
                        t->Z[val.first].first.push_back(val.second.first[t_indices[x]]);
                        t->Z[val.first].second.push_back(val.second.second[t_indices[x]]);
                    }
                }
            }
            
            sort(v_indices.begin(), v_indices.end());
            
            for(int x = 0; x < v_indices.size(); x++)
            {
                v->X.col(x) = o->X.col(v_indices[x]);
                v->y[x] = o->y[v_indices[x]];
                
                if(o->Z.size() > 0)
                {
                    for(auto const &val : o->Z)
                    {
                        v->Z[val.first].first.push_back(val.second.first[t_indices[x]]);
                        v->Z[val.first].second.push_back(val.second.second[t_indices[x]]);
                    }
                }
            }

            
        }
     
        void CCVData::train_test_split(bool shuffle, float split)
        {
            /* @param X: n_features x n_samples matrix of training data
             * @param y: n_samples vector of training labels
             * @param shuffle: whether or not to shuffle X and y
             * @param[out] X_t, X_v, y_t, y_v: training and validation matrices
             */
             
                                     
            if (shuffle)     // generate shuffle index for the split
                shuffle_data();
                
            if(classification)
                split_stratified(split);
            else
            {        
                // resize training and test sets
                X_t.resize(o->X.rows(),int(o->X.cols()*split));
                X_v.resize(o->X.rows(),int(o->X.cols()*(1-split)));
                y_t.resize(int(o->y.size()*split));
                y_v.resize(int(o->y.size()*(1-split)));
                
                // map training and test sets  
                t->X = MatrixXf::Map(o->X.data(),t->X.rows(),t->X.cols());
                v->X = MatrixXf::Map(o->X.data()+t->X.rows()*t->X.cols(),
                                           v->X.rows(),v->X.cols());

                t->y = VectorXf::Map(o->y.data(),t->y.size());
                v->y = VectorXf::Map(o->y.data()+t->y.size(),v->y.size());
                if(o->Z.size() > 0)
                    split_longitudinal(o->Z, t->Z, v->Z, split);
            }

        }  
        
        void CCVData::split_longitudinal(
                                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z,
                                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z_t,
                                std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf> > > &Z_v,
                                float split)
        {
        
            int size;
            for ( const auto val: Z )
            {
                size = Z[val.first].first.size();
                break;
            }
            
            int testSize = int(size*split);
            int validateSize = int(size*(1-split));
                
            for ( const auto &val: Z )
            {
                vector<ArrayXf> _Z_t_v, _Z_t_t, _Z_v_v, _Z_v_t;
                _Z_t_v.assign(Z[val.first].first.begin(), Z[val.first].first.begin()+testSize);
                _Z_t_t.assign(Z[val.first].second.begin(), Z[val.first].second.begin()+testSize);
                _Z_v_v.assign(Z[val.first].first.begin()+testSize, 
                              Z[val.first].first.begin()+testSize+validateSize);
                _Z_v_t.assign(Z[val.first].second.begin()+testSize, 
                              Z[val.first].second.begin()+testSize+validateSize);
                
                Z_t[val.first] = make_pair(_Z_t_v, _Z_t_t);
                Z_v[val.first] = make_pair(_Z_v_v, _Z_v_t);
            }
        }
        
        /// reordering utility for shuffling longitudinal data.
        void CCVData::reorder_longitudinal(vector<ArrayXf> &vec1,
                                           vector<ArrayXf> &vec2,
                                           vector<int> order)
        {
                
//            for( int s = 1, d; s < order.size(); ++ s )
//            {
//                for ( d = order[s]; d < s; d = order[d] );

//                if ( d == s )
//                    while ( d = order[d], d != s )
//                    {
//                    swap(vec1[s], vec1[d]);
//                    swap(vec2[s], vec2[d]);
//                    }
//            }

            for( size_t i = 0; i < order.size(); ++i )
            { 
                // while order[i] is not yet in place 
                // every swap places at least one element in it's proper place
                while(order[i] != order[order[i]] )
                {
                    swap(vec1[order[i]], vec1[order[order[i]]]);
                    swap(vec2[order[i]], vec2[order[order[i]]]);
                    swap(order[i], order[order[i]]);
                }
            }
            
            for(int x = 0; x < order.size(); x++)
                cout << order[x] << "\t";
            cout << "\n";
        }
        
//        void DataRef::reorder_longitudinal(vector<ArrayXf> &vec1, const vector<int>& order)
//        {  
//			vector<int> index = order; 
//			// Fix all elements one by one 
//			for (int i=0; i<index.size(); i++) 
//			{ 
//				// While index[i] and vec1[i] are not fixed 
//				while (index.at(i) != i) 
//				{ 
//					// Store values of the target (or correct)  
//					// position before placing vec1[i] there 
//					int  oldTargetI  = index.at(index.at(i)); 
//					auto oldTargetE  = vec1.at(index.at(i)); 
//		  
//					// Place vec1[i] at its target (or correct) 
//					// position. Also copy corrected index for 
//					// new position 
//					vec1.at(index.at(i)) = vec1.at(i); 
//					index.at(index.at(i)) = index.at(i); 
//		  
//					// Copy old target values to vec1[i] and 
//					// index[i] 
//					index.at(i) = oldTargetI; 
//					vec1.at(i)   = oldTargetE; 
//				} 
//			}   
//        }    
    }
}
