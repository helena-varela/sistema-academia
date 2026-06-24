#ifndef ACADEMIAEXCEPTION_HPP
#define ACADEMIAEXCEPTION_HPP

#include <string>
#include <exception>

class AcademiaException : public std::exception
{
    private:
        std::string mensagemErro;
    public:
        AcademiaException(std::string mensagemErro);
        const char* what() const noexcept override;
};

#endif