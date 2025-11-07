#include <iostream>
#include "arguments.hpp"

void Arguments::showArguments(int argc, char* argv[]){
    for(int idx =0; idx < argc; idx++){
        std::cout<< argv[idx] << std::endl;
    }
}

bool Arguments::recieveArgument(int argc, char* argv[]){
    return reciveLoadBmpPath(argc, argv);
}

bool Arguments::reciveLoadBmpPath(int argc, char* argv[]){
    for(int idx =0; idx < argc; idx++){
        std::string argString = argv[idx];
        if((argString == "--load" || argString == "--l") && (idx +1 < argc)){
            std::cout<<"Exist Load Flags." << std::endl;
            std::filesystem::path filePath(argv[idx+1]);
            if(std::filesystem::exists(filePath)){
                m_loadBmpPath = filePath.generic_string();
                std::cout<<"Exist file: " << filePath.generic_string() << std::endl;
                return true;
            }
        }
    }
    return false;
}