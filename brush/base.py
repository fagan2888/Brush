# base learner. 
from sklearn.base import BaseEstimator, RegressorMixin, ClassifierMixin
import time
import numpy as np
# brush imports
from brush.params import Parameters
from brush.population import Population
from brush.selection import Selection
from brush.data import Data

class BrushBase(BaseEstimator):
    """Base class for Brush learners.
    """
    #TODO survival class not defined
    def __init__(self, pop_size=100, offspring_size=100, classification=False,
            selection=None, survial=None, xo_rate=0.5, functions=None,
            n_jobs=-1, verbosity=1, max_stall=0, logfile=None, objectives=None,
            scoring_function=None):
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
        self.selection = Selection(selection)
        self.survival = Selection(survival)
        self.evaluation = Evaluation()
        self.pop = Population(10)
        
        print("Population size is ", self.pop.size())

    def _fit_init(self):
        """Routines to run at the beginning of the fit method"""
        self.stall_count = 0
        self.params.current_generation = 0
        
        # initialize any parameters that depend on the data

    def fit(self,features,target):
        """fit a model"""
        
        # initialize the data class
        self.data = CVData(features, target, params.classification)
        self.data.train_test_split(self.params.shuffle, self.params.split)

        self._fit_init()

        while not self._terminate():
            self._run_generation()
        
        self._select_final_model()
        
        return self

    def _run_generation(data): 
        """Runs one generation of the population on data"""
        data = self.data.get_batch() if self.params.use_batch else data

        parents = self.selection.select(self.pop, self.params)
        self.variation.vary(self.pop, parents, self.params, data)
        self.evaluation.fitness(offspring, data, self.params, offspring=True)
        survivors = self.survival.survive(self.pop)
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
