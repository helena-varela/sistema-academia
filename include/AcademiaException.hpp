#ifndef ACADEMIAEXCEPTION_HPP
#define ACADEMIAEXCEPTION_HPP

#include <string>

class AcademiaException
{
    private:
        std::string mensagemErro;
    public:
        AcademiaException(std::string mensagemErro);
        const char what();
};

#endif