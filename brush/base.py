# base learner. 
from sklearn.base import BaseEstimator, RegressorMixin, ClassifierMixin
import time
import numpy as np
# brush imports
# from brush.params import Parameters
from brush.params import Parameters
from brush.population import Population
from brush.selection import Selection
from brush.data import CVData 
from brush.evaluation import Evaluation
import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
formatter = logging.Formatter('%(module)s: %(levelname)s: %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

class BrushBase(BaseEstimator):
    """Base class for Brush learners.
    """
    #TODO survival class not defined
    def __init__(self, pop_size=100, offspring_size=100, classification=False,
            selection='lexicase', survival='offspring', xo_rate=0.5, functions=None,
            n_jobs=-1, verbosity=1, max_stall=0, logfile=None, objectives=None,
            scoring_function=None):
        logger.debug('initializing')
        self.pop_size = pop_size
        self.offspring_size = offspring_size
        self.classification = classification
        self.regression = not self.classification
        self.selection = selection
        self.xo_rate = xo_rate
        self.functions = functions
        self.n_jobs = n_jobs
        self.verbosity = verbosity
        self.max_stall = max_stall
        self.exit_on_stall = max_stall > 0
        self.logfile = logfile
        self.objectives = objectives
        self.params = Parameters()
        self.selection = Selection(selection.encode())
        self.survival = Selection(survival.encode())
        self.evaluation = Evaluation(b'mse')
        self.pop = Population(10)
        
        print("Population size is ", self.pop.size())

    def _fit_init(self):
        """Routines to run at the beginning of the fit method"""
        logger.debug('fit initialization')
        self.stall_count = 0
        self.params.current_generation = 0
        
        # initialize any parameters that depend on the data

    def fit(self,features,target):
        """fit a model"""
        
        logger.debug('setting the CVData class')
        # initialize the data class
        self.data = CVData(features, target, self.params.classification)
        logger.debug('train test split')
        self.data.train_test_split(self.params.shuffle, self.params.split)

        self._fit_init()
        
        logger.debug('running generational loop')

        while not self._terminate():
            self._run_generation()
        
        logger.debug('selecting final model')
        self._select_final_model()
        
        return self

    def _run_generation(data): 
        """Runs one generation of the population on data"""
        logger.debug('get batch')
        data = self.data.get_batch() if self.params.use_batch else data

        logger.debug('selection')
        parents = self.selection.select(self.pop, self.params)
        logger.debug('variation')
        self.variation.vary(self.pop, parents, self.params, data)
        logger.debug('fitness')
        self.evaluation.fitness(offspring, data, self.params, offspring=True)
        logger.debug('survival')
        survivors = self.survival.survive(self.pop)
        logger.debug('update best')
        self.update_best()
        self.pop.update(survivors)
        self.params.current_generation += 1

    # def _get_train_batch():
    #     """Returns data to train on, potentially a batch"""
    #     if self.params.use_batch:
    #         return data.get_batch(params.batch_size)
    #     else:
    #         return data

    def _select_final_model():
        """Evaluates population/archive on validation set and chooses best"""

        if (params.split < 1):
            self.evaluation.fitness(self.pop, data.val, self.params, 
                    offspring=False, validation=True)
            self.update_best(data, true)

    def predict(self, X):
        """generate prediction"""
        return self._best_estimator.predict(X)

    def _terminate(self):
        """Checks whether or not to stop"""
        return (self.current_generation == self.generations or
                (self.exit_on_stall and self.stall_count > self.max_stall))

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
