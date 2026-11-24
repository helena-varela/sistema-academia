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
}
