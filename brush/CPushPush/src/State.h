#ifndef __STATE_H__
#define __STATE_H__

#include <fstream>
#include <vector>
#include <valarray>
#include <set>

#include "CodeUtils.h"
#include "RNG.h"
#include "GeneticOps.h"
#include "Instruction.h"
#include "Literal.h"
#include "Code.h"

class State {

private:
	/* Stacks */
    std::vector<push::Exec>	      exec_stack;
    std::vector<int>          int_stack;
    std::vector<push::Code>         code_stack;
    std::vector<bool>         bool_stack;
    std::vector<double>       double_stack;
    std::vector<push::name_t>	      name_stack;
	std::vector<std::string>  string_stack;

public:
	State() {
		this->exec_stack = std::vector<push::Exec>();
		this->int_stack = std::vector<int>();
		this->code_stack = std::vector<push::Code>();
		this->bool_stack = std::vector<bool>();
		this->double_stack = std::vector<double>();
		this->name_stack = std::vector<push::name_t>();
		this->string_stack = std::vector<std::string>();
	}
	State(std::vector<push::Exec> e_stack, std::vector<int> i_stack, std::vector<push::Code> c_stack, std::vector<bool> b_stack, std::vector<double> d_stack, std::vector<push::name_t> n_stack, std::vector<std::string> s_stack) {
		this->exec_stack = e_stack;
		this->int_stack = i_stack;
		this->code_stack = c_stack;
		this->bool_stack = b_stack;
		this->double_stack = d_stack;
		this->name_stack = n_stack;
		this->string_stack = s_stack;
	}

	std::vector<push::Exec> getExec() {
		return this->exec_stack;
	}

	push::Exec popExec() {
		push::Exec toReturn = this->exec_stack.back();
		this->exec_stack.pop_back();
		return toReturn;
	}

	std::vector<int> getInt() {
		return this->int_stack;
	}

	int popInt() {
		int toReturn = this->int_stack.back();
		this->int_stack.pop_back();
		return toReturn;
	}

	std::vector<push::Code> getCode() {
		return this->code_stack;
	}

	push::Code popCode() {
		push::Code toReturn = this->code_stack.back();
		this->code_stack.pop_back();
		return toReturn;
	}

	std::vector<bool> getBool() {
		return this->bool_stack;
	}

	bool popBool() {
		bool toReturn = this->bool_stack.back();
		this->bool_stack.pop_back();
		return toReturn;
	}

	std::vector<double> getDouble() {
		return this->double_stack;
	}

	double popDouble() {
		double toReturn = this->double_stack.back();
		this->double_stack.pop_back();
		return toReturn;
	}

	std::vector<push::name_t> getName() {
		return this->name_stack;
	}

	push::name_t popName() {
		push::name_t toReturn = this->name_stack.back();
		this->name_stack.pop_back();
		return toReturn;
	}

	std::vector<std::string> getString() {
		return this->string_stack;
	}

	std::string popString() {
		std::string toReturn = this->string_stack.back();
		this->string_stack.pop_back();
		return  toReturn;
	}

	std::string getPrintString() {
		std::ostringstream os;
	    os.setf(std::ios_base::showpoint);
	    
	    os << "(\n";
	    
		    os << "\tInteger(";
		    for (unsigned i = 0; i < this->int_stack.size(); ++i)
		        os << this->int_stack[i] << ' ';
		    os << ")\n";

		    os << "\tCode(";
		    for (unsigned i = 0; i < this->code_stack.size(); ++i)
		        os << this->code_stack[i] << ' ';
		    os << ")\n";
		    
		    os << "\tBool(";
		    for (unsigned i = 0; i < this->bool_stack.size(); ++i)
		        os << (this->bool_stack[i]? "TRUE ":"FALSE ");
		    os << ")\n";
		    
		    os << "\tDouble(";
		    for (unsigned i = 0; i < this->double_stack.size(); ++i)
		        os << this->double_stack[i] << ' ';
		    os << ")\n";

		    os << "\tName(";
		    for (unsigned i = 0; i < this->name_stack.size(); ++i)
		        os << push::print(this->name_stack[i]) << ' ';
		    os << ")\n";

			os << "\tString(";
			for (unsigned i = 0; i < this->string_stack.size(); ++i)
				os << this->string_stack[i] << ' ';
			os << ")\n";

		os << ")\n";
    	return os.str();
	}
};
#endif