/* FEAT
copyright 2017 William La Cava
license: GNU/GPL v3
*/

#include "params.h"

namespace FT{

    using namespace Util;
    
    CParameters::CParameters(){}
     CParameters::CParameters(int pop_size, int gens, bool classification, 
                    int max_stall, char output_type, int verbosity, string functions, 
                    float xo_rate, unsigned int max_depth, unsigned max_dim,
                    string objectives, bool shuffle, float split, float feedback, 
                    string scorer, string feature_names, bool backprop, int iters,
                    float learning_rate, int batch_size, bool hill_climb,
                    int max_time, bool use_batch, unsigned n_jobs):
            pop_size(pop_size),
            gens(gens),
            classification(classification),
            max_stall(max_stall), 
            cross_rate(xo_rate),
            max_depth(max_depth),
            max_dim(max_dim),
            shuffle(shuffle),
            split(split),
            otype(output_type),
            feedback(feedback),
            backprop(backprop),
            bp(iters, learning_rate, batch_size),
            hillclimb(hillclimb),
            hc(iters, learning_rate),
            max_time(max_time),
            use_batch(use_batch),
            n_jobs(n_jobs)
        {
            set_verbosity(verbosity);
            if (functions.empty())
                functions = "+,-,*,/,^2,^3,sqrt,sin,cos,exp,log,^,"
                      "logit,tanh,gauss,relu,"
                      "split,split_c,"
                      "b2f,c2f,and,or,not,xor,=,<,<=,>,>=,if,ite";
                
            set_functions(functions);
            set_objectives(objectives);
            set_feature_names(feature_names);
            updateSize();     
            set_otypes();
            n_classes = 2;
            set_scorer(scorer);
            set_n_jobs(n_jobs);
        }
    
    CParameters::~CParameters(){}
    
    /*! checks initial parameter settings before training.
     *  make sure scorer is set. 
     *  for classification, check clases and find number.
     */
    void CParameters::init(CData& d)
    {

        if (this->classification)  // setup classification endpoint
        {
           this->set_classes(d.y);       
           this->set_scorer(scorer);
           this->set_sample_weights(d.y); 
        } 
        cout << "types\n"; 
        if (this->dtypes.size()==0)    // set feature types if not set
            this->dtypes = find_dtypes(d.X);
        cout << "set terminals\n"; 
        this->set_terminals(d);
        vector<float> w; 
        cout << "set term weights\n"; 
        this->set_term_weights(w);
        /* if (params.normalize) */
        /*     N.fit_normalize(X,params.dtypes);                   // normalize data */

    }
  
    /// sets current generation
    void CParameters::set_current_gen(int g) { current_gen = g; }
    
    /// sets scorer type
    void CParameters::set_scorer(string sc)
    {
        // TODO: revisit this for brush
        if (sc.empty())
        {
            if (classification && n_classes == 2)
            {
                /* if (ml.compare("LR") || ml.compare("SVM")) */
                /*     scorer = "log"; */
                /* else */
                scorer = "zero_one";
            }
            else if (classification){
                /* if (ml.compare("LR") || ml.compare("SVM")) */
                /*     scorer = "multi_log"; */
                /* else */
                scorer = "bal_zero_one";
            }
            else
                scorer = "mse";
        }
        else
            scorer = sc;
        logger.log("scorer set to " + scorer,3);
    }
    
    /// sets weights for terminals. 
    void CParameters::set_term_weights(const vector<float>& w)
    {           
        cout << "blah1\n";
        string weights;
        float u = 1.0/float(terminals.size());
        term_weights.clear();
        cout << "blah2\n";
        if (w.empty())  // set all weights uniformly
        {
            cout << "setting weights to << " << u << "\n";
            for (unsigned i = 0; i < terminals.size(); ++i)
                term_weights.push_back(u);
        }
        else
        {
            // take abs value of weights
            vector<float> aw = w;
            float weighted_proportion = float(w.size())/float(terminals.size());
            float sum = 0;
            for (unsigned i = 0; i < aw.size(); ++i)
            { 
                aw[i] = fabs(aw[i]); 
                sum += aw[i];
            }
            // normalize weights to one
            for (unsigned i = 0; i < aw.size(); ++i)
            { 
                aw[i] = aw[i]/sum*weighted_proportion;  // awesome!
            }
            int x = 0;
            // assign transformed weights as terminal weights
            for (unsigned i = 0; i < terminals.size(); ++i)
            {
                if(terminals[i]->otype == 'z')
                    term_weights.push_back(u);
                else
                {
                    term_weights.push_back((1-feedback)*u + feedback*aw[x]);
                    ++x;
                }
            }
               
        }
        weights = "term weights: ";
        for (unsigned i = 0; i < terminals.size(); ++i)
        {
            weights += ("(" + terminals.at(i)->name + "(" + terminals.at(i)->otype 
                        + ")," 
                        + std::to_string(term_weights.at(i)) + "), ") ; 
        }
        weights += "\n";
        logger.log(weights, 2);
    }
    
    void CParameters::updateSize()
    {
    	max_size = (pow(2,max_depth+1)-1)*max_dim;
    }
    
    /// set max depth of programs
    void CParameters::set_max_depth(unsigned int max_depth)
    {
    	this->max_depth = max_depth;
    	updateSize();
    }
    
    /// set maximum dimensionality of programs
    void CParameters::set_max_dim(unsigned int max_dim)
    {
    	this->max_dim = max_dim;
    	updateSize();
    }

    void CParameters::set_otype(char ot){ otype = ot; set_otypes();}
    
    void CParameters::set_ttypes()
    {
        ttypes.clear();
        // set terminal types
        for (const auto& t: terminals)
        {
            if (!in(ttypes,t->otype)) 
                ttypes.push_back(t->otype);
        }
    }

    /// set the output types of programs
    void CParameters::set_otypes(bool terminals_set)
    {
        otypes.clear();
        // set output types
        switch (otype)
        { 
            case 'b': otypes.push_back('b'); break;
            case 'f': otypes.push_back('f'); break;
            //case 'c': otypes.push_back('c'); break;
            default: 
            {
                // if terminals are all boolean, remove floating point functions
                if (ttypes.size()==1 && ttypes[0]=='b')
                {
                    logger.log("otypes is size 1 and otypes[0]==b\nerasing functions...\n",2);
                    size_t n = functions.size();
                    for (vector<int>::size_type i =n-1; 
                         i != (std::vector<int>::size_type) -1; i--){
                        if (functions.at(i)->arity['f'] >0){
                            logger.log("erasing function " + functions.at(i)->name + "\n", 2);
                            functions.erase(functions.begin()+i);
                        }
                    }
                    
                    otype = 'b';
                    otypes.push_back('b');
                }           
                else
                {
                    otypes.push_back('b');
                    otypes.push_back('f');
                }
                
                //erasing categorical nodes if no categorical stack exists  
                if (terminals_set && !in(ttypes, 'c'))
                {
                    size_t n = functions.size();
                    for (vector<int>::size_type i =n-1; 
                         i != (std::vector<int>::size_type) -1; i--){
                        if (functions.at(i)->arity['c'] >0){
                            logger.log("erasing function " + functions.at(i)->name + "\n", 2);
                            functions.erase(functions.begin()+i);
                        }
                    }
                }         
                break;
            }
        }

    }
    
    std::unique_ptr<CNode> CParameters::createNode(string str,
                                                 float d_val,
                                                 bool b_val,
                                                 size_t loc,
                                                 string name)
    {
        // algebraic operators
    	if (str.compare("+") == 0) 
    		return std::unique_ptr<CNode>(new NodeAdd({1.0, 1.0}));
        
        else if (str.compare("-") == 0)
    		return std::unique_ptr<CNode>(new NodeSubtract({1.0, 1.0}));

        else if (str.compare("*") == 0)
    		return std::unique_ptr<CNode>(new NodeMultiply({1.0, 1.0}));

     	else if (str.compare("/") == 0)
    		return std::unique_ptr<CNode>(new NodeDivide({1.0, 1.0}));

        else if (str.compare("sqrt") == 0)
    		return std::unique_ptr<CNode>(new NodeSqrt({1.0}));
    	
    	else if (str.compare("sin") == 0)
    		return std::unique_ptr<CNode>(new NodeSin({1.0}));
    		
    	else if (str.compare("cos") == 0)
    		return std::unique_ptr<CNode>(new NodeCos({1.0}));
    		
    	else if (str.compare("tanh")==0)
            return std::unique_ptr<CNode>(new NodeTanh({1.0}));
    	   
        else if (str.compare("^2") == 0)
    		return std::unique_ptr<CNode>(new NodeSquare({1.0}));
 	
        else if (str.compare("^3") == 0)
    		return std::unique_ptr<CNode>(new NodeCube({1.0}));
    	
        else if (str.compare("^") == 0)
    		return std::unique_ptr<CNode>(new NodeExponent({1.0, 1.0}));

        else if (str.compare("exp") == 0)
    		return std::unique_ptr<CNode>(new NodeExponential({1.0}));
    		
    	else if (str.compare("gauss")==0)
            return std::unique_ptr<CNode>(new NodeGaussian({1.0}));
        
        else if (str.compare("gauss2d")==0)
            return std::unique_ptr<CNode>(new Node2dGaussian({1.0, 1.0}));

        else if (str.compare("log") == 0)
    		return std::unique_ptr<CNode>(new NodeLog({1.0}));   
    		
    	else if (str.compare("logit")==0)
            return std::unique_ptr<CNode>(new NodeLogit({1.0}));

        else if (str.compare("relu")==0)
            return std::unique_ptr<CNode>(new NodeRelu({1.0}));

        else if (str.compare("b2f")==0)
            return std::unique_ptr<CNode>(new NodeFloat<bool>());
        
        else if (str.compare("c2f")==0)
            return std::unique_ptr<CNode>(new NodeFloat<int>());
        
        // logical operators
        else if (str.compare("and") == 0)
    		return std::unique_ptr<CNode>(new NodeAnd());
       
    	else if (str.compare("or") == 0)
    		return std::unique_ptr<CNode>(new NodeOr());
   		
     	else if (str.compare("not") == 0)
    		return std::unique_ptr<CNode>(new NodeNot());
    		
    	else if (str.compare("xor")==0)
            return std::unique_ptr<CNode>(new NodeXor());
   		
    	else if (str.compare("=") == 0)
    		return std::unique_ptr<CNode>(new NodeEqual());
    		
        else if (str.compare(">") == 0)
    		return std::unique_ptr<CNode>(new NodeGreaterThan());

    	else if (str.compare(">=") == 0)
    		return std::unique_ptr<CNode>(new NodeGEQ());        

    	else if (str.compare("<") == 0)
    		return std::unique_ptr<CNode>(new NodeLessThan());
    	
    	else if (str.compare("<=") == 0)
    		return std::unique_ptr<CNode>(new NodeLEQ());
 
            else if (str.compare("split") == 0)
      		    return std::unique_ptr<CNode>(new NodeSplit<float>());
      		
      		else if (str.compare("split_c") == 0)
      		    return std::unique_ptr<CNode>(new NodeSplit<int>());
    	
     	else if (str.compare("if") == 0)
    		return std::unique_ptr<CNode>(new NodeIf());   	    		
        	
    	else if (str.compare("ite") == 0)
    		return std::unique_ptr<CNode>(new NodeIfThenElse());
    		
    	else if (str.compare("step")==0)
            return std::unique_ptr<CNode>(new NodeStep());
            
        else if (str.compare("sign")==0)
            return std::unique_ptr<CNode>(new NodeSign());
           
        // longitudinal nodes
        else if (str.compare("mean")==0)
            return std::unique_ptr<CNode>(new NodeMean());
            
        else if (str.compare("median")==0)
            return std::unique_ptr<CNode>(new NodeMedian());
            
        else if (str.compare("max")==0)
            return std::unique_ptr<CNode>(new NodeMax());
        
        else if (str.compare("min")==0)
            return std::unique_ptr<CNode>(new NodeMin());
        
        else if (str.compare("variance")==0)
            return std::unique_ptr<CNode>(new NodeVar());
            
        else if (str.compare("skew")==0)
            return std::unique_ptr<CNode>(new NodeSkew());
            
        else if (str.compare("kurtosis")==0)
            return std::unique_ptr<CNode>(new NodeKurtosis());
            
        else if (str.compare("slope")==0)
            return std::unique_ptr<CNode>(new NodeSlope());
            
        else if (str.compare("count")==0)
            return std::unique_ptr<CNode>(new NodeCount());
        
        else if (str.compare("recent")==0)
            return std::unique_ptr<CNode>(new NodeRecent());

        // variables and constants
        else if (str.compare("x") == 0)
        { 
            if(dtypes.size() == 0)
            {
                if (feature_names.size() == 0)
                    return std::unique_ptr<CNode>(new NodeVariable<float>(loc));
                else
                    return std::unique_ptr<CNode>(new NodeVariable<float>(loc,'f', feature_names.at(loc)));
            }
            else if (feature_names.size() == 0)
            {
                switch(dtypes[loc])
                {
                    case 'b': return std::unique_ptr<CNode>(new NodeVariable<bool>(loc,
                                                                                  dtypes[loc]));
                    case 'c': return std::unique_ptr<CNode>(new NodeVariable<int>(loc,
                                                                                  dtypes[loc]));
                    case 'f': return std::unique_ptr<CNode>(new NodeVariable<float>(loc,
                                                                                  dtypes[loc]));
                }
            }
            else
            {
                switch(dtypes[loc])
                {
                    case 'b': return std::unique_ptr<CNode>(new NodeVariable<bool>(loc, 
                                                           dtypes[loc],feature_names.at(loc)));
                    
                    case 'c': return std::unique_ptr<CNode>(new NodeVariable<int>(loc, 
                                                           dtypes[loc],feature_names.at(loc)));
                    
                    case 'f': return std::unique_ptr<CNode>(new NodeVariable<float>(loc, 
                                                           dtypes[loc],feature_names.at(loc)));
                }
            }
        }
            
        else if (str.compare("kb")==0)
            return std::unique_ptr<CNode>(new NodeConstant(b_val));
            
        else if (str.compare("kd")==0)
            return std::unique_ptr<CNode>(new NodeConstant(d_val));
            
        else if (str.compare("z")==0)
            return std::unique_ptr<CNode>(new NodeLongitudinal(name));
        else
            HANDLE_ERROR_THROW("Error: no node named '" + str + "' exists."); 
        
        //TODO: add squashing functions, time delay functions, and stats functions
    	
    }

    void CParameters::set_feature_names(string fn)
    {
        if (fn.empty())
            feature_names.clear();
        else
        {
            fn += ',';      // add delimiter to end
            string delim=",";
            size_t pos = 0;
            string token;
            while ((pos = fn.find(delim)) != string::npos) 
            {
                token = fn.substr(0, pos);
                feature_names.push_back(token);
                fn.erase(0, pos + delim.length());
            }
        }
    }
    
    void CParameters::set_functions(string fs)
    {
        /*! 
         * Input: 
         *
         *		fs: string of comma-separated Node names
         *
         * Output:
         *
         *		modifies functions 
         *
         */

        fs += ',';          // add delimiter to end 
        string delim = ",";
        size_t pos = 0;
        string token;
        functions.clear();
        while ((pos = fs.find(delim)) != string::npos) 
        {
            token = fs.substr(0, pos);

        	functions.push_back(createNode(token));

            fs.erase(0, pos + delim.length());
        } 
        
        string log_msg = "functions set to [";
        for (const auto& f: functions) log_msg += f->name + ", "; 
        log_msg += "]\n";
        
        logger.log(log_msg, 2);
        
        // reset output types
        set_otypes();
    }

    void CParameters::set_terminals(const CData& d)
    {
        /*!
         * based on number of features.
         */
        terminals.clear();
        num_features = d.X.rows(); 
        for (size_t i = 0; i < num_features; ++i)
            terminals.push_back(createNode(string("x"), 0, 0, i));
    	
       
        for (const auto &val : d.Z)
        {
            longitudinalMap.push_back(val.first);
            terminals.push_back(createNode(string("z"), 0, 0, 0, val.first));
        }
        /* for (const auto& t : terminals) */ 
        /*     cout << t->name << " " ; */
        /* cout << "\n"; */
        // reset output types
        set_ttypes();
        
        set_otypes(true);
    }

    void CParameters::set_objectives(string obj)
    {
        /*! Input: obj, a comma-separated list of objectives
         */

        obj += ',';          // add delimiter to end 
        string delim = ",";
        size_t pos = 0;
        string token;
        objectives.clear();
        while ((pos = obj.find(delim)) != string::npos) 
        {
            token = obj.substr(0, pos);
            objectives.push_back(token);
            obj.erase(0, pos + delim.length());
        }
    }
    
    void CParameters::set_verbosity(int verbosity)
    {
        this->verbosity = verbosity;
        logger.set_log_level(verbosity);
    }

    void CParameters::set_classes(VectorXf& y)
    {
        classes.clear();

        // set class labels
        vector<float> uc = unique(y);
        
        n_classes = uc.size();

        for (auto c : uc)
            classes.push_back(int(c));
    }

    void CParameters::set_sample_weights(VectorXf& y)
    {
        // set class weights
        /* cout << "setting sample weights\n"; */
        class_weights.resize(n_classes);
        sample_weights.clear();
        for (unsigned i = 0; i < n_classes; ++i){
            class_weights.at(i) = float((y.cast<int>().array() == int(classes.at(i))).count())/y.size(); 
            class_weights.at(i) = (1 - class_weights.at(i))*float(n_classes);
        }
        /* cout << "y size: " << y.size() << "\n"; */
        for (unsigned i = 0; i < y.size(); ++i)
            sample_weights.push_back(class_weights.at(int(y(i))));
        /* std::cout << "sample weights size: " << sample_weights.size() << "\n"; */
        /* std::cout << "class weights: "; */ 
        /* for (auto c : class_weights) std::cout << c << " " ; std::cout << "\n"; */
        /* std::cout << "number of classes: " << n_classes << "\n"; */
    }
}
