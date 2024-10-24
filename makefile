# Nome do executável
EXEC = pokemons

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++20

# Arquivos fonte
SRC = pokemons.cpp

# Regra padrão para compilar e gerar o executável
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

# Regra para rodar o executável
run: $(EXEC)
	./$(EXEC)

# Limpar arquivos gerados
clean:
	rm -f $(EXEC)
