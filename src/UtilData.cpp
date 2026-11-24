#include "UtilData.hpp"
#include "AcademiaException.hpp"

#include <cctype>
#include <cstdio>
#include <ctime>

namespace
{
    bool soDigitos(const std::string& s)
    {
        for (char c : s)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        }
        return !s.empty();
    }

    std::tm paraTm(const std::string& data)
    {
        if (!UtilData::dataValida(data))
        {
            throw AcademiaException("Data invalida: " + data + " (use DD/MM/AAAA).");
        }
        std::tm t{};
        t.tm_mday = std::stoi(data.substr(0, 2));
        t.tm_mon = std::stoi(data.substr(3, 2)) - 1;
        t.tm_year = std::stoi(data.substr(6, 4)) - 1900;
        t.tm_hour = 12; // meio-dia evita problemas com horario de verao
        return t;
    }
}

namespace UtilData
{
    std::string hoje()
    {
        std::time_t agora = std::time(nullptr);
        std::tm* local = std::localtime(&agora);
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d",
                      local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
        return buffer;
    }

    std::string mesAtual()
    {
        return mesDe(hoje());
    }

    std::string mesDe(const std::string& data)
    {
        if (!dataValida(data))
        {
            throw AcademiaException("Data invalida: " + data + " (use DD/MM/AAAA).");
        }
        return data.substr(3);
    }

    int diaDe(const std::string& data)
    {
        if (!dataValida(data))
        {
            throw AcademiaException("Data invalida: " + data + " (use DD/MM/AAAA).");
        }
        return std::stoi(data.substr(0, 2));
    }

    bool dataValida(const std::string& data)
    {
        if (data.size() != 10 || data[2] != '/' || data[5] != '/')
            return false;
        if (!soDigitos(data.substr(0, 2)) || !mesValido(data.substr(3)))
            return false;
        int dia = std::stoi(data.substr(0, 2));
        return dia >= 1 && dia <= 31;
    }

    bool mesValido(const std::string& mes)
    {
        if (mes.size() != 7 || mes[2] != '/')
            return false;
        if (!soDigitos(mes.substr(0, 2)) || !soDigitos(mes.substr(3, 4)))
            return false;
        int m = std::stoi(mes.substr(0, 2));
        return m >= 1 && m <= 12;
    }

    int mesComoNumero(const std::string& mes)
    {
        if (!mesValido(mes))
        {
            throw AcademiaException("Mes invalido: " + mes + " (use MM/AAAA).");
        }
        return std::stoi(mes.substr(3, 4)) * 12 + std::stoi(mes.substr(0, 2));
    }

    int diasEntre(const std::string& inicio, const std::string& fim)
    {
        std::tm a = paraTm(inicio);
        std::tm b = paraTm(fim);
        std::time_t ta = std::mktime(&a);
        std::time_t tb = std::mktime(&b);
        return static_cast<int>(std::difftime(tb, ta) / (60 * 60 * 24) + (tb >= ta ? 0.5 : -0.5));
    }
}
