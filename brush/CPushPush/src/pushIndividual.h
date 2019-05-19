#ifndef _PUSH_INDIVIDUAL__H__
#define _PUSH_INDIVIDUAL__H__
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
#include "StaticInit.h"
#include "State.h"
#include "Data.h"
// #include "../Cython/Test.h"

namespace Brush {
    class PushIndividual {
        bool operator<(const PushIndividual& other) const {
            // if (fitness > other.fitness)
            //     return true;
            return false;
        }

        private:
            push::Code code;
            double fitness;
            unsigned effort;
            std::vector<push::Type> trace;
            push::Env* rootEnv;
            push::Env testEnv;
            std::vector<State> lastState;

            // Returns the fitness of the individual for the specific input (Can't rely on evironment since that won't be handled by Cython?)
            std::vector<int> evaluate(std::vector<std::vector<int>> inputs);    // TODO extend to use non ints

        public:
            PushIndividual(): fitness(-1), effort(0), trace(1,0), lastState() {W
                push::Env env;
                std::cout << "Environment declared!\n";
                env.configure(push::parse("#PUSH CONFIGURATION\n(\n    1.0000000000000000  ENV.MAX-RANDOM-FLOAT\n    -1.0000000000000000 ENV.MIN-RANDOM-FLOAT\n    10  ENV.MAX-RANDOM-INTEGER\n    -10 ENV.MIN-RANDOM-INTEGER\n    1000    ENV.EVALPUSH-LIMIT\n    50  ENV.MAX-POINTS-IN-RANDOM-EXPRESSION\n    150 ENV.MAX-POINTS-IN-PROGRAM\n    # Genetic Programming specific options\n    5000  GP.POPSIZE\n    10   GP.NGENS\n    \n    # Ramplevels: depth of a virtual ramping tree: number of runs is 2^RAMPLEVELS\n    20   GP.RAMPLEVELS\n    #-16257008 ENV.RANDOM-SEED\n    \n    CODE.QUOTE\n    (\n        BOOLEAN.=\n        BOOLEAN.AND\n        BOOLEAN.DEFINE\n        BOOLEAN.DUP\n        BOOLEAN.FLUSH\n        BOOLEAN.FROMFLOAT\n        BOOLEAN.FROMINTEGER\n        BOOLEAN.NAND\n        BOOLEAN.NOR\n        BOOLEAN.NOT\n        BOOLEAN.OR\n        BOOLEAN.POP\n        BOOLEAN.ROT\n        BOOLEAN.SHOVE\n        BOOLEAN.STACKDEPTH\n        BOOLEAN.SWAP\n        BOOLEAN.YANK\n        BOOLEAN.YANKDUP\n    STRING.FIRST\n    STRING.LAST\n    STRING.SPLITATSPACE\n    STRING.SPLITATSTRING\n    STRING.LENGTH\n    STRING.REVERSE\n    STRING.CHARAT\n    STRING.EMPTY\n    STRING.CONTAINS\n    STRING.REPLACESUBSTRING\n    STRING.REPLACE\n    STRING.APPENDSTRING\n    STRING.POP\n    STRING.FROMINTEGER\n    STRING.FROMFLOAT\n    STRING.FROMBOOLEAN\n    STRING.FROMNAME\n    STRING.REMOVECHAR\n    STRING.YANK\n        CODE.=\n        CODE.APPEND\n        CODE.ATOM\n        CODE.CAR\n        CODE.CDR\n        CODE.CONS\n        CODE.CONTAINER\n        CODE.DEFINE\n        CODE.DEFINITION\n        CODE.DO\n        CODE.DO*\n        CODE.DO*COUNT\n        CODE.DO*RANGE\n        CODE.DO*TIMES\n        CODE.DUP\n        CODE.EXTRACT\n        CODE.FLUSH\n        CODE.FROMBOOLEAN\n        CODE.FROMFLOAT\n        CODE.FROMINTEGER\n        CODE.FROMNAME\n        CODE.IF\n        CODE.INSERT\n        CODE.INSTRUCTIONS\n        CODE.LENGTH\n        CODE.LIST\n        CODE.MEMBER\n        CODE.NOOP\n        CODE.NTH\n        CODE.NTHCDR\n        CODE.NULL\n        CODE.POP\n        CODE.POSITION\n        CODE.QUOTE\n        CODE.ROT\n        CODE.SHOVE\n        CODE.SIZE\n        CODE.STACKDEPTH\n        CODE.SWAP\n       CODE.YANK\n        CODE.YANKDUP\n        FLOAT.%\n        FLOAT.*\n        FLOAT.+\n        FLOAT.-\n        FLOAT./\n        FLOAT.<\n        FLOAT.=\n        FLOAT.>\n        FLOAT.COS\n        FLOAT.DEFINE\n        FLOAT.DUP\n        FLOAT.FLUSH\n        FLOAT.FROMBOOLEAN\n        FLOAT.FROMINTEGER\n        FLOAT.MAX\n        FLOAT.MIN\n        FLOAT.POP\n        FLOAT.ROT\n        FLOAT.SHOVE\n        FLOAT.SIN\n        FLOAT.STACKDEPTH\n        FLOAT.SWAP\n        FLOAT.TAN\n        FLOAT.YANK\n        FLOAT.YANKDUP\n        INTEGER.%\n        INTEGER.*\n        INTEGER.+\n        INTEGER.-\n        INTEGER./\n        INTEGER.<\n        INTEGER.=\n        INTEGER.>\n        INTEGER.DEFINE\n        INTEGER.DUP\n        INTEGER.FLUSH\n        INTEGER.FROMBOOL\n        INTEGER.FROMFLOAT\n        INTEGER.MAX\n        INTEGER.MIN\n        INTEGER.POP\n        INTEGER.ROT\n        INTEGER.SHOVE\n        INTEGER.STACKDEPTH\n        INTEGER.SWAP\n        INTEGER.YANK\n        INTEGER.YANKDUP\n        EXEC.DEFINE\n        EXEC.DO*COUNT\n        EXEC.DO*RANGE\n        EXEC.DO*TIMES\n        EXEC.FLUSH\n        EXEC.IF\n        EXEC.K\n        EXEC.POP\n        EXEC.ROT\n        EXEC.S\n        EXEC.SHOVE\n        EXEC.STACKDEPTH\n        EXEC.SWAP\n        EXEC.Y\n        EXEC.YANK\n        EXEC.YANKDUP\n    )\n    ENV.INSTRUCTIONS # read in function set\n) \n#END OF PUSH2 CONFIGURATION"));
                std::cout << "Config parsed\n";
                // env.print_exec_stack(Need to provide an osstream);
                std::cout << "Printing environment before cleared: " << print(env.next()) << "\n";
                env.next().clear_stacks();
                std::cout << "Stacks cleared\n";

                // Add random code to environment
                static push::Code rnd = push::parse("CODE.RAND");
                for (int i = 0; i < 1; i++) {
                    (*rnd)(env); // create random program
                    if (not push::has_elements<push::Code>(env,1)) { --i; continue; }
                    code = push::pop<push::Code>(env); // get random program
                }
                
                // Might be failing since no code in environment yet
                // push::Code codeGen = push::pop<push::Code>(env); // SegFault on this line
                this->rootEnv = &env;//env.next(); // TODO Remove references to this variable
                this->testEnv = env;
                std::cout << "Environment assigned";
                std::cout << "Printing environment after cleared: " << print(env.next()) << "\n";
            }

            PushIndividual(std::string instructionSet): fitness(-1), effort(0), trace(1,0), lastState() {
                push::Env env;
                std::cout << "Environment declared!\n";
                env.configure(push::parse("#PUSH CONFIGURATION\n(\n    1.0000000000000000  ENV.MAX-RANDOM-FLOAT\n    -1.0000000000000000 ENV.MIN-RANDOM-FLOAT\n    10  ENV.MAX-RANDOM-INTEGER\n    -10 ENV.MIN-RANDOM-INTEGER\n    1000    ENV.EVALPUSH-LIMIT\n    50  ENV.MAX-POINTS-IN-RANDOM-EXPRESSION\n    150 ENV.MAX-POINTS-IN-PROGRAM\n    # Genetic Programming specific options\n    5000  GP.POPSIZE\n    10   GP.NGENS\n    \n    # Ramplevels: depth of a virtual ramping tree: number of runs is 2^RAMPLEVELS\n    20   GP.RAMPLEVELS\n    #-16257008 ENV.RANDOM-SEED\n    \n    CODE.QUOTE\n    (\n        BOOLEAN.=\n        BOOLEAN.AND\n        BOOLEAN.DEFINE\n        BOOLEAN.DUP\n        BOOLEAN.FLUSH\n        BOOLEAN.FROMFLOAT\n        BOOLEAN.FROMINTEGER\n        BOOLEAN.NAND\n        BOOLEAN.NOR\n        BOOLEAN.NOT\n        BOOLEAN.OR\n        BOOLEAN.POP\n        BOOLEAN.ROT\n        BOOLEAN.SHOVE\n        BOOLEAN.STACKDEPTH\n        BOOLEAN.SWAP\n        BOOLEAN.YANK\n        BOOLEAN.YANKDUP\n    STRING.FIRST\n    STRING.LAST\n    STRING.SPLITATSPACE\n    STRING.SPLITATSTRING\n    STRING.LENGTH\n    STRING.REVERSE\n    STRING.CHARAT\n    STRING.EMPTY\n    STRING.CONTAINS\n    STRING.REPLACESUBSTRING\n    STRING.REPLACE\n    STRING.APPENDSTRING\n    STRING.POP\n    STRING.FROMINTEGER\n    STRING.FROMFLOAT\n    STRING.FROMBOOLEAN\n    STRING.FROMNAME\n    STRING.REMOVECHAR\n    STRING.YANK\n        CODE.=\n        CODE.APPEND\n        CODE.ATOM\n        CODE.CAR\n        CODE.CDR\n        CODE.CONS\n        CODE.CONTAINER\n        CODE.DEFINE\n        CODE.DEFINITION\n        CODE.DO\n        CODE.DO*\n        CODE.DO*COUNT\n        CODE.DO*RANGE\n        CODE.DO*TIMES\n        CODE.DUP\n        CODE.EXTRACT\n        CODE.FLUSH\n        CODE.FROMBOOLEAN\n        CODE.FROMFLOAT\n        CODE.FROMINTEGER\n        CODE.FROMNAME\n        CODE.IF\n        CODE.INSERT\n        CODE.INSTRUCTIONS\n        CODE.LENGTH\n        CODE.LIST\n        CODE.MEMBER\n        CODE.NOOP\n        CODE.NTH\n        CODE.NTHCDR\n        CODE.NULL\n        CODE.POP\n        CODE.POSITION\n        CODE.QUOTE\n        CODE.ROT\n        CODE.SHOVE\n        CODE.SIZE\n        CODE.STACKDEPTH\n        CODE.SWAP\n       CODE.YANK\n        CODE.YANKDUP\n        FLOAT.%\n        FLOAT.*\n        FLOAT.+\n        FLOAT.-\n        FLOAT./\n        FLOAT.<\n        FLOAT.=\n        FLOAT.>\n        FLOAT.COS\n        FLOAT.DEFINE\n        FLOAT.DUP\n        FLOAT.FLUSH\n        FLOAT.FROMBOOLEAN\n        FLOAT.FROMINTEGER\n        FLOAT.MAX\n        FLOAT.MIN\n        FLOAT.POP\n        FLOAT.ROT\n        FLOAT.SHOVE\n        FLOAT.SIN\n        FLOAT.STACKDEPTH\n        FLOAT.SWAP\n        FLOAT.TAN\n        FLOAT.YANK\n        FLOAT.YANKDUP\n        INTEGER.%\n        INTEGER.*\n        INTEGER.+\n        INTEGER.-\n        INTEGER./\n        INTEGER.<\n        INTEGER.=\n        INTEGER.>\n        INTEGER.DEFINE\n        INTEGER.DUP\n        INTEGER.FLUSH\n        INTEGER.FROMBOOL\n        INTEGER.FROMFLOAT\n        INTEGER.MAX\n        INTEGER.MIN\n        INTEGER.POP\n        INTEGER.ROT\n        INTEGER.SHOVE\n        INTEGER.STACKDEPTH\n        INTEGER.SWAP\n        INTEGER.YANK\n        INTEGER.YANKDUP\n        EXEC.DEFINE\n        EXEC.DO*COUNT\n        EXEC.DO*RANGE\n        EXEC.DO*TIMES\n        EXEC.FLUSH\n        EXEC.IF\n        EXEC.K\n        EXEC.POP\n        EXEC.ROT\n        EXEC.S\n        EXEC.SHOVE\n        EXEC.STACKDEPTH\n        EXEC.SWAP\n        EXEC.Y\n        EXEC.YANK\n        EXEC.YANKDUP\n    )\n    ENV.INSTRUCTIONS # read in function set\n) \n#END OF PUSH2 CONFIGURATION"));
                std::cout << "Config parsed\n";
                // env.print_exec_stack();
                std::cout << "Printing environment before cleared: " << print(env.next()) << "\n";
                env.next().clear_stacks();
                std::cout << "Stacks cleared\n";

                // Populate code 
                std::string code_string = instructionSet;
                this->code = push::parse(code_string);
                std::cout << "Assigned code: " << str(this->code) << "\n";
                
                this->rootEnv = &env;
                this->testEnv = env;
                std::cout << "Environment assigned";
                std::cout << "Printing environment after cleared: " << print(env.next()) << "\n";
            }

            PushIndividual& operator=(const PushIndividual& oth) {
                code = oth.code;
                fitness = oth.fitness;
                effort = oth.effort;
                trace = oth.trace;
                rootEnv = oth.rootEnv;

                delete rootEnv;
                rootEnv = 0;
                if (oth.rootEnv) {
                    rootEnv = oth.rootEnv->clone();
                }
                
                return *this;
            }

            std::vector<int> predict(int* x, int numRows, int numCols);
            std::string getDescription();

            double getFitness() { return fitness; };
            unsigned getEffort() { return effort; };
            int getSize() {};   // TODO Return the size of the program (how many instructions)
            State getState() { return this->lastState.empty() ? State() : this->lastState.back(); };
            std::vector<State> getFullState() { return this->lastState; };
    };
};
#endif