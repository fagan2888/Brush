/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/
#ifndef METRICS_H
#define METRICS_H
#include "../../util/src/utils.h"
using Eigen::Map;
using Eigen::ArrayXf;

namespace FT
{
    namespace Eval
    {

        /* Scoring functions */

        // Squared difference
        VectorXf squared_difference(const VectorXf& y,
                                    const VectorXf& yhat,
                                    const vector<float>& weights=vector<float>());

        // Derivative of squared difference with respec to yhat
        VectorXf d_squared_difference(const VectorXf& y,
                                      const VectorXf& yhat,
                                      const vector<float>& weights=vector<float>());
       
        /// mean squared error
        float mse(const VectorXf& y, const VectorXf& yhat, VectorXf& loss, 
                   const vector<float>& weights=vector<float>() );
        
        VectorXf log_loss(const VectorXf& y, const VectorXf& yhat, 
                          const vector<float>& class_weights=vector<float>());
       
        /// log loss
        float mean_log_loss(const VectorXf& y, const VectorXf& yhat, VectorXf& loss,
                          const vector<float>& class_weights = vector<float>());

        VectorXf d_log_loss(const VectorXf& y, const VectorXf& yhat, 
                            const vector<float>& class_weights=vector<float>());

        /// multinomial log loss
        VectorXf multi_log_loss(const VectorXf& y, const ArrayXXf& confidences, 
                const vector<float>& class_weights=vector<float>());

        float mean_multi_log_loss(const VectorXf& y, const ArrayXXf& confidences, VectorXf& loss,
                          const vector<float>& class_weights=vector<float>());
                                                    
        /// 1 - balanced accuracy 
        float bal_zero_one_loss(const VectorXf& y, const VectorXf& yhat, VectorXf& loss, 
                   const vector<float>& class_weights=vector<float>() );

        float zero_one_loss(const VectorXf& y, const VectorXf& yhat, VectorXf& loss, 
                   const vector<float>& class_weights=vector<float>() );
        
    } // metrics
} // FT

#endif

