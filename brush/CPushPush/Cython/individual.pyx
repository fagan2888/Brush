# distutils: language=c++
# distutils: include_dirs = ../src/

import ctypes
import numpy as np 
cimport numpy as np
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "../src/State.h":
	cppclass State:
		vector[int] getInt();
		int popInt();
		vector[bool] getBool();
		bool popBool();
		# vector[float] getFloat();
		string getPrintString();
		vector[double] getDouble();
		double popDouble();

# Cython definition of CPushPush underbelly
cdef extern from "../src/pushIndividual.h" namespace "Brush":
	cppclass PushIndividual:
		PushIndividual() except +
		PushIndividual(string instructionSet) except +
		# PushIndividual(vector[string] instructionSet, int pop_size, int gens) except +
		
		vector[int] predict(int* x, int numRows, int numCols);
		string getDescription();
		double getFitness();
		unsigned getEffort();
		State getState();
		vector[State] getFullState();


		# void fit(float* X, int rowsX, float* y, int lenY)
		# vector[int] predict(float* x, int x_size)

# Define the API for interacting with CPushPush
cdef class PyPushIndividual:
	cdef PushIndividual* pIndividual
	cdef vector[State] lastOutput

	def __cinit__(self):
		# Initialize hyper parameters
		print("Calling Cython wrapped init")
		self.pIndividual = new PushIndividual()
		print("Done with cython initialization")

	def __cinit__(self, string program):
		print("Running string initializer")
		self.pIndividual = new PushIndividual(program)

	def __dealloc__(self):
		del self.pIndividual

	def predict(self, np.ndarray X):
		cdef np.ndarray[int, ndim=2, mode='c'] arr_x;
		# cdef np.ndarray[string, ndim=1] inputSequence;
		print("Defined new array")
		arr_x = np.asanyarray(X, dtype=np.intc)
		# inputSequence = np.asanyarray(labels, dtype=string)
		print("Asanyarray returned")
		returnValue = self.pIndividual.predict(&arr_x[0,0], X.shape[0], X.shape[1])
		outputState = self.pIndividual.getState();
		print("Output state from cython: " + outputState.getPrintString());
		# outputStateVec = self.pIndividual.getFullState();
		# cdef vector[State] stateVec = self.pIndividual.getFullState();
		self.lastOutput = self.pIndividual.getFullState();
		print("Got the full state!");
		for i in self.lastOutput:
			print("Iterating through elements!");
			print(i.getPrintString());
		
		return returnValue;

	# def fit(self, np.ndarray X, ): TODO

	def testFunc(self):
		print("Testing functionality")

	def getDescription(self):
		print(self.pIndividual.getDescription())

	# Evaluate using MSE
	def evaluate(self, np.ndarray typeList, np.ndarray values):
		error = 0.0;

		# Iterate through samples
		sample = 0;
		cdef double testDouble;
		cdef int testInt;
		cdef bool testBool;
		samples = 0;
		for i in self.lastOutput:
			index = 0;
			# Evaluate against expected output
			for type in typeList:
				if type == "DOUBLE":
					testDouble = i.popDouble();
					error += (testDouble - values[sample, index]) * (testDouble - values[sample, index]);
					samples += 1;
				elif type == "INT":
					testInt = i.popInt();
					error += (testInt - values[sample, index]) * (testInt - values[sample, index]);
					samples += 1;
					print("Adding error for ", testInt, " and ", values[sample, index]);
				elif type == "BOOL":
					testBool = i.popBool();
					error += (testBool != values[sample, index]);
					samples += 1;
				index += 1;
			sample += 1;
		print("Error for ", samples, " is ", error);
		return error/samples if samples > 0 else -1;