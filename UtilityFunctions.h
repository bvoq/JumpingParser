//
//  UtilityFunctions.h
//  AggregateProver
//
//  Created by kdkdk on 11.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef UtilityFunctions_h
#define UtilityFunctions_h

#include <cmath>

#include <string>
#include "wchar.h"

//Side function for PrintAST in order to figure out the amount of spaces. Returns the full width it uses.
int printPostOrderUTF8(const AST & root, std::vector<std::wstring> & result, int depth, std::size_t pos) {
    
    //Make sure position is ok.
    while(depth >= result.size()) result.push_back(L"");
    for(std::size_t i = result[depth].size(); i < pos; ++i) result[depth] += L" ";
    assert(result[depth].size() == pos);
    
    //Compute the children and their sizes
    std::size_t childSpace = 0;
    for(std::size_t i = 0; i < root.children.size(); ++i) {
        std::size_t childWidth = printPostOrderUTF8(root.children[i], result, depth + 2, pos + childSpace);
        
        //slight correction
        if(childWidth % 2 == 0) childWidth--;
        //Add the spaces or lines until the child is reached.
        std::size_t childPos = pos + childSpace + childWidth/2;
        if(i == 0) {
            for(std::size_t i = result[depth+1].size(); i < childPos; ++i) result[depth+1] += L" ";
        } else {
            for(std::size_t i = result[depth+1].size(); i < childPos; ++i) result[depth+1] += L"─";
        }
        
        //Add specific downward slope
        if(root.children.size() == 1) result[depth+1] += L"│";
        else if(i == 0) result[depth+1] += L"╭";
        else if(i+1 == root.children.size()) result[depth+1] += L"╮";
        else result[depth+1] += L"┬";
        
        childSpace += childWidth;
        if(i+1 != root.children.size()) childSpace += 3; //this should be an odd number
    }
    
    std::size_t parentPosition = 0;
    //Add additional space to the parent node, according to the size of the children.
    if(childSpace >= root.expression.size()) {
        parentPosition = pos + (childSpace - root.expression.size()) / 2;
        for(std::size_t i = pos; i < parentPosition; ++i) result[depth].push_back(' ');
    }
    //In case the width of the parent is larger than the width of the children, add additional space to the children.
    else if(root.children.size() > 0) {
        //insert space into the children at position pos.

        std::size_t additionalSpace = (root.expression.size() - childSpace) / 2;
        
        for(int i = depth + 1; i < result.size(); ++i) {
            if(pos < result[i].size()) {
                std::wstring before = result[i].substr(0,pos);
                for(std::size_t i = 0; i < additionalSpace; ++i) before.push_back(' ');
                before += result[i].substr(pos);
                result[i] = before;
            }
        }
    }
    
    result[depth] += utf8::toWideString(root.expression);
    
    //Last but not least add uplink towards the chain.
    if(root.children.size() > 0) {
        //Set parentPostition from front position to middle position
        parentPosition = parentPosition + root.expression.size()/2;
        if     (result[depth+1].substr(parentPosition,1) == L"─") result[depth+1].replace(parentPosition,1,L"┴");
        else if(result[depth+1].substr(parentPosition,1) == L"┬") result[depth+1].replace(parentPosition,1,L"┼");
        else if(result[depth+1].substr(parentPosition,1) == L"╭") result[depth+1].replace(parentPosition,1,L"├");
        else if(result[depth+1].substr(parentPosition,1) == L"╮") result[depth+1].replace(parentPosition,1,L"┤");
    }
    if(childSpace < root.expression.size()) return root.expression.size();
    else return childSpace;
}



int printPostOrderASCII(const AST & root, std::vector<std::string> & result, int depth, std::size_t pos) {
    //Make sure position is ok.
    while(depth >= result.size()) result.push_back("");
    for(std::size_t i = result[depth].size(); i < pos; ++i) result[depth].push_back(' ');
    assert(result[depth].size() == pos);
    
    //Compute the children and their sizes
    std::size_t childSpace = 0;
    for(std::size_t i = 0; i < root.children.size(); ++i) {
        std::size_t childWidth = printPostOrderASCII(root.children[i], result, depth + 2, pos + childSpace);
        
        //slight correction since odd space width:
        if(childWidth % 2 == 0) childWidth--;
        //Add the spaces or lines until the child is reached.
        std::size_t childPos = pos + childSpace + childWidth/2;
        if(i == 0) {
            for(std::size_t i = result[depth+1].size(); i < childPos; ++i) result[depth+1].push_back(' ');
        } else {
            for(std::size_t i = result[depth+1].size(); i < childPos; ++i) result[depth+1] += "-";
        }
        
        //Add specific downward slope
        if(root.children.size() == 1) result[depth+1] += "|";
        else if(i == 0) result[depth+1] += "+";
        else if(i+1 == root.children.size()) result[depth+1] += "+";
        else result[depth+1] += "+";
        
        childSpace += childWidth;
        if(i+1 != root.children.size()) childSpace += 3;
    }
    
    std::size_t parentPosition = 0;
    //Add additional space to the parent node, according to the size of the children.
    if(childSpace >= root.expression.size()) {
        parentPosition = pos + (childSpace - root.expression.size()) / 2;
        for(std::size_t i = pos; i < parentPosition; ++i) result[depth].push_back(' ');
    }
    //In case the width of the parent is larger than the width of the children, add additional space to the children.
    else if(root.children.size() > 0) {
        //insert space into the children at position pos.
        
        std::size_t additionalSpace = (root.expression.size() - childSpace) / 2;
        
        for(int i = depth + 1; i < result.size(); ++i) {
            if(pos < result[i].size()) {
                std::string before = result[i].substr(0,pos);
                for(std::size_t i = 0; i < additionalSpace; ++i) before.push_back(' ');
                before += result[i].substr(pos);
                result[i] = before;
            }
        }
    }
    result[depth] += root.expression;
    
    //Last but not least add uplink towards the chain.
    if(root.children.size() > 0) {
        //Set parentPostition from front position to middle position
        parentPosition = parentPosition + root.expression.size()/2;
        if     (result[depth+1].substr(parentPosition,1) == "-") result[depth+1].replace(parentPosition,1,"+");
    }
    if(childSpace < root.expression.size()) return root.expression.size();
    else return childSpace;
}


void PrintASTASCII(const AST & root) {
    std::vector<std::string> representation;
    
    printPostOrderASCII(root,  representation, 0, 0);
    for(std::size_t i = 0; i < representation.size(); ++i) {
        std::cout << representation[i] << std::endl;
    }
}


void PrintAST(const AST & root) {
    std::vector<std::wstring> representation;

    printPostOrderUTF8(root, representation, 0, 0);
    for(std::size_t i = 0; i < representation.size(); ++i) {
        std::wcout << representation[i] << std::endl;
    }
}




#endif /* UtilityFunctions_h */
