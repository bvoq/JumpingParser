//
//  Utf8.h
//  AggregateProver
//
//  Created by kdkdk on 12.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef Utf8_h
#define Utf8_h
#include <iostream>

namespace utf8 {
    //Modified code snippet from: http://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
    std::wstring toWideString(std::string str) {
        std::wstring str2(str.size(), L' '); // Make room for characters
            
        // Copy string to wstring.
        std::copy(str.begin(), str.end(), str2.begin());
        
        return str2;
    }
}

//UNUSED:
/*
 struct utf8 {
 std::string str = "";
 std::wstring wstr = L"";
 
 utf8 () {}
 utf8 (std::string _str) : str(_str) {}
 utf8 (std::string & _str) : str(_str) {}
 
 //Code snippet from: http://stackoverflow.com/questions/4063146/getting-the-actual-length-of-a-utf-8-encoded-stdstring
 std::size_t size() {
 std::size_t len = 0;
 std::string::iterator it = str.begin();
 while (*it) len += (*it++ & 0xc0) != 0x80;
 return len;
 }
 
 
 //Modificated snippet from: http://stackoverflow.com/questions/30995246/substring-of-a-stdstring-in-utf-8-c11
 utf8 substr(unsigned int start, std::size_t leng)
 {
 if (leng==0) { return utf8(""); }
 std::size_t c, i, ix, q, min = std::string::npos, max = std::string::npos;
 for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
 {
 if (q==start){ min=i; }
 if (q <= start+leng || leng == std::string::npos) max=i;
 
 c = (unsigned char) str[i];
 if      (
 //c>=0   &&
 c<=127) i+=0;
 else if ((c & 0xE0) == 0xC0) i+=1;
 else if ((c & 0xF0) == 0xE0) i+=2;
 else if ((c & 0xF8) == 0xF0) i+=3;
 //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
 //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
 else return utf8("");//invalid utf8
 }
 if (q <= start+leng || leng == std::string::npos) max = i;
 if (min == std::string::npos || max == std::string::npos)  return utf8("");
 return str.substr(min,max);
 }
 
 
 
 //Modified code snippet from: http://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
 std::wstring toWideString() {
 if(wstr.size() == 0) {
 wstr = std::wstring(size(), L' '); // Make room for characters
 
 // Copy string to wstring.
 std::copy(str.begin(), str.end(), wstr.begin());
 }
 return wstr;
 }
 };
 */
#endif /* Utf8_h */
