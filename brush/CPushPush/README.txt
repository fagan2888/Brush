State of the Projects
----------------------------------
The project currently supports the creation of push individuals via a Cython script located in the Cython folder. 

Things that need to be addressed
**********************************
1.) Creating Input instructions and registering program
	Currently the class defined in the file InputInstruction.h is supposed to handle registering instructions which at run-time can push the input values for the current sample onto the stacks. The mechnanism for doing so is registering instructions via the make_new_input_instruction method defined in inputInstruction.cpp using the static_initializer. Instructions need to be registered as the parse function used to convert a program into its corresponding push executable code needs to be able to lookup the registered function in the String2CodeMap (defined in Code.h). Failure to register a function in this map will cause the parser to fail to be able to translate provided code which will cause the individual to act unpredictably (and usually segFault).

	The major current issue with the InputInstruction class is the presumed failure of a linker to associate the method calls in the Data class with the function defined in the InputInstruction class. Because of this failure execution fails at runtime. Proposed solutions to this issue are moving the calls to make_new_input_instruction to the environment class and moving the register function for InputInstructions to another class.
		Things to note when approaching a solution to this problem.
			1.) Potential for a circular dependency between Env, InputInstruction, and Data. Since the Env needs to know what data is associated with the input (which is the responsibility of the Data class to hold and track) it needs to have the data class as a member. In addition, the environment serves as the main object passed around through the different components of Push and as such it is how access to the Data object is made available to those who need it. Since instructions only take as input the an Env variable the input instruction must access the Data object reference through the reference to the Env they get. Thus there is a circular dependence where the Env needs to know about the Data class to define itself which itself requires knowledge about InputInstructions to call the register function, however the inputInstructions require knowledge about the env.
			2.) The dependency between creating a new program and registering the functions. Since creating a new program requires parsing a string and checking it against the String2CodeMap, the corresponding instructions must be registered by the program ahead of time. Thus the register call for InputInstructions must be done ahead of time for any code that needs to use input. The catch here is that input instructions can only be known once the type of input is known which is currently done in the predict function. This would mean an individual can't create its code until the input is given to it.
				Thus the parse function could not be called until predict is called (but ideally would not want to call parse every time predict is called, even caching the code defintion isn't an elegant solutions to this problem). Instead it might be beneficial to modify the constructor to add what types of inputs the individual would expect so that it could call the register functions ahead of time (as the inputInstructions don't depend on the data being populated when they are registered since they just access the data object defined in the Env at runtime).

2.) Creating mechanism to choose which instruction set to use.
	Currently the system just defaults to include all instructions.

3.) Testing string instructions
4.) Change file structure
5.) Defining make script for the individual components of the project



Running the program
----------------------------------
Running the program requires a shared object file be made from the Cython wrapper of the C++ classes. The command I used for this on my virtual machine was: CC=gcc python setup.py build_ext --inplace
The build will start from scratch if one of the files wrapped in individual.pyx is changed. If changes are made to auxiliary files, delete individual.cpp and rerun the build command.
Once a succesful build is achieved you can use the produced shared object file (which will be called pushRunner.so) in your python files by adding an "import individualRunner" statement. The individualRunner import will make the PyPushIndividual object available for use. It can then be treated as a normal python object. For details on its available methods and definitions, check the individual.pyx file.

C++ Code
----------------------------------

PushIndividual.h
=================
The C++ PushIndividual class, wrapped by Cython, serves as the primary point of contact between a user and the Push codebase. Each individual has a reference to its own environment object (Env.h) and its code (Code.h). The environment object is responsible for handling the stacks (including input values). The code variable stores the instructions that define how a push individual will act. Code for an individual can be updated through the setProgram function or via the constructor. Creating a code variable leverages the parse (CodeUtils) which takes a string and creates a Code object from it. If an instruction listed in the string passed to the parse function is not found then the program will fail to be parsed and that failed instruction will appear on the name stack.

Data.h
=======
Holds the input data. Replicates the stacks from env but only pushes input values onto them. Requires a string to be passed to its constructor in order to parse the data from the "input data".
