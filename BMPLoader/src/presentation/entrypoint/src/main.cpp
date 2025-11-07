
#include <iostream>
#include "../include/arguments.hpp"

int main(int argc, char* argv[]){
    Arguments args;
    args.showArguments(argc, argv);
    args.recieveArgument(argc, argv);
}

