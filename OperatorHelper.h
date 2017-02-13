//
//  OperatorHelper.h
//  AggregateProver
//
//  Created by kdkdk on 13.02.17.
//  Copyright © 2017 a. All rights reserved.
//

//This helper method creates an operator in the following way.

#ifndef OperatorHelper_h
#define OperatorHelper_h


//This method copies everything from production rule and creates a vector of operators which can be adjoined to the tokenizer input vector and it will work out.

//Sample usage:
//Ternary operator: operatorHelper("*?*:*", 12,true);
//Forall operator in predicate logic: operatorHelper("forall**", 12, true);
//Forall operator with alternate syntax: operatorHelper("forall*.*",12,true);
std::vector<Operator> operatorHelper(std::string productionRule, double precedence, bool leftAssociative) {
    //Not yet UTF-8 safe, due to char symbol checks!
    
    //1. Find the first symbol, which is not a literal *
    std::string firstSymbol = "";
    bool finishedWritten = false;
    std::size_t positionOfLastSymbol = 0;
    for(std::size_t i = 0; i < productionRule.size(); ++i) {
        if(finishedWritten == false && productionRule[i] != '*') {
            positionOfLastSymbol = i;
            firstSymbol.push_back( productionRule[i] );
        }
        else if(firstSymbol.size() > 0 && productionRule[i] == '*') finishedWritten = true;
    }
    //Make sure that it is a legal operator Rule.
    assert(firstSymbol.size() > 0);
    
    //(Screw morse code idea)
    
    std::vector<Operator> output;
    Operator sentinelOperator (precedence, leftAssociative, firstSymbol, productionRule);
    output.push_back(sentinelOperator);
    
    //Now add all the other non-star symbols.
    std::string currentStr = "";
    for(int i = positionOfLastSymbol + 1; i < productionRule.size(); ++i) {
        if(productionRule[i] == '*' && currentStr.size() > 0) {
            Operator specialHelperOperator(precedence, leftAssociative, currentStr, SpecialHelper);
            output.push_back(specialHelperOperator);
            currentStr = "";
        } else if(productionRule[i] != '*') {
            currentStr.push_back( productionRule[i] );
        }
    }
    if(currentStr.size() > 0) {
        Operator specialHelperOperator(precedence, leftAssociative, currentStr, SpecialHelper);
        output.push_back(specialHelperOperator);
    }
    
    return output;
}


#endif /* OperatorHelper_h */
