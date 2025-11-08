/**
 * @file main.cpp
 * @brief エントリポイント。引数の表示と受け取りを行います。
 */
#include <iostream>
#include "../include/arguments.hpp"

/**
 * @brief アプリケーションのエントリポイント。
 */
int main(int argc, char* argv[]){
    Arguments args;
    args.showArguments(argc, argv);
    args.recieveArgument(argc, argv);
}

