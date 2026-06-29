#ifndef TREINO_HPP
#define TREINO_HPP

#include <string>
#include <list>
#include <fstream>
#include "AcademiaException.hpp"

class Treino
{
    private:
        std::string focoMuscular;
        std::list<std::string> exercicios;
        int duracaoEstimadaMin;
    public:
        Treino(std::string focoMuscular, std::list<std::string> exercicios, int duracaoEstimadaMin);
        std::string getFoco() const;
        std::list<std::string> getExercicios() const;
        int getDuracao() const;
        friend std::ostream& operator<<(std::ostream& os, const Treino& treino);
};

#endif