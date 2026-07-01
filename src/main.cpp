#include <iostream>
#include "AcademiaException.hpp"
#include "GerenciadorAcademia.hpp"
#include "Interface.hpp"
using namespace std;

int main()
{
    GerenciadorAcademia gerenciador;

    bool executando = true;
    int opcao;
    string cpf;
    string cref;

    try
    {
        gerenciador.carregarDeArquivo("data/backup.txt");
    }
    catch (const AcademiaException& e)
    {
        cout << "Aviso: " << e.what() << " Iniciando banco de dados vazio.\n";
    }

    while (executando)
    {
        try
        {
            cout << "=====================================" << endl;
            cout << "        SISTEMA UPESTEEM" << endl;
            cout << "=====================================" << endl;
            cout << "1 - Login Cliente" << endl;
            cout << "2 - Login Instrutor" << endl;
            cout << "3 - Login Gerenciador" << endl;
            cout << "0 - Sair" << endl;
            cout << "=====================================" << endl;
            cout << "Escolha uma opcao: ";
            cin >> opcao;

            switch (opcao)
            {
                case 1:
                    {
                        cout << "[Interface do Cliente]" << endl;
                        cout << "Digite seu CPF: " << endl;
                        cin >> cpf;
                        Cliente* cliente = gerenciador.consultarCliente(cpf);
                        interfaceCliente(*cliente);
                        break;
                    }
                case 2:
                    {
                        cout << "[Interface do Instrutor]" << endl;
                        cout << "Digite seu CREF: " << endl;
                        cin >> cref;
                        {
                            Instrutor* instrutor = gerenciador.consultarInstrutor(cref);
                            interfaceInstrutor(gerenciador, instrutor);
                        }
                        break;
                    }
                case 3:
                    cout << "[Interface do Gerenciador]" << endl;
                    interfaceGerenciador(gerenciador);
                    break;

                case 0:
                    executando = false;
                    cout << "Encerrando sistema..." << endl;
                    break;

                default:
                    throw AcademiaException("Opção Invalida");
            }
        }
        catch(const AcademiaException& e){
            std::cout << "Erro: " << e.what() << '\n';
        }
        
    }

    try 
    {
        gerenciador.salvarEmArquivo("data/backup.txt");
    } 
    catch (const AcademiaException& e) 
    {
        cout << "Erro ao salvar backup: " << e.what() << '\n';
    }


    return 0;
}