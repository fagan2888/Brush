# Brush

Bill's Rewrite of puSH
or a Brief Reduction of puSH

Brush is a lightweight programming language for constructing programs.
Most of the computation is done in C++, but it is written to give users flexible access to programs.

It supports genetic programming using linear, tree, multi-output tree, and directed acyclic graph encodings.
Unlike most genetic programming representations, Brush embeds weights in the edges of nodes that are differentiable.
That way, it can support gradient descent via backpropagation. 
It is multithreaded and GPU-enabled.

## Structure

Brush uses Cython to glue c++ code to python code.

Python handles:

- the main optimization loop
- calls to evaluate individuals

## Basic Usage

```
from brush.program import Tree
from brush.data import Data
import pandas as pd

# load some data
df = pd.read_csv('data.csv')
dat = Data(X = df.drop('label'), y = df['label'])

program = Tree()    # initializes random tree
output = Tree.evaluate(dat)

```
