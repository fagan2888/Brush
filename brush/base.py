# base learner. 
from sklearn.base import BaseEstimator, RegressorMixin, ClassifierMixin
import time
import numpy as np
# brush imports
from .params import Parameters
from .population import Population
from .selection import Selection, Survival
from .data import Data

class BrushBase(BestEstimator):
    """Base class for Brush learners.
    """
    def __init__(self, pop_size=100, offspring_size=100, classification=False,
            selection=None, survival=None, xo_rate=0.5, functions=None,
            n_jobs=-1, verbosity=1, max_stall=0, logfile=None, objectives=None,
            scoring_function=None):
        self.pop_size = pop_size
        self.offspring_size = offspring_size
        self.classification = classification
        self.regression = not self.classification
        self.selection = selection
        self.survival = survival
        self.xo_rate = xo_rate
        self.functions = functions
        self.n_jobs = n_jobs
        self.verbosity = verbosity
        self.max_stall = max_stall
        self.exit_on_stall = max_stall > 0
        self.logfile = logfile
        self.objectives = objectives

        self.params = Parameters()
        self.selection = Selection(self.params)
        self.survival = Survival(self.params)
        self.pop = Population(self.params)

    def _fit_init(self):
        """Routines to run at the beginning of the fit method"""

    def fit(self,features,target):
        """fit a model"""
        X, y = check_X_y(features, target, accept_sparse=True, dtype=None)

        while not self._terminate():
            parents = self.selection.select(self.pop)
            offspring = self.variation.vary(self.pop)
            self.evaluate(offspring)
            self.survival.survive(self.pop)
            self.update_best()

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
    self.classification=False
    self.regression = True
    self.scoring_function = 'r2_score'

class BrushClassifier(BrushBase, ClassifierMixin):
    self.classification=False
    self.regression = True
    self.scoring_function = 'accuracy'
