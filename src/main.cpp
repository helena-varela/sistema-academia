#include <iostream>
#include "../include/AcademiaException.hpp"
#include "../include/GerenciadorAcademia.hpp"

using namespace std;

int main()
{
    GerenciadorAcademia gerenciador;

    bool executando = true;
    int opcao;

    try
    {
        // gerenciador.carregarDeArquivo();

        while (executando)
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
                    cout << "[Interface do Cliente]" << endl;
                    // interfaceCliente();
                    break;

                case 2:
                    cout << "[Interface do Instrutor]" << endl;
                    // interfaceInstrutor();
                    break;

                case 3:
                    cout << "[Interface do Gerenciador]" << endl;
                    // interfaceGerenciador();
                    break;

                case 0:
                    executando = false;
                    cout << "Encerrando sistema..." << endl;
                    break;

                default:
                    throw AcademiaException("Opção Inválida");
            }
        }

        // gerenciador.salvarEmArquivo();
    }
    catch (const AcademiaException& e)
    {
        cout << "Erro: " << e.what() << endl;
    }

    return 0;
}