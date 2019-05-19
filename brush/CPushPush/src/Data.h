#ifndef __DATA_H__
#define __DATA_H__

#include <fstream>
#include <vector>
#include <valarray>
#include <set>
#include <iostream>

// #include "CodeUtils.h"
// #include "RNG.h"
// #include "GeneticOps.h"
#include "TypeDef.h"
#include "InputInstruction.h"
// #include "Literal.h"
#include "Code.h"

class Data {

private:
	// Keep track of what "input" is currently being considered
	int curSample = 0;

	// List of inputs
	std::vector<std::string> inputSequence; // Used to determine what input1, input2 correspond to

	/* Stacks */
    std::vector<std::vector<push::Exec>>   exec_stack;
    std::vector<std::vector<int>>          int_stack;
    std::vector<std::vector<push::Code>>   code_stack;
    std::vector<std::vector<bool>>         bool_stack;
    std::vector<std::vector<double>>       double_stack;
    std::vector<std::vector<push::name_t>> name_stack;
	std::vector<std::vector<std::string>>  string_stack;

public:
	Data() {
		std::vector<push::Exec> newExec;
		std::vector<int> newInt;
		std::vector<push::Code> newCode;
		std::vector<bool> newBool;
		std::vector<double> newDouble;
		std::vector<push::name_t> newName;
		std::vector<std::string> newString;

		this->exec_stack = std::vector<std::vector<push::Exec>>(1, newExec);
		this->int_stack = std::vector<std::vector<int>>(1, newInt);
		this->code_stack = std::vector<std::vector<push::Code>>(1, newCode);
		this->bool_stack = std::vector<std::vector<bool>>(1, newBool);
		this->double_stack = std::vector<std::vector<double>>(1, newDouble);
		this->name_stack = std::vector<std::vector<push::name_t>>(1, newName);
		this->string_stack = std::vector<std::vector<std::string>>(1, newString);
		this->inputSequence = std::vector<std::string>();
	}
	Data(int* startSeq, int numRows, int numCols, std::vector<std::string> inputSequence) {	// TODO ask if should base solely off of inputSequence
		this->curSample = 0;
		if (inputSequence.size() == numCols) {
			this->inputSequence = inputSequence;
			int* curPtr = startSeq;

			// Create the base containers for ALL inputs
			std::vector<std::vector<push::Exec>> newExecs;
			this->exec_stack = newExecs;
			std::vector<std::vector<int>> newInts;
			this->int_stack = newInts;
			std::vector<std::vector<push::Code>> newCodes;
			this->code_stack = newCodes;
			std::vector<std::vector<bool>> newBools;
			this->bool_stack = newBools;
			std::vector<std::vector<double>> newDoubles;
			this->double_stack = newDoubles;
			std::vector<std::vector<push::name_t>> newNames;
			this->name_stack = newNames;
			std::vector<std::vector<std::string>> newStrings;
			this->string_stack = newStrings;

			std::cout << "Parsing for " << numRows << " rows and " << numCols << " cols.";

			for (int i = 0; i < numRows; i++) {
				// On each iteration need to create new stacks to hold the input for that sample
				std::vector<push::Exec> newExec;
				std::vector<int> newInt;
				std::vector<push::Code> newCode;
				std::vector<bool> newBool;
				std::vector<double> newDouble;
				std::vector<push::name_t> newName;
				std::vector<std::string> newString;

				for (int j = 0; j < numCols; j++) {
					// Decode each individual input for the sample
					if (inputSequence[j] == "Int") {
						int nextVal = *curPtr;
						curPtr++;
						newInt.push_back(nextVal);
					} else if (inputSequence[j] == "Exec") {
						push::Exec* nextVal = reinterpret_cast<push::Exec*>(curPtr);
						curPtr += sizeof(push::Exec);
						newExec.push_back(*nextVal);
					} else if (inputSequence[j] == "Code") {
						push::Code* nextVal = reinterpret_cast<push::Code*>(curPtr);
						curPtr += sizeof(push::Code);
						newCode.push_back(*nextVal);
					} else if (inputSequence[j] == "Bool") {
						bool nextVal = (bool) *(curPtr);
						curPtr += sizeof(bool);
						newBool.push_back(nextVal);
					} else if (inputSequence[j] == "Double") {
						double nextVal = (double) *(curPtr);
						curPtr += sizeof(double);
						newDouble.push_back(nextVal);
					} else if (inputSequence[j] == "Name") {
						push::name_t* nextVal = reinterpret_cast<push::name_t*>(curPtr);
						curPtr += sizeof(push::name_t);
						newName.push_back(*nextVal);
					} else if (inputSequence[j] == "String") {
						std::string* nextVal = reinterpret_cast<std::string*>(curPtr);
						curPtr += sizeof(std::string);
						newString.push_back(*nextVal);
					}
				}
				this->exec_stack.push_back(newExec);
				this->int_stack.push_back(newInt);
				this->code_stack.push_back(newCode);
				this->bool_stack.push_back(newBool);
				this->double_stack.push_back(newDouble);
				this->name_stack.push_back(newName);
				this->string_stack.push_back(newString);
			}
			makeInputInstructions();
		} else {
			// Were passed data and sequence that don't make sense. Can't properly parse.
			std::vector<push::Exec> newExec;
			std::vector<int> newInt;
			std::vector<push::Code> newCode;
			std::vector<bool> newBool;
			std::vector<double> newDouble;
			std::vector<push::name_t> newName;
			std::vector<std::string> newString;

			this->exec_stack = std::vector<std::vector<push::Exec>>(1, newExec);
			this->int_stack = std::vector<std::vector<int>>(1, newInt);
			this->code_stack = std::vector<std::vector<push::Code>>(1, newCode);
			this->bool_stack = std::vector<std::vector<bool>>(1, newBool);
			this->double_stack = std::vector<std::vector<double>>(1, newDouble);
			this->name_stack = std::vector<std::vector<push::name_t>>(1, newName);
			this->string_stack = std::vector<std::vector<std::string>>(1, newString);
			this->inputSequence = std::vector<std::string>(1, "None");
		}
	}

	std::vector<std::vector<push::Exec>> getExec() {
		return this->exec_stack;
	}

	std::vector<push::Exec>& getCurExec() {
		return this->exec_stack.at(curSample);
	}


	std::vector<std::vector<int>> getInt() {
		return this->int_stack;
	}

	std::vector<int>& getCurInt() {
		return this->int_stack.at(curSample);
	}

	std::vector<std::vector<push::Code>> getCode() {
		return this->code_stack;
	}

	std::vector<push::Code>& getCurCode() {
		return this->code_stack.at(curSample);
	}


	std::vector<std::vector<bool>> getBool() {
		return this->bool_stack;
	}
	std::vector<bool>& getCurBool() {
		return this->bool_stack.at(curSample);
	}

	std::vector<std::vector<double>> getDouble() {
		return this->double_stack;
	}

	std::vector<double>& getCurDouble() {
		return this->double_stack.at(curSample);
	}

	std::vector<std::vector<push::name_t>> getName() {
		return this->name_stack;
	}

	std::vector<push::name_t>& getCurName() {
		return this->name_stack.at(curSample);
	}

	std::vector<std::vector<std::string>> getStringInputs() {
		return this->string_stack;
	}

	std::vector<std::string>& getCurStringInputs() {
		return this->string_stack.at(curSample);
	}

	std::string getPrintString();

	// TODO figure out how to get function with generic return type
	void makeInputInstructions();

	int next_sample();
};
#endif