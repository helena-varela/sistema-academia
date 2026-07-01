#include "Treino.hpp"
#include <iostream>

// construtor
Treino::Treino(std::string focoMuscular, const std::list<std::string>& exercicios, int duracaoEstimadaMin){
    // 1º regras de negócio: a lista de exercícios não pode estar vazia
    if (exercicios.empty())
    {
        throw AcademiaException("O treino deve ter pelo menos um exercicio");
    }
    
    // 2º regra de negócio: a duração deve ser maior que 0 minutos
    if (duracaoEstimadaMin <= 0)
    {
        throw AcademiaException("O treino deve ter duracao maior que 0 minutos");
    }
    
    // inicialização
    this -> focoMuscular = focoMuscular;
    this -> exercicios = exercicios;
    this -> duracaoEstimadaMin = duracaoEstimadaMin;
}


// getters
std::string Treino::getFoco() const{
    return this -> focoMuscular;
};

std::list<std::string> Treino::getExercicios() const{
    return this -> exercicios;
}

int Treino::getDuracao() const{
    return this -> duracaoEstimadaMin;
}

std::ostream& operator<<(std::ostream& os, const Treino& treino)
{
    os << "Foco: " << treino.getFoco() << '\n';
    os << "Exercicios:\n";

    for (const std::string& exercicio : treino.getExercicios())
    {
        os << "- " << exercicio << '\n';
    }

    return os;
}