# Variáveis de compilação
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Pastas do projeto
SRC_DIR = src
BIN_DIR = bin

# Encontra todos os arquivos .cpp na pasta src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Define os arquivos de objeto (.o) correspondentes na pasta bin
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# Nome do executável final
TARGET = $(BIN_DIR)/sistema

# Regra principal (o que acontece quando você digita apenas 'make')
all: $(TARGET)

# Regra para linkar os objetos e criar o executável executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Regra para compilar cada arquivo .cpp em um arquivo .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados (make clean)
clean:
	rm -rf $(BIN_DIR)

# Regra para rodar o sistema direto (make run)
run: $(TARGET)
	./$(TARGET)