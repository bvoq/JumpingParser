//
//  JumpingParser.h
//  AggregateProver
//
//  Created by Kevin De Keyser on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef JumpingParser_h
#define JumpingParser_h
struct AST;
void PrintAST(const AST &);

#include <iostream>
#include <locale>

void initEditor() {
    //Configure UTF-8
    //std::ios_base::sync_with_stdio(false);
    
    std::cout.imbue( std::locale("en_US.UTF-8") );
    std::wcout.imbue(std::locale("en_US.UTF-8")); // A concrete UTF-8 standard has to be set in order for wcout to work!
    setlocale(LC_ALL, "en_US.UTF-8");
    
}

#include "Utf8.h"

#include "Error.h"
#include "Operators.h"
#include "Tokens.h"
#include "Parser.h"
#include "Tokenizer.h"

#include "UtilityFunctions.h"

#include "Testset.h"



#endif /* JumpingParser_h */
