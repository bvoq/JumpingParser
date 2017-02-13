//
//  Tokens.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef Tokens_h
#define Tokens_h

#include <cassert>

struct Token {
    std::string representation;
    
    std::size_t position; //Start position in the code of the token.
    std::size_t nestDepth; //How deep the current token is nested inside paranthesis.
    
    Token(std::string _representation, std::size_t _position, std::size_t _nestDepth)
    : representation(_representation), position(_position), nestDepth(_nestDepth) {}
};

bool operator<(const Token & lhs, const Token & rhs) {
    //Tokens are only sorted by position. Since no two tokens share the same position, the following comparison suffices:
    return lhs.position < rhs.position;
}


struct OperatorToken : Token {
    Operator* op;
    OperatorToken(std::string _representation, std::size_t _position, std::size_t _nestDepth, Operator* _op)
    : Token(_representation, _position, _nestDepth), op(_op) {}
};

bool operator<(const OperatorToken & lhs, const OperatorToken & rhs) {
    //1. Sort by nestedness.
    if(lhs.nestDepth != rhs.nestDepth) return lhs.nestDepth > rhs.nestDepth;
    //2. Sort by operator precedence
    if(lhs.op->precedence != rhs.op->precedence) return lhs.op->precedence < rhs.op->precedence;
    //Make sure they share the same precedence, when they already share the same precedence
    assert(lhs.op->leftAssociativity == rhs.op->leftAssociativity);
    //3. Sort by position according to associativity
    assert(lhs.position != rhs.position);
    if(lhs.op->leftAssociativity) return lhs.position < rhs.position;
    else return lhs.position > rhs.position;
}

bool operator>(const OperatorToken & lhs, const OperatorToken & rhs) {
    return rhs < lhs;
}

#endif /* Tokens_h */
