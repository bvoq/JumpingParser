//
//  Parser.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef Parser_h
#define Parser_h

#include <queue>
#include <set>
#include <vector>


struct AST {
    std::string expression;
    std::vector<AST> children;
    std::size_t position, nestDepth;
    
    //In case it is an operator
    OperatorType operatorType;
    //In case it is a function (type of endbracket, needed for syntactic sugar).
    std::string lParen = "", rParen = ""; //used for printing out nicely
    
    //In the future maybe add static types here, but for now they are all dynamically casted.
    
    //Copy constructor
    AST(const AST & fromOther) : expression(fromOther.expression), children(fromOther.children), position(fromOther.position), nestDepth(fromOther.nestDepth), operatorType(fromOther.operatorType), lParen(fromOther.lParen), rParen(fromOther.rParen) {}
    
    AST(std::string _expression, std::vector<AST> _children, std::size_t _position, std::size_t _nestDepth)
    : expression(_expression), children(_children), position(_position), nestDepth(_nestDepth) {}
    AST(std::string _expression, std::vector<AST> _children, std::size_t _position, std::size_t _nestDepth, OperatorType _operatorType)
    : expression(_expression), children(_children), position(_position), nestDepth(_nestDepth), operatorType(_operatorType) {}
    AST(std::string _expression, std::vector<AST> _children, std::size_t _position, std::size_t _nestDepth, std::string _lParen, std::string _rParen)
    : expression(_expression), children(_children), position(_position), nestDepth(_nestDepth), lParen(_lParen), rParen(_rParen) {}
    
    
    
    void debugPrint(std::string & str) const {
        if(children.size() == 0) str += expression;
        //Operator
        else if(rParen.size() == 0) {
            if(operatorType == Infix) {
                str += "(";
                children[0].debugPrint(str);
                str += " " + expression + " ";
                children[1].debugPrint(str);
                str += ")";
            } else if(operatorType == Prefix) {
                str += expression + "(";
                children[0].debugPrint(str);
                str += ")";
            } else if(operatorType == Postfix) {
                str += "(";
                children[0].debugPrint(str);
                str += ")" + expression;
            }
            else {
                error << "Somehow OPTYPE " << operatorType << " doesn't get printed.\n";
                error.promptError();
            }
            
        } else { //Function
            str += expression + lParen;
            for(int i = 0; i < children.size(); ++i) {
                children[i].debugPrint(str);
                if(i+1 != children.size()) str += ", ";
            }
            str += rParen;
        }
    }
};

bool operator<(const AST & lhs, const AST & rhs) {
    return lhs.position < rhs.position;
}

bool endParenthesis(std::string toCheck, std::vector<Parenthesis> & parens) {
    for(int i = 0; i < parens.size(); ++i) {
        if(toCheck == parens[i].RParen) return true;
    }
    return false;
}

AST parser(std::set<Token> & allTokens, std::priority_queue<OperatorToken, std::vector<OperatorToken>, std::greater<OperatorToken> > & operatorTokens, std::vector<Parenthesis> parens) {
    
    //1. Transform all operatorTokens into AST without children.
    std::set<AST> abstractTree;
    for(const Token & current : allTokens) {
        AST temp(current.representation, {}, current.position, current.nestDepth);
        abstractTree.insert(temp);
    }
    
    
    while(!operatorTokens.empty()) {
        OperatorToken current = operatorTokens.top();
        operatorTokens.pop();
        std::set<AST>::iterator currentPos = abstractTree.find( AST("", {}, current.position, 0) );
        AST replacement(*currentPos);
        //Remove the current operator from the expressions and move the iterator one forward
        currentPos = abstractTree.erase(currentPos);
        
        
        bool successful = false;
        //Functions
        if(current.op->operatorType % 7 == 0) {
            //Read in the left parenthesis
            replacement.lParen = currentPos->expression;
            currentPos = abstractTree.erase(currentPos);
            do {
                if(currentPos->expression != ",") {
                    AST newChild(*currentPos);
                    replacement.children.push_back( newChild );
                }
                //erase the next element
                currentPos = abstractTree.erase(currentPos);
            } while(currentPos->nestDepth != current.nestDepth || !endParenthesis(currentPos->expression, parens));
            //Store type of closing parenthesis for beautiful printing
            replacement.rParen = currentPos->expression;
            
            //Remove closing parenthesis
            abstractTree.erase(currentPos);
            successful = true;
        }
        //Infix operator
        if(current.op->operatorType % 2 == 0 && !successful) {
            
            if(currentPos == abstractTree.end()) error << "Expected an argument after the operator " << replacement.expression << ", but received end of line.\n";
            else if(currentPos == abstractTree.begin()) error << "Expected an argument before the operator " << replacement.expression << ", but received beginning of line instead.\n";
            else {
                if(currentPos->nestDepth < replacement.nestDepth) error << "Expected an argument after the operator " << replacement.expression << ", but instead received " << currentPos->expression << ".\n";
                else if((--currentPos)->nestDepth < replacement.nestDepth) {
                    error << "Expected an argument before the operator " << replacement.expression << ", but instead received " << currentPos->expression << ".\n";
                    ++currentPos;
                }
                else {
                    AST leftChild(*currentPos);
                    currentPos = abstractTree.erase(currentPos);
                    
                    AST rightChild(*currentPos);
                    currentPos = abstractTree.erase(currentPos);
                    replacement.children.push_back( leftChild );
                    replacement.children.push_back( rightChild );
                    replacement.operatorType = Infix;
                    successful = true;
                }
            }
            //AST rightChild (currentPos->expression, {}, currentPos->position, currentPos->nestDepth);
        }
        //Prefix operator
        if(current.op->operatorType % 3 == 0 && !successful) {
            if(currentPos == abstractTree.end()) error << "Expected an argument after the operator " << replacement.expression << ", but received end of line instead.\n";
            else if(currentPos->nestDepth < replacement.nestDepth) error << "Expected an argument after the operator " << replacement.expression << ", but instead received " << currentPos->expression << ".\n";
            else {
                AST rightChild(*currentPos);
                currentPos = abstractTree.erase(currentPos);
                replacement.children.push_back( rightChild );
                replacement.operatorType = Prefix;
                successful = true;
            }
        }
        //Postfix operator
        if(current.op->operatorType % 5 == 0 && !successful) {
            if(currentPos == abstractTree.begin()) error << "Expected an argument before the operator " << replacement.expression << ", but received beginning of line instead.\n";
            else if((--currentPos)->nestDepth < replacement.nestDepth) {
                error << "Expected an argument before the operator " << replacement.expression << ", but instead received " << currentPos->expression << ".\n";
                ++currentPos;
            }
            else {
                AST leftChild(*currentPos);
                currentPos = abstractTree.erase(currentPos);
                replacement.children.push_back( leftChild );
                replacement.operatorType = Postfix;
                successful = true;
            }
        }
        if(successful) {
            error.resolveError();
            abstractTree.insert(replacement);
        }
        error.promptError();
    }
    
    if(abstractTree.size() != 1) {
        error << "The syntax tree is not connected together.\n";
        error << "Printing out all seperate syntax trees, which are missing a connection to each other:\n";
        
        for(const AST & node : abstractTree) {
            std::string representation;
            node.debugPrint(representation);
            //PrintAST(node);
            error << representation << "\n";
        }
        
        error.promptError();
    }
    /*
     for(const AST & node : abstractTree) {
     node.debugPrint();
     std::cout << std::endl;
     }
     */
    return *abstractTree.begin();
}

#endif /* Parser_h */
