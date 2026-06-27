#include "AcademiaException.hpp"

AcademiaException::AcademiaException(const std::string& msg)
 : mensagemErro(msg) {}

const char* AcademiaException::what() const noexcept
{
    return mensagemErro.c_str();
}