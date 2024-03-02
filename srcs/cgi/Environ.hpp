#ifndef Environ_HPP
# define Environ_HPP

#include <cstdlib>
#include <iostream>
#include "Cgi.hpp"
// extern char **environ;

class Environ {
    private:
        std::string path;
    public:
        std::string getPath();
        void addEnvVar(std::string var, std::string var2);
        std::string checkAccess(std::string uri);
};

#endif