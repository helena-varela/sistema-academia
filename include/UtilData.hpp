#ifndef UTILDATA_HPP
#define UTILDATA_HPP

#include <string>

// Funcoes auxiliares para datas no formato DD/MM/AAAA e meses no formato MM/AAAA.
// Ficam em um namespace (e nao em uma classe) porque sao apenas utilitarios.
namespace UtilData
{
    std::string hoje();                                   // data atual (DD/MM/AAAA)
    std::string mesAtual();                               // mes atual (MM/AAAA)
    std::string mesDe(const std::string& data);           // DD/MM/AAAA -> MM/AAAA
    int diaDe(const std::string& data);                   // DD/MM/AAAA -> DD
    bool dataValida(const std::string& data);
    bool mesValido(const std::string& mes);
    int mesComoNumero(const std::string& mes);            // MM/AAAA -> numero comparavel
    int diasEntre(const std::string& inicio, const std::string& fim); // fim - inicio, em dias
}

#endif
