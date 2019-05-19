/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _STRING_OPS_H__
#define _STRING_OPS_H__

#include <string>

#include "Env.h"
#include "Literal.h"
// #include "NumOps.h"

// At the base level stacks are implemented as vectors (so any operation on a stack is an operation on a vector)
    // Is just a vector holding a particular type

// Wonder what the return of the functions is supposed to represent (maybe number of new elements?)

namespace push {

    inline unsigned _split_at_space(Env& env) {
        // Split the string at all spaces, and each new string to stack
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<int>& intStack = get_stack<int>(env);
		if (stringStack.size() > 1) {
			std::string s = pop<std::string>(env);
			std::string delimiter = " ";

			// std::string word = "";
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
			    token = s.substr(0, pos);
			    push<std::string>(env, token);
			    s.erase(0, pos + delimiter.length());
			}

			// std::string word = "";
			// for (char x : bodyString)
			// {
			// 	if (x == ' ')
			// 	{
			// 		stringStack.push_back(word);
			// 		word = "";
			// 	}
			// 	else
			// 	{
			// 		word = word + x;
			// 	}
			// }
			// push<std::string>(env, word);
			// stringStack.push_back(word);
		}

		return 1;
    }

	inline unsigned _split_at_string(Env& env) {
		// Split string at specified string, all elements added
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		if (stringStack.size() > 1) {
			std::string s = pop<std::string>(env);
			std::string delimiter = pop<std::string>(env);

			// std::string word = "";
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
			    token = s.substr(0, pos);
			    push<std::string>(env, token);
			    s.erase(0, pos + delimiter.length());
			}
			// for (char x : bodyString)
			// {
			// 	if (x == splitter)
			// 	{
			// 		push<std::string>(env, word);
			// 		word = "";
			// 	}
			// 	else
			// 	{
			// 		word = word + x;
			// 	}
			// }
			// push<std::string>(env, word);
			// stringStack.push_back(word);
		}

		return 1;
	}

	inline unsigned _length(Env& env) {
		// Get size of string
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<int>& intStack = get_stack<int>(env);
		if (stringStack.size() > 1) {
			std::string bodyString = pop<std::string>(env);
			push<int>(env, bodyString.size());
		}

		return 1;
	}

	inline unsigned _reverse(Env& env) {
		// last letter now becomes first and so on, 
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		if (stringStack.size() > 1) {
			std::string bodyString = pop<std::string>(env);
			std::reverse(bodyString.begin(), bodyString.end());
			push<std::string>(env, bodyString);
		}

		return 1;
	}

	inline unsigned _char_at(Env& env) {
		// Return the specified character
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<int>& intStack = get_stack<int>(env);
		if (stringStack.size() > 1 && intStack.size() > 0) {
			std::string bodyString = pop<std::string>(env);
			int pos = pop<int>(env);
			if (bodyString.size() < pos) {
				push<std::string>(env, "" + bodyString.at(pos));
			} else {
				push<std::string>(env, bodyString);
				push<int>(env, pos);
			}
			
		}

		return 1;
	}

	inline unsigned _empty(Env& env) {
		// Bool of whether 2nd string contains first
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<bool>& boolStack = get_stack<bool>(env);
		if (stringStack.size() > 1) {
			std::string bodyString = pop<std::string>(env);
			push<bool>(env, bodyString.empty());
		}

		return 1;
	}

	inline unsigned _contains(Env& env) {
		// Bool of whether 2nd string contains first
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<bool>& boolStack = get_stack<bool>(env);
		if (stringStack.size() > 1) {
			std::string bodyString = pop<std::string>(env);
			std::string searchTerm = pop<std::string>(env);
			push<bool>(env, bodyString.find(searchTerm) == -1 ? false : true);
		}

		return 1;
	}

	inline unsigned _replace_substring(Env& env) {
		// Replace subset with new character
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<int>& intStack = get_stack<int>(env);
		if (stringStack.size() > 1 && intStack.size() > 3) {
			std::string frontString = pop<std::string>(env);
			std::string backString = pop<std::string>(env);

			int pos = pop<int>(env);
			int len = pop<int>(env);
			int subpos = pop<int>(env);
			int sublen = pop<int>(env);
			push<std::string>(env, frontString.replace(pos, len, backString, subpos, sublen));
		}

		return 1;
	}

	inline unsigned _replace(Env& env) {
		// Replace subset with new character
		std::vector<std::string>& stringStack = get_stack<std::string>(env);
		std::vector<int>& intStack = get_stack<int>(env);
		if (stringStack.size() > 1 && intStack.size() > 1) {
			std::string frontString = pop<std::string>(env);
			std::string backString = pop<std::string>(env);
			int pos = pop<int>(env);
			int len = pop<int>(env);
			push<std::string>(env, frontString.replace(pos, len, backString));
		}
		
		return 1;
	}

	inline unsigned _append_string(Env& env) {
		// Append character to end of string
		std::vector<std::string>& stack = get_stack<std::string>(env);
		if (stack.size() > 1) {
			std::string frontString = pop<std::string>(env);
			std::string backString = pop<std::string>(env);
			push<std::string>(env, frontString + backString);
		}

		return 1;
	}

	inline unsigned _first(Env& env) {
		// Gets first character from string
		std::vector<std::string>& stack = get_stack<std::string>(env);
		if (stack.size() > 0) {
			std::string toProcess = pop<std::string>(env);
			std::string frontChar = "" + toProcess.front();
			push<std::string>(env, frontChar);
		}
		return 1;
	}

	inline unsigned _last(Env& env) {
		// Get the last character from string
		std::vector<std::string>& stack = get_stack<std::string>(env);
		if (stack.size() > 0) {
			std::string toProcess = pop<std::string>(env);
			push<std::string>(env, "" + toProcess.back());
		}
		return 1;
	}

	inline unsigned _pop_back(Env& env) {
		std::vector<std::string>& stack = get_stack<std::string>(env);
		if (stack.size() > 0) {
			std::string toProcess = pop<std::string>(env);
		}
		return 1;
	}
    /*inline unsigned _split_at_char(Env& env) {
        // Split string at a specifiec character
        // TODO 

        if (1 + first<Code>(env)->size() + second<Code>(env)->size() >= env.parameters.max_points_in_program) return 1;
        
        String a = pop<String>(env);
        Char b = pop<Char>(env);
        push(env, list(b,a));
        return 1;
    }*/

    /* From */
    // template <class From, std::string>
    // inline unsigned push_cast(Env& env)  {
    //     //if (not has_elements<From>(env,1)) return 1;
    //     push<std::string>(env, std::to_string(pop<From>(env)));
        
    //     return 1;
    // }

    // template <class T>
    // inline unsigned _from_T(Env& env) { 
    //     T a = pop<T>(env);
    //     push<Code>(env, Code(new Literal<T>(a)));
    //     return 1;
    // }

	inline unsigned _int2String(Env& env) {
		int toConvert = pop<int>(env);
		push<std::string>(env, std::to_string(toConvert));
		return 1;
		
	}

	inline unsigned _float2String(Env& env) {
		float toConvert = pop<float>(env);
		push<std::string>(env, std::to_string(toConvert));
		return 1;
	}

	inline unsigned _bool2String(Env& env) {
		bool toConvert = pop<bool>(env);
		push<std::string>(env, toConvert ? "true" : "false");
		return 1;
	}

	

    /*inline unsigned _append_char(Env& env) {
        // Append character to end of string
        // TODO

        if (pop<Code>(env)->get_stack().size()) {
            push<bool>(env, false);
        } else {
            push<bool>(env, true);
        }

        return 1;
    }*/

    /*inline unsigned _nth(Env& env) {
        // Get the nth character from string
        // TODO (How different from char at?)

        Code first = pop<Code>(env);
        Code second = pop<Code>(env);

        push(env, contains(first, second));
        return first->size() * second->size();
    }*/

    /*inline unsigned _replace_char(Env& env) {
        // Replace characters in string with something else
        // TODO

        Code code = pop<Code>(env);
        int val = pop<int>(env);
        
        if (code->size() == 0) { // nil
            push(env, code);
            return 1;
        }

        val = std::abs(val) % code->size();

        push(env, extract(code, val));
        return code->size(); // Complexity is O(n) 
    }*/

    /*inline unsigned _replace_first_char(Env& env) {
        // Replace first character with something else
        // TODO

        Code one    = first<Code>(env);
        Code other  = second<Code>(env);
        int val     = first<int>(env);
        return 1;
    }*/

    /*inline unsigned _member(Env& env) {
        // Remove all of a particular character
        // TODO

        Code first = pop<Code>(env);
        Code second = pop<Code>(env);

        push(env, member(first, second));

        return 1;
    }*/

    // inline unsigned _noop(Env& env) { return 1; }

    /* large parts of the stuff below should go to a source file */

    unsigned _nth(Env& env); 
    unsigned _nthcdr(Env& env);
    unsigned _null(Env& env);
    unsigned _position(Env& env);

} // namespace push

#endif
