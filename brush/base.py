# base learner. 
from sklearn.base import BaseEstimator, RegressorMixin, ClassifierMixin
import time
import numpy as np
# brush imports
# from brush.params import Parameters
from brush.params import Parameters
from brush.population import Population, Individual
from brush.selection import Selection
from brush.data import CVData 
from brush.evaluation import Evaluation
import pdb

import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
formatter = logging.Formatter('%(module)s: %(levelname)s: %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

class BrushBase(BaseEstimator):
    """Base class for Brush learners.

    Parameters
    ----------
    pop_size : int (default=100)
        The size of the population.

    max_depth : int (default=6)
        The maximum depth of a program. 

    max_dim : int (default=1)
        The maximum number of independent outputs in program. Unless Feat or
        multiclass classification is happening, this should probably be 1. 

    gens : int (default=100)
        The maximum number of iterations of optimization. 

    max_time : int (default=0)
        The maximum time in seconds to run. If -1, not used. 
        
    max_stall : int (default=0)
        The maximum iterations without improvement in the validation loss of the
        median population member. If 0, not used. 

    classification : bool (default=False)
        Whether to conduct classification. Default is false, i.e. regression.

    objectives: str (default='fitness,complexity')
        Learning objectives. Used with multi-objective search. 

    scoring_function: str (default='')
        Scoring function to use. If '', it is set to 'mse' for regression,
        'logloss' for classification. 

    functions: str (default='')
        Functions to use to compose programs. If '', all are used. All corresponds
        to: "+,-,*,/,^2,^3,sqrt,sin,cos,exp,log,^,logit,tanh,gauss,relu,split,
        split_c,b2f,c2f,and,or,not,xor,=,<,<=,>,>=,if,ite"

    output_type: {'a','b','f'} (default='a')
        Types of outputs to allow. 'a': all types; 'b': just boolean; 'f': just
        floating point.

    selection: {'lexicase', 'nsga2', 'random', 'simanneal'} 
        (default:'lexicase')
        The selection algorithm to use. 
    
    survival: {'lexicase', 'nsga2', 'offspring', 'random', 'simanneal'} 
        (default:'lexicase')
        The survival algorithm to use. 

    xo_rate: float (default=0.5)
        The proportion of variation that is crossover. The mutation proportion is its
        complement. 

    mut_rate: float or None (default=None)
        Controls the amount of mutation per mutation event. If None, the rate is set
        as 1/length of the program.

    n_jobs: int (default:-1)
        The number of parallel jobs. If -1, use all cores. 

    verbosity: {0, 1, 2, 3} (default:1)
        The amount of printing. 0: none, 1: a lot, 2: a ton, 3: a bajillion

    logfile: str (default:'')
        A file to print runtime statistics to

    feedback: float (default:0.0)
        Controls how much the weights on a program's edges bias variation locations.
        By default this is not used.
        
    """
    #TODO survival class not defined
    def __init__(self, 
            # size and termination criteria
            pop_size=100,  gens=100, max_time=0, max_stall=0, max_depth=6, 
            max_dim=1,
            # problem definition and evaluation
            classification=False, objectives='fitness,complexity', scorer='', 
            functions='', output_type='a',
            # selection and survival algorithms
            selection='lexicase', survival='offspring', 
            # variation
            xo_rate=0.5, mut_rate=None, feedback=0.0,
            # parallelism
            n_jobs=-1, 
            # constant optimization
            backprop=False, hill_climb=False, iters=1, learning_rate=0.1,
            # data handling
            batch_size=0, shuffle=True, split=0.5,
            # reporting
            verbosity=1, logfile=None
            ):
        logger.debug('initializing')
        #TODO: go thru parameters and filter out ones not in use. 
        # rename parameters to proper names in cython wrapper

        self.use_batch = batch_size>0
        self.exit_on_stall = max_stall > 0
        self.feature_names = ''
        self.selection = selection.encode()
        self.survival = survival.encode()

        self.P = Parameters(pop_size, gens, classification, max_stall,
                output_type.encode(),
                verbosity, functions.encode(), xo_rate, max_depth,  max_dim, 
                objectives.encode(), 
                shuffle, split, feedback, scorer.encode(), 
                self.feature_names.encode(), backprop, 
                iters, learning_rate, batch_size, hill_climb, max_time, 
                self.use_batch)

        self.random_state = None
        self._best_estimator = Individual() 

    def _fit_init(self):
        """Routines to run at the beginning of the fit method"""
        logger.debug('fit initialization')
        self.selection = Selection(self.selection)
        self.survival = Selection(self.survival)
        
        self.stall_count = 0
        self.P.current_gen = 0
        
        self.P.init()
        if self.classification:
            self.P.set_classes(self.data.train.y)
            self.P.set_scorer(self.scorer)
        # TODO: set data types
        # TODO: normralize data?

        self.evaluation = Evaluation(self.P.scorer)
        self.pop = Population(self.P.pop_size)
        # initialize any parameters that depend on the data
        # self.P.set_terminals(self.data.train)
        # fit initial model

        self.P.set_terminals(self.data.train)

    def fit(self,features,target):
        """fit a model"""
        
        logger.debug('setting the CVData class')
        # initialize the data class
        self.data = CVData(features, target, self.P.classification, self.P.shuffle,
                           self.P.split, self.random_state)
        # pdb.set_trace()
        # logger.debug('train test split')
        # self.data.train_test_split(self.P.shuffle, self.P.split)
        print('self.data:',self.data,dir(self.data))
        print('self.data X_train:',self.data.train.X[:10])
        print('self.data X_test:',self.data.val.X[:10])
        print('self.data y_train:',self.data.train.y[:10])
        print('self.data y_test:',self.data.val.y[:10])

        self._fit_init()
        
        self.pop.init(self._best_estimator, self.P, False)
        self.evaluation.fitness(self.pop, data, self.P, offspring=False)

        logger.debug('running generational loop')

        while not self._terminate():
            self._run_generation()
        
        logger.debug('selecting final model')
        self._select_final_model()
        
        return self

    def _run_generation(self): 
        """Runs one generation of the population on data"""
        logger.debug('get batch')
        data = self.data.train.get_batch() if self.P.use_batch else self.data.train
        print('data:',data,dir(data))
        print('data X_train:',data.train.X[:10])
        print('data X_test:',data.val.X[:10])
        print('data y_train:',data.train.y[:10])
        print('data y_test:',data.val.y[:10])
        logger.debug('selection')
        parents = self.selection.select(self.pop, self.P)
        logger.debug('variation')
        self.variation.vary(self.pop, parents, self.P, data)
        logger.debug('fitness')
        self.evaluation.fitness(offspring, data, self.P, offspring=True)
        logger.debug('survival')
        survivors = self.survival.survive(self.pop)
        logger.debug('update best')
        self.update_best()
        self.pop.update(survivors)
        self.P.current_gen+= 1

    # def _get_train_batch():
    #     """Returns data to train on, potentially a batch"""
    #     if self.P.use_batch:
    #         return data.get_batch(params.batch_size)
    #     else:
    #         return data

    def _select_final_model():
        """Evaluates population/archive on validation set and chooses best"""

        if (params.split < 1):
            self.evaluation.fitness(self.pop, data.val, self.P, 
                    offspring=False, validation=True)
            self.update_best(data, true)

    def predict(self, X):
        """generate prediction"""
        return self._best_estimator.predict(X)

    def _terminate(self):
        """Checks whether or not to stop"""
        return (self.P.current_gen == self.P.gens or
               (self.exit_on_stall and self.stall_count > self.P.max_stall))

    def update_best(self):
        """Keeps track of the best current estimator"""
        # do stuff

class BrushRegressor(BrushBase, RegressorMixin):
    classification=False
    regression=True
    scoring_function = 'r2_score'

class BrushClassifier(BrushBase, ClassifierMixin):
    classification=True
    regression=False
    scoring_function = 'balanced_accuracy'
