# tests the Individual class.


from brush.evaluation import Evaluation
from brush.data import Data
from brush.population import Population
from brush.parameters import Parameters
from sklearn.datasets import load_diabetes

'''
TODO
	- mse
	- bal_accuracy
	- log_loss
	- multi_log_loss
	- out_ml
DONE
	- fitness
'''


def test_fitness():
    """Check evaluation fitness"""
    diabetes = load_diabetes()
    Data d = Data(diabetes.data, diabetes.target)
    #TODO push nodes into population
    Population pop = Population()
    Parameters params = Parameters()
    Evaluation eval_obj = Evaluation("mse")
    eval_obj.fitness(pop, d, params, False, False) 
    #TODO assert population individual fitness

