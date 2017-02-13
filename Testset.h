//
//  TokenizerTests.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef TokenizerTests_h
#define TokenizerTests_h

#include <string>
#include <vector>

void testset1() {
    //make sure editor is initialized
    initEditor();
    
    std::vector<Operator> operators = {
        {10,false,"!",Prefix},
        {10,false,"not",Prefix},
        {11,true, "^", Infix},
        {12,true,"v", Infix},
        {13,true,"->", Infix},
        {13,true,"implies", Infix},
        {14,true,"<->", Infix},
        {15,true,"*", Infix},
        {15,true,"/", Infix},
        {16.2,true,"+", Infix},
        {16.2,true,"-", Infix},
        {std::numeric_limits<double>::max(),false,":=",Infix},
        {std::numeric_limits<double>::max(),false,":<=>",Infix},
    };
    
    std::vector<Parenthesis> parans = {
        {"(",")",true}, //true means its a function paranthesis, like hello(x)
        {"(",")",false}, //false means its just a paranthesis.
        {"[","]",true}
    };
    
    std::string code;
    //code="[a + (e + b * / c - g / d * e)]vd";tokenizer(code, operators, parans);
    code="3 := sin[3] * a + !!!!(4) :<=> 213 :<=> 123";
    AST result = tokenizeAndParse(code, operators, parans);
    PrintAST(result);
    
    std::string repr;
    result.debugPrint(repr);
    std::cout << repr << std::endl;
}


void testset2() {
    std::vector<Operator> operators = {
        {0,false,"!",Prefix},
        {0,false,"~",Prefix},
        {1,true,"^",Infix},
        {1,true,"&",Infix},
        {1,true,"v",Infix},
        {1,true,"|",Infix},
        {2,true,"->",Infix},
        {3,false,"forall",Prefix},
        {3,false,"exists",Prefix}
    
    };
}

#endif /* TokenizerTests_h */
