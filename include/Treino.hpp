#ifndef TREINO_HPP
#define TREINO_HPP

#include <string>
#include <list>
#include "AcademiaException.hpp"

class Treino
{
    private:
        std::string focoMuscular;
        std::list<std::string> exercicios;
        int duracaoEstimadaMin;
    public:
        Treino(std::string focoMuscular, std::list<std::string> exercicios, int duracaoEstimadaMin);
        std::string getFoco();
        std::list<std::string> getExercicios();
        int getDuracao();
};

#endif