/**
 * @file arguments.cpp
 * @brief Arguments の実装。
 */
#include <iostream>
#include "arguments.hpp"

void Arguments::showArguments(int argc, char* argv[]){
    for(int idx =0; idx < argc; idx++){
        std::cout<< argv[idx] << std::endl;
    }
}

bool Arguments::recieveArgument(int argc, char* argv[]){
    bool result = reciveLoadBmpPath(argc, argv);
    if(!result){
        std::cout<<"No Load BMP Path Specified." << std::endl;
    }
    result = reciveSaveBmpPath(argc, argv) || result;
    if(!result){
        std::cout<<"No Save BMP Path Specified." << std::endl;
    }
    return result;
}

bool Arguments::reciveLoadBmpPath(int argc, char* argv[]){
    for(int idx =0; idx < argc; idx++){
        std::string argString = argv[idx];
        if((argString == "--load" || argString == "--l") && (idx +1 < argc)){
            std::cout<<"Exist Load Flags." << std::endl;
            std::filesystem::path filePath(argv[idx+1]);
            if(std::filesystem::exists(filePath)){
                loadBmpPath_ = filePath.generic_string();
                std::cout<<"Exist file: " << filePath.generic_string() << std::endl;
                return true;
            }
        }
    }
    return false;
}
bool Arguments::reciveSaveBmpPath(int argc, char* argv[]){
    for(int idx =0; idx < argc; idx++){
        std::string argString = argv[idx];
        if((argString == "--save" || argString == "--s") && (idx +1 < argc)){
            std::cout<<"Exist Load Flags." << std::endl;
            std::filesystem::path filePath(argv[idx+1]);
            if(!std::filesystem::exists(filePath) && (!loadBmpPath_.empty() && !loadBmpPath_._Equal(filePath.generic_string()))){
                saveBmpPath_ = filePath.generic_string();
                std::cout<<"Exist file: " << filePath.generic_string() << std::endl;
                return true;
            }
        }
    }
    return false;
}
