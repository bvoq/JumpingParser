//
//  Operator.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//



// Checked Operator rules:
// - It is not allowed to have an operator with the same representation AND operatorType.
// - Operators with identical representation are prioritized in the following order: Infix, Prefix, Postfix
// - It is not allowed to have an operator with the same name as a left paranthesis or right paranthesis.
// - Function-capable paranthesis are prioritized over non-function capable paranthesis.
// - Specific white-spaces can be overriden to be operators.
// - If not specified white-spaces are completely ignored. This can be dangerous if you have an operator called "v" and a string such as "hellovworld", hence it will result in "hello" "v" "world". This can be circumvented by good coding practice or by adding " v " with spaces as an operator. Another dangerous area might be the space in between string literals, "hello world" gets read as "helloworld". You will get a warning when this happens though! TODO: You will get a warning if a string has the same name as an operator without white-spaces.
// Alternative Idea: Non operator white-spaces are seperators between operators and non-operators.

#ifndef Operator_h
#define Operator_h

#include <cassert>
#include <climits>
#include <string>
#include <vector>

//if % 2 == 0 infix, % 3 == 0 prefix, % 5 == 0 postfix.
enum OperatorType {
    Unknown=0,
    Infix=2, Prefix=3, Postfix=5, Nullary=7,Special=11,SpecialHelper=13,Function=17,
    InfixPrefix=2*3, InfixPostfix=2*5, PrefixPostfix=3*5,
    InfixPrefixPostfix=2*3*5
};

struct Operator {
    //Operator information:
    double precedence;
    bool leftAssociativity;
    std::string representation; //Their symbolic representation, like + - * ...
    OperatorType operatorType;
    
    std::string fullOperator;
    
    Operator(double _precedence, bool _leftAssociativity, std::string _representation, OperatorType _operatorType)
    : precedence(_precedence), leftAssociativity(_leftAssociativity), representation(_representation), operatorType(_operatorType) {}
    Operator(double _precedence, bool _leftAssociativity, std::string _representation, std::string _fullOperator)
    : precedence(_precedence), leftAssociativity(_leftAssociativity), representation(_representation), fullOperator(_fullOperator) {
        operatorType = Special;
    }

    
    //Token information:
    std::size_t position; //Start position of token in source code.
    unsigned long long nestedness; //Depth of nesting
};

bool operator<(const Operator & lhs, const Operator & rhs) {
    //This is only used to get the right order of operators.
    //1. Sort operators by size.
    if(lhs.representation.size() != rhs.representation.size()) return lhs.representation.size() > rhs.representation.size();
    //2. Sort by actual operator name
    if(lhs.representation != rhs.representation) return lhs.representation < rhs.representation;
    //It is forbidden to have 2 operators with the same representation and type.
    assert(lhs.operatorType == rhs.operatorType);
    return false; //Equal
}


struct Parenthesis {
    std::string LParen;
    std::string RParen;
    bool function; //true if used for functions, false if not.
    Parenthesis(std::string _LParen, std::string _RParen, bool _function)
    : LParen(_LParen), RParen(_RParen), function(_function) {
    }
};

bool operator<(const Parenthesis & lhs, const Parenthesis & rhs) {
    //1. Sort by whether it is used as a function, such as *[*,*,*] or as brackets () [].
    if(lhs.function != rhs.function) return lhs.function;
    //2. Sort by LParen
    if(lhs.LParen != rhs.LParen) return lhs.LParen < rhs.LParen;
    //3. Sort by RParen
    return lhs.RParen < rhs.RParen;
}

//This checks whether there are operators with the same name as parens,
//and whether there are operators which have the same name and type.
void sanityCheckOperators(std::vector<Operator> & operators, std::vector<Parenthesis> & parens) {
    for(int i = 0; i < operators.size(); ++i) {
        assert(operators[i].representation != ",");
        for(int j = 0; j < parens.size(); ++j) {
            assert(operators[i].representation != parens[j].LParen);
            assert(operators[i].representation != parens[j].RParen);
        }
        
        for(int j = i + 1; j < operators.size(); ++j) {
            //Checks whether operators[i] is of the same type as operator[j].
            if(typeid(operators[i]).hash_code() == typeid(operators[j]).hash_code()) {
                assert(operators[i].representation != operators[j].representation);
            }
        }
    }
}


//  *[*] function
//  (*) paranthesis

// stoppers:
//on level 6: + -
//on level 5: * /
//on level INF: *[*]



//representant:
// + -
// [ (

#endif /* Operator_h */
