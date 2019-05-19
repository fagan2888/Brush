#ifndef _INPUT_INSTRUCTIONS_H__
#define _INPUT_INSTRUCTIONS_H__


// #include "Env.h"
#include "StaticInit.h"
#include "TypedAtom.h"
#include "TypeDef.h"

// namespace push {
//     template <class GENERIC>
//     class InputInstruction : public TypedAtom {
//         int index;
//         std::string name;
//         std::string type;

//         public:
        
//         const Type& get_precondition() const { return nullType; }
//         const Type& get_postcondition() const { return get_type<GENERIC>(); }
        
//         InputInstruction(int i, std::string name) : index(i), name(name) {}

//         unsigned operator()(Env& env) const { 
//             if (type == "Int") {
//                 std::vector<int>& inputStack = get_input_stack<int>(env);
//                 if (inputStack.size() > index) {
//                     push<int>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "Exec") {
//                 std::vector<push::Exec>& inputStack = get_input_stack<push::Exec>(env);
//                 if (inputStack.size() > index) {
//                     push<push::Exec>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "Code") {
//                 std::vector<push::Code>& inputStack = get_input_stack<push::Code>(env);
//                 if (inputStack.size() > index) {
//                     push<push::Code>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "Bool") {
//                 std::vector<bool>& inputStack = get_input_stack<bool>(env);
//                 if (inputStack.size() > index) {
//                     push<bool>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "Float") {
//                 std::vector<float>& inputStack = get_input_stack<float>(env);
//                 if (inputStack.size() > index) {
//                     push<float>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "Name") {
//                 std::vector<push::name_t>& inputStack = get_input_stack<push::name_t>(env);
//                 if (inputStack.size() > index) {
//                     push<push::name_t>(env, inputStack.at(index)); 
//                 }
//             } else if (type == "String") {
//                 std::vector<std::string>& inputStack = get_input_stack<std::string>(env);
//                 if (inputStack.size() > index) {
//                     push<std::string>(env, inputStack.at(index)); 
//                 }
//             }
//             return 1; 
//         }

//         std::string to_string() const {
//             return typeid(GENERIC).name();
//         }
         
//         bool equal_to(const CodeBase& other) const { 
//             return this->index == static_cast<const InputInstruction<GENERIC>& >(other).index;
//             // return does_equal(this->index, static_cast<const InputInstruction<T>& >(other).index);
//         }
//     };

//     template <typename T> inline
//     Code make_input_instruction(int index, std::string name) {
//         return static_initializer.register_pushfunc(push::Code(new push::InputInstruction<T>(index, name))); 
//     }
// }


namespace push {
    class Env;

    template <class GENERIC>
    class InputInstruction : public TypedAtom 
    {
        int index;
        std::string name;
        std::string type;

        public:
        
        const Type& get_precondition() const { return nullType; }
        const Type& get_postcondition() const { return get_type<GENERIC>(); }
        
        InputInstruction(int i, std::string name) : index(i), name(name) {}

        unsigned operator()(Env& env) const;
        //  const { 
        //     // std::vector<GENERIC>& inputStack = get_input_stack<GENERIC>(env);
        //     // if (inputStack.size() > index) {
        //     //     push<GENERIC>(env, inputStack.at(index)); 
        //     // }
        //     return 1; 
        // }

        std::string to_string() const {
            return typeid(GENERIC).name();
        }
         
        bool equal_to(const CodeBase& other) const { 
            return this->index == static_cast<const InputInstruction<GENERIC>& >(other).index;
            // return does_equal(this->index, static_cast<const InputInstruction<T>& >(other).index);
        }
    };

    Code make_new_input_instruction(int index, std::string name, std::string type);

    // template <> inline
    // Code make_input_instruction<int>(int index, std::string name);
    //  {
    //     return static_initializer.register_pushfunc(push::Code(new push::InputInstruction<T>(index, name))); 
    // }
}

#endif