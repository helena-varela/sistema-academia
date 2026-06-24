#include "AcademiaException.hpp"

AcademiaException::AcademiaException(std::string mensagemErro)
{
    this->mensagemErro = mensagemErro;
}

const char* AcademiaException::what() const noexcept
{
    return mensagemErro.c_str();
}