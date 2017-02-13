//
//  Tokenizer.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef Tokenizer_h
#define Tokenizer_h

#include <algorithm>
#include <queue>
#include <set>
#include <stack>

#include <iostream>


bool checkMatch(std::string & code, std::string search, std::size_t pos) {
    //Make sure the string to be searched actually fits into the code at position pos.
    if(pos + search.size() - 1 >= code.size()) return false;
    
    //Check whether any of the characters do not match.
    for(std::size_t i = 0; i < search.size(); ++i) {
        if(search[i] != code[i + pos]) return false;
    }
    
    //If all characters match, return true.
    return true;
}

//This method gets called before an operator token, left paranthesis or right paranthesis is inserted into the code and pushes the entire string
void pushStringToken(std::set<Token> & allTokens, std::string & currentString, std::size_t currentStringStartIndex, std::size_t nestDepth) {
    if(currentString.size() > 0) {
        Token stringToken(currentString, currentStringStartIndex, nestDepth);
        allTokens.insert(stringToken);
        currentString = "";
    }
}





AST tokenizeAndParse(std::string & code, std::vector<Operator> operators, std::vector<Parenthesis> parens) {
    sanityCheckOperators(operators, parens);
    sort(operators.begin(), operators.end());
    sort(parens.begin(), parens.end());
    
    std::set<Token> allTokens;
    
    //These are all operator tokens. Since priority queues sort from greatest precedence to smallest precedence,
    //we have to reverse the operator with std::greater.
    std::priority_queue<OperatorToken, std::vector<OperatorToken>, std::greater<OperatorToken> > operatorTokens;
    
    
    //While the tokenizer will go over the code, it will at any time store all unclosed parens (regardless of it being a function or not) in order in the stack.
    //nestDepth = currentParens.size()
    std::stack<Parenthesis*> currentParens;
    
    //In case no operators match the character will be added towards this string. As soon as something matches, the current string will get pushed.
    //These could be globalized for neater code.
    std::string cs = ""; std::size_t cssi = 0; //cs = currentString, csi = currentStringStartIndex
    
    for(std::size_t pos = 0; pos < code.size(); ++pos) {
        //1. Check for the simple seperator ,
        if(checkMatch(code, ",", pos)) {
            pushStringToken(allTokens, cs, cssi, currentParens.size());
            //Simply reduce the nestedness by one, this makes later handling easier with the comma seperator.
            Token commaToken(currentParens.top()->RParen, pos, currentParens.size()-1);
            allTokens.insert(commaToken);
            goto nextCharacter;
        }
        
        //2. Check whether it is a closed parenthesis. If so check whether it matches with the corresponding open parenthesis.
        for(int i = 0; i < parens.size(); ++i) {
            //Matched a closed paranthesis!
            if(currentParens.size() != 0 && checkMatch(code, parens[i].RParen, pos)) {
                //Check whether the closed paranthesis is the expected paranthesis:
                if(currentParens.top()->LParen == parens[i].LParen && currentParens.top()->RParen == parens[i].RParen && currentParens.top()->function == parens[i].function) {
                    error.resolveError();
                    pushStringToken(allTokens,cs, cssi, currentParens.size());
                    //Only add it to the tokens if it belongs to a function, so the function knows where to stop.
                    //!TODO remove these lines:
                    if(currentParens.top()->function) {
                        Token endFunctionParanthesisToken(currentParens.top()->RParen, pos, currentParens.size()-1);
                        allTokens.insert(endFunctionParanthesisToken);
                    }
                    
                    //Move the position according to the size of the right parenthesis.
                    pos += currentParens.top()->RParen.size()-1;
                    currentParens.pop();
                    goto nextCharacter;
                }
                else { //This can be legal, if both a parenthesis exists with function and without.
                    error << "Expected a closed bracket of type: " << currentParens.top()->RParen << ", but received " << parens[i].RParen;
                }
            }
        }
        error.promptError();
        
        //3. Check whether a new open paranthesis matches.
        for(int i = 0; i < parens.size(); ++i) {
            if(checkMatch(code, parens[i].LParen, pos)) {
                if(!parens[i].function && cs.size() > 0) {
                    error.resolveError();
                    error << "No operator in front of paranthesis " << parens[i].LParen << " at position " << pos << "\n";
                }
                else if(parens[i].function && cs.size() == 0) {
                    //This can happen legally, but only if a parens exist with the same name without the function attribute.
                    error << "Expected string in front of " << parens[i].LParen << " at position " << pos << "\n";
                }
                else {
                    error.resolveError();
                    currentParens.push(&parens[i]);
                    if(parens[i].function) {
                        Operator functionOperator (-std::numeric_limits<double>::max(), true, cs, Function);
                        OperatorToken functionOperatorToken(functionOperator.representation, pos-1, currentParens.size()-1, &functionOperator);
                        operatorTokens.push(functionOperatorToken);
                        allTokens.insert(functionOperatorToken);
                        Token lparenToken (parens[i].LParen, pos, currentParens.size()-1);
                        allTokens.insert(lparenToken);
                        cs = "";
                    }
                    //Move the position according to the size of the left parenthesis.
                    pos += parens[i].LParen.size()-1;
                    
                    goto nextCharacter;
                }
            }
        }
        //Flush the errors.
        error.promptError();
        
        //4. Check whether an operator matches
        for(int i = 0; i < operators.size(); ++i) {
            if(checkMatch(code, operators[i].representation, pos)) {
                pushStringToken(allTokens, cs, cssi, currentParens.size());
                OperatorToken newOperatorToken(operators[i].representation, pos, currentParens.size(), &operators[i]);
                operatorTokens.push(newOperatorToken);
                allTokens.insert(newOperatorToken);
                
                //Move the position according to the size of the operator.
                pos += operators[i].representation.size()-1;
                goto nextCharacter;
            }
        }
        
        //5. Completely ignore whitespaces, unless they are operator overloaded!
        if(isspace(code[pos])) {
            //pushStringToken(allTokens, cs, cssi, currentParens.size());
            goto nextCharacter;
        }
        
        //6. When it's neither an operator, a left paranthesis, a right paranthesis, a comma or a whitespace
        if(cs.size() == 0) cssi = pos;
        cs.push_back(code[pos]);
        
        ;nextCharacter:;
        
    }
    
    //7. Make sure the last string is pushed
    pushStringToken(allTokens, cs, cssi, currentParens.size());

    
    //For future debugging purposes
    //for(const Token & t : allTokens) std::cout << "\"" << t.representation << "," <<t.nestDepth << "\" ";
    //std::cout << std::endl;
    
    //while(!operatorTokens.empty()) {
    //    std::cout << "\"" << operatorTokens.top().representation << "," << operatorTokens.top().nestDepth << "\" ";
    //    operatorTokens.pop();
    //}
    //std::cout << std::endl;
    
    AST result = parser(allTokens, operatorTokens, parens);
    return result;
}

#endif /* Tokenizer_h */
