import individualRunner
import numpy as np

# print("Testing an individual")
# testInd = individualRunner.PyPushIndividual()
# print("Created an individual")
# testInd.testFunc()
# print("Ran test function")
# testInd.getDescription()
# print("Attempting to do a prediction")
# print("Predicted: ", testInd.predict(np.array([[1], [4]], np.int32)))

def testIndividual2():
	 newInd = individualRunner.PyPushIndividual("FLOAT.FROMINTEGER INTEGER.STACKDEPTH BOOLEAN.FROMINTEGER INTEGER.STACKDEPTH INTEGER.+ INTEGER.STACKDEPTH")
	 print("Running testIndividual");
	 # new.setProgram(["INTEGER.STACKDEPTH INTEGER.+"])
	 print("Predicting: ", newInd.predict(np.array([[1], [4]], np.int32)))
	 print("Evaluate: ", newInd.evaluate(np.array(["INT"]), np.array([[3], [6]], np.int32)))

def testIndividual(program, input, typeList, expectedOutput):
	 newInd = individualRunner.PyPushIndividual(program)
	 print("Running testIndividual");
	 # new.setProgram(["INTEGER.STACKDEPTH INTEGER.+"])
	 print("Predicting: ", newInd.predict(input))
	 print("Evaluate: ", newInd.evaluate(typeList, expectedOutput))

def testIndividualInput(program, input, typeList, expectedOutput):
	 newInd = individualRunner.PyPushIndividual()
	 print("Running testIndividual");
	 # new.setProgram(["INTEGER.STACKDEPTH INTEGER.+"])
	 print("Predicting: ", newInd.predict(input))
	 print("Evaluate: ", newInd.evaluate(typeList, expectedOutput))

# testIndividual("FLOAT.FROMINTEGER INTEGER.STACKDEPTH BOOLEAN.FROMINTEGER INTEGER.STACKDEPTH INTEGER.+ INTEGER.STACKDEPTH", 
# 				np.array([[1], [4]], np.int32), 
# 				np.array(["INT"]),
# 				np.array([[3], [6]], np.int32))
testIndividual("INTEGER.INPUT_0 STRING.FROMINTEGER FLOAT.FROMINTEGER INTEGER.STACKDEPTH BOOLEAN.FROMINTEGER INTEGER.STACKDEPTH INTEGER.+ INTEGER.STACKDEPTH",
				np.array([[1], [4]], np.int32), 
				np.array(["BOOL"]),
				np.array([[False], [True]], np.bool))

# testIndividualInput("INTEGER.INPUT_0 STRING.FROMINTEGER FLOAT.FROMINTEGER INTEGER.STACKDEPTH BOOLEAN.FROMINTEGER INTEGER.STACKDEPTH INTEGER.+ INTEGER.STACKDEPTH",
# 				np.array([[1], [4]], np.int32), 
# 				np.array(["BOOL"]),
# 				np.array([[False], [True]], np.bool))
