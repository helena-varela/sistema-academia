#ifndef ACADEMIAEXCEPTION_HPP
#define ACADEMIAEXCEPTION_HPP

#include <exception>
#include <string>

class AcademiaException : public std::exception
{
    private:
        std::string mensagemErro;
    public:
        explicit AcademiaException(const std::string& mensagemErro);
        const char* what() const noexcept override;
};

#endif