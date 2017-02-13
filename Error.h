//
//  Error.h
//  AggregateProver
//
//  Created by kdkdk on 10.02.17.
//  Copyright © 2017 a. All rights reserved.
//

#ifndef Error_h
#define Error_h

#include <cassert>
#include <iostream>
#include <string>
#include <vector>



struct Error {
    std::vector<std::string> errorMessages;
    void addArrToError(const std::vector<std::string> error) {
        for(int i = 0; i < error.size(); ++i)
            errorMessages.push_back(error[i]);
    }
    
    void addStrToError(const std::string error) {
        if(errorMessages.size() == 0) errorMessages = {error};
        else errorMessages.back() += error;
    }
    
    void resolveError() {
        errorMessages.clear();
    }
    
    void promptError() {
        if(errorMessages.size() == 0) return;
        for(int i = 0; i < errorMessages.size(); ++i) {
            std::cerr << errorMessages[i] << std::endl;
        }
        exit(1); //An error is not supposed to happen.
    }

} error;


//Utility methods for nice error handling:
Error& operator<<(Error& os, const std::string & i)
{
    os.addStrToError(i);
    return os;
}

//The other types get implicitly casted to this, in case it overshoots long long we'll be doomed!


Error& operator<<(Error& os, const short & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const unsigned short & i) {
    os.addStrToError(std::to_string(i));
    return os;
}

Error& operator<<(Error& os, const char & i) {
    std::string newStr; //Make sure the char conversion works as intended.
    newStr.push_back(i);
    os.addStrToError(newStr);
    return os;
}

Error& operator<<(Error& os, const unsigned char & i) {
    os.addStrToError(std::to_string(i));
    return os;
}

Error& operator<<(Error& os, const int & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const unsigned int & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const long & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const unsigned long & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const long long & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const unsigned long long & i) {
    os.addStrToError(std::to_string(i));
    return os;
}
Error& operator<<(Error& os, const double & i) {
    os.addStrToError(std::to_string(i));
    return os;
}

Error& operator<<(Error& os, const std::vector<std::string> & i) {
    os.addArrToError(i);
    return os;
}




#endif /* Error_h */
