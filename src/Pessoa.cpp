#include "Pessoa.hpp"

#include <algorithm>
#include <cctype>

Pessoa::Pessoa(const std::string& nome,
               const std::string& cpf,
               const std::string& email)
    : nome(nome),
      cpf(cpf),
      email(email)
{
    if (nome.empty())
    {
        throw DadosInvalidosException("nome", "Nome nao pode ser vazio.");
    }

    if (email.empty())
    {
        throw DadosInvalidosException("email", "Email nao pode ser vazio.");
    }

    if (!validarCPF(cpf))
    {
        throw DadosInvalidosException("cpf", "CPF invalido (deve ter 11 digitos).");
    }
}

Pessoa::~Pessoa()
{
}

std::string Pessoa::getNome() const
{
    return nome;
}

std::string Pessoa::getCpf() const
{
    return cpf;
}

std::string Pessoa::getEmail() const
{
    return email;
}

bool Pessoa::ValidarEntrada() const
{
    return !nome.empty() &&
           !email.empty() &&
           validarCPF(cpf);
}

bool Pessoa::validarCPF(const std::string& cpf)
{
    if (cpf.length() != 11)
        return false;

    return std::all_of(cpf.begin(), cpf.end(), ::isdigit);
}

void Pessoa::definirSenha(const std::string& senha)
{
    if (senha.size() < 4)
    {
        throw DadosInvalidosException("senha", "A senha deve ter pelo menos 4 caracteres.");
    }
    senhaHash = gerarHash(senha);
}

void Pessoa::alterarSenha(const std::string& senhaAtual, const std::string& novaSenha)
{
    if (!autenticar(senhaAtual))
    {
        throw AutenticacaoException("usuario", "Senha atual incorreta.");
    }
    definirSenha(novaSenha);
}

bool Pessoa::autenticar(const std::string& senha) const
{
    return !senhaHash.empty() && senhaHash == gerarHash(senha);
}

std::string Pessoa::getSenhaHash() const
{
    return senhaHash;
}

void Pessoa::restaurarSenhaHash(const std::string& hash)
{
    senhaHash = hash;
}

// Hash FNV-1a de 64 bits: deterministico, para nao salvar a senha em texto puro no arquivo
std::string Pessoa::gerarHash(const std::string& texto)
{
    unsigned long long hash = 1469598103934665603ULL;
    for (unsigned char c : texto)
    {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    const char* hex = "0123456789abcdef";
    std::string resultado(16, '0');
    for (int i = 15; i >= 0; --i)
    {
        resultado[i] = hex[hash & 0xF];
        hash >>= 4;
    }
    return resultado;
}
