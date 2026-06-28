#include "Treino.hpp"
#include <iostream>

// construtor
Treino::Treino(std::string focoMuscular, std::list<std::string> exercicios, int duracaoEstimadaMin){
    // 1º regras de negócio: a lista de exercícios não pode estar vazia
    if (exercicios.empty())
    {
        throw AcademiaException("O treino deve ter pelo menos um exercício");
    }
    
    // 2º regra de negócio: a duração deve ser maior que 0 minutos
    if (duracaoEstimadaMin <= 0)
    {
        throw AcademiaException("O treino deve ter duração maior que 0 minutos");
    }
    
    // inicialização
    this -> focoMuscular = focoMuscular;
    this -> exercicios = exercicios;
    this -> duracaoEstimadaMin = duracaoEstimadaMin;
}


// getters
std::string Treino::getFoco(){
    return this -> focoMuscular;
};

std::list<std::string> Treino::getExercicios(){
    return this -> exercicios;
}

int Treino::getDuracao(){
    return this -> duracaoEstimadaMin;
}