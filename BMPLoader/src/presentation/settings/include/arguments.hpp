#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <filesystem>

class Arguments {
public:
    static void showArguments(int argc, char* argv[]);
    bool recieveArgument(int argc, char* argv[]);
    const std::string getLoadBmpPath()const {return m_loadBmpPath;};
private:
    std::string m_loadBmpPath;

    bool reciveLoadBmpPath(int argc, char* argv[]);
};

#endif