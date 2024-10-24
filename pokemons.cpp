#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <set>
#include <algorithm>

using namespace std;

class Ataque {
public:
    string nome;
    string tipo;

    Ataque(const string& nome, const string& tipo) : nome(nome), tipo(tipo) {}
};

class Pokemon {
public:
    string nome;
    string tipo1;
    string tipo2;
    vector<Ataque> ataques;
    int nivel;
    int hpMax;
    int hpAtual;

    Pokemon(const string& nome, const string& tipo1, int nivel, const string& tipo2 = "")
        : nome(nome), tipo1(tipo1), nivel(nivel), tipo2(tipo2), hpMax(100), hpAtual(100) {}

    void adicionarAtaque(const Ataque& ataque) {
        ataques.push_back(ataque);
    }

    bool estaDerrotado() const {
        return hpAtual <= 0;
    }

    void receberDano(int dano) {
        hpAtual -= dano;
        if (hpAtual < 0) hpAtual = 0;
    }

    void exibirStatus() const {
        cout << nome << " - HP: " << hpAtual << "/" << hpMax << "\n";
    }
};

class Jogador {
public:
    string nome;
    int vitorias = 0;
    int derrotas = 0;
    int pontuacao = 0;

    Jogador(const string& nome) : nome(nome) {}

    void registrarVitoria(int pontos) {
        vitorias++;
        pontuacao += pontos;
    }

    void registrarDerrota() {
        derrotas++;
    }

    void exibirRanking() const {
        cout << nome << " - Vitórias: " << vitorias
             << ", Derrotas: " << derrotas
             << ", Pontuação: " << pontuacao << '\n';
    }
};

vector<Pokemon> carregarPokemons(const string& caminhoArquivo) {
    vector<Pokemon> pokemons;
    ifstream arquivo(caminhoArquivo);
    string linha;

    if (arquivo.is_open()) {
        while (getline(arquivo, linha)) {
            string nome, tipo1, tipo2;
            size_t pos1 = linha.find(',');
            size_t pos2 = linha.find(',', pos1 + 1);
            nome = linha.substr(0, pos1);
            tipo1 = linha.substr(pos1 + 1, pos2 - pos1 - 1);
            tipo2 = linha.substr(pos2 + 1);
            pokemons.emplace_back(nome, tipo1, 1, tipo2);
        }
        arquivo.close();
    }
    return pokemons;
}

// Função para carregar os ataques de um arquivo (pulando a primeira linha)
vector<Ataque> carregarAtaques(const string& caminhoArquivo) {
    vector<Ataque> listaAtaques;
    ifstream arquivo(caminhoArquivo);
    string linha;

    if (arquivo.is_open()) {
        getline(arquivo, linha); // Pula a primeira linha (cabeçalho)
        while (getline(arquivo, linha)) {
            string nome, tipo;
            size_t pos1 = linha.find(',');
            size_t pos2 = linha.find_last_of(',');

            nome = linha.substr(0, pos1);
            tipo = linha.substr(pos2 + 1);

            listaAtaques.emplace_back(nome, tipo); // Cria e adiciona o ataque
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
    }

    return listaAtaques;
}

vector<Pokemon> sortearPokemons(const vector<Pokemon>& lista, int quantidade) {
    vector<Pokemon> sorteados;
    vector<bool> indicesUsados(lista.size(), false);

    while (sorteados.size() < quantidade) {
        int indice = rand() % lista.size(); //Sorteia o pokemon a partir do indice 1
        if (!indicesUsados[indice]) {
            sorteados.push_back(lista[indice]);
            indicesUsados[indice] = true;
        }
    }
    return sorteados;
}

vector<Ataque> sortearAtaques(const vector<Ataque>& listaAtaques, const Pokemon& pokemon) {
    vector<Ataque> ataquesSorteados;
    vector<bool> indicesUsados(listaAtaques.size(), false);
    
    while (ataquesSorteados.size() < 4) {
        int indice = rand() % listaAtaques.size();
        if (!indicesUsados[indice]) {
            const Ataque& ataque = listaAtaques[indice];
            if (ataque.tipo == "Normal" || 
                ataque.tipo == pokemon.tipo1 || 
                ataque.tipo == pokemon.tipo2) {
                ataquesSorteados.push_back(ataque);
            }
            indicesUsados[indice] = true;
        }
    }
    
    return ataquesSorteados;
}

vector<Pokemon> ajustarPokemonsCPU(const vector<Pokemon>& pokemonsJogador, int dificuldade, const vector<Pokemon>& todosPokemons) {
    vector<Pokemon> pokemonsCPU;
    set<int> indicesUsados;

    for (const auto& pokemon : pokemonsJogador) {
        auto it = find_if(todosPokemons.begin(), todosPokemons.end(), [&](const Pokemon& p) { return p.nome == pokemon.nome; });
        if (it != todosPokemons.end()) {
            indicesUsados.insert(distance(todosPokemons.begin(), it));
        }
    }

    while (pokemonsCPU.size() < pokemonsJogador.size()) {
        int indice = rand() % todosPokemons.size();
        if (indicesUsados.find(indice) == indicesUsados.end()) {
            Pokemon pokemonCPU = todosPokemons[indice];

            if (dificuldade == 1) {
                pokemonCPU.nivel = rand() % (pokemonsJogador[0].nivel + 1);
            } else if (dificuldade == 2) {
                pokemonCPU.nivel = rand() % (pokemonsJogador[0].nivel + 1);
            } else if (dificuldade == 3) {
                pokemonCPU.nivel = pokemonsJogador[0].nivel + (rand() % 5 + 1);
            }

            pokemonsCPU.push_back(pokemonCPU);
            indicesUsados.insert(indice);
        }
    }

    return pokemonsCPU;
}

Ataque escolherAtaqueCPU(const Pokemon& pokemonCPU, int dificuldade) {
    if (dificuldade == 1) {
        return pokemonCPU.ataques[0];
    } else if (dificuldade == 2) {
        return pokemonCPU.ataques[rand() % pokemonCPU.ataques.size()];
    } else {
        return pokemonCPU.ataques.back();
    }
}

void mostrarPokemonsESeusAtaques(const vector<Pokemon>& pokemonsJogador, const vector<Pokemon>& pokemonsCPU, const vector<Ataque>& listaAtaques) {
    cout << "Pokémons do jogador e seus ataques:\n";
    int contador = 1;

    // Mostrar os Pokémons e ataques do jogador
    for (const auto& pokemon : pokemonsJogador) {
        cout << pokemon.nome << " (" << pokemon.tipo1 << "/" << (pokemon.tipo2.empty() ? "Nenhum" : pokemon.tipo2) << ")\n";
        vector<Ataque> ataquesSorteados = sortearAtaques(listaAtaques, pokemon);
        for (const auto& ataque : ataquesSorteados) {
            cout << contador << ". " << ataque.nome << " (" << ataque.tipo << ")\n";
            contador = (contador % 4) + 1;
        }
    }

    cout << "\nPokémons da CPU e seus ataques:\n";
    
    // Mostrar os Pokémons e ataques da CPU
    for (const auto& pokemon : pokemonsCPU) {
        cout << pokemon.nome << " (" << pokemon.tipo1 << "/" << (pokemon.tipo2.empty() ? "Nenhum" : pokemon.tipo2) << ")\n";
        vector<Ataque> ataquesSorteados = sortearAtaques(listaAtaques, pokemon);
        for (const auto& ataque : ataquesSorteados) {
            cout << contador << ". " << ataque.nome << " (" << ataque.tipo << ")\n";
            contador = (contador % 4) + 1;
        }
    }
}

void exibirRanking(const vector<Jogador>& jogadores, const Jogador& cpu) {
    cout << "Ranking atualizado:\n";
    for (const auto& jogador : jogadores) {
        jogador.exibirRanking();
    }
    cpu.exibirRanking();
}

void batalhaPokemon(vector<Pokemon>& pokemonsJogador, vector<Pokemon>& pokemonsCPU, int dificuldade, vector<Ataque>& listaAtaques ) {
    int indiceJogador = 0;
    int indiceCPU = 0;

    cout << "Escolha um Pokémon:\n";
    for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
        cout << i + 1 << ". " << pokemonsJogador[i].nome << " - Nível: " << pokemonsJogador[i].nivel << " - HP: " << pokemonsJogador[i].hpAtual << "/" << pokemonsJogador[i].hpMax << "\n";
    }
    cin >> indiceJogador;
    indiceJogador--; // Para ajustar ao índice 0.

    cout << "A CPU escolheu " << pokemonsCPU[indiceCPU].nome << ".\n";

    bool batalhaContinua = true;

    while (batalhaContinua) {
        // Turno do jogador
        if (!pokemonsJogador[indiceJogador].estaDerrotado()) {
            pokemonsJogador[indiceJogador].exibirStatus();
            pokemonsCPU[indiceCPU].exibirStatus();

            cout << "Escolha uma ação:\n1. Atacar\n2. Trocar Pokémon\n";
            int escolha;
            cin >> escolha;

            if (escolha == 1) {
                // Atacar
                cout << "Escolha um ataque:\n";
                
                // Exibe os ataques do Pokémon do jogador
                for (size_t i = 0; i < pokemonsJogador[indiceJogador].ataques.size(); ++i) {
                    cout << i + 1 << ". " << pokemonsJogador[indiceJogador].ataques[i].nome 
                        << " (" << pokemonsJogador[indiceJogador].ataques[i].tipo << ")\n";
                }
                
                
                // Entrada do jogador para escolher o ataque
                int ataqueEscolhido;
                cout << "\nEscolha um ataque: "; // Aqui é o prompt para a escolha
                cin >> ataqueEscolhido;
                ataqueEscolhido--; // Ajusta a escolha para índice do array

                // Calcula dano aleatório
                int dano = rand() % 20 + 10; // Dano entre 10 e 30
                pokemonsCPU[indiceCPU].receberDano(dano);

                // Exibe a mensagem do ataque realizado
                cout << pokemonsJogador[indiceJogador].nome << " usou " 
                    << pokemonsJogador[indiceJogador].ataques[ataqueEscolhido].nome 
                    << " e causou " << dano << " de dano!\n";
            } else if (escolha == 2) {
                // Trocar Pokémon
                cout << "Escolha o Pokémon para trocar:\n";
                
                // Exibe a lista de Pokémons disponíveis para troca
                for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
                    if (!pokemonsJogador[i].estaDerrotado()) {
                        cout << i + 1 << ". " << pokemonsJogador[i].nome 
                            << " - HP: " << pokemonsJogador[i].hpAtual 
                            << "/" << pokemonsJogador[i].hpMax << "\n";
                    }
                }

                // Entrada do jogador para escolher o Pokémon
                cin >> indiceJogador;
                indiceJogador--; // Ajusta a escolha para o índice do array

                // Exibe a mensagem de troca
                cout << "Você trocou para " << pokemonsJogador[indiceJogador].nome << "!\n";
            }


            // Verifica se o Pokémon da CPU foi derrotado
            if (pokemonsCPU[indiceCPU].estaDerrotado()) {
                cout << pokemonsCPU[indiceCPU].nome << " foi derrotado!\n";
                indiceCPU++;

                if (indiceCPU < pokemonsCPU.size()) {
                    cout << "A CPU escolheu " << pokemonsCPU[indiceCPU].nome << "!\n";
                    // Jogador pode trocar de Pokémon sem perder a vez
                    cout << "Você pode trocar de Pokémon sem perder a vez.\nDeseja trocar? (1 para sim, 2 para não): ";
                    int troca;
                    cin >> troca;
                    if (troca == 1) {
                        cout << "Escolha o Pokémon para trocar:\n";
                        for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
                            if (!pokemonsJogador[i].estaDerrotado()) {
                                cout << i + 1 << ". " << pokemonsJogador[i].nome << " - HP: " << pokemonsJogador[i].hpAtual << "/" << pokemonsJogador[i].hpMax << "\n";
                            }
                        }
                        cin >> indiceJogador;
                        indiceJogador--;
                        cout << "Você trocou para " << pokemonsJogador[indiceJogador].nome << "!\n";
                    }
                } else {
                    cout << "Parabéns! Você derrotou todos os Pokémons da CPU!\n";
                    batalhaContinua = false;
                    break;
                }
            }
        }

        // Turno da CPU
        if (!pokemonsCPU[indiceCPU].estaDerrotado() && batalhaContinua) {
            cout << "\nTurno da CPU!\n";
            Ataque ataqueCPU = escolherAtaqueCPU(pokemonsCPU[indiceCPU], dificuldade);
            int dano = rand() % 20 + 10;
            pokemonsJogador[indiceJogador].receberDano(dano);
            cout << pokemonsCPU[indiceCPU].nome << " usou " << ataqueCPU.nome << " e causou " << dano << " de dano!\n";

            // Verifica se o Pokémon do jogador foi derrotado
            if (pokemonsJogador[indiceJogador].estaDerrotado()) {
                cout << pokemonsJogador[indiceJogador].nome << " foi derrotado!\n";
                bool todosDerrotados = true;
                for (const auto& pokemon : pokemonsJogador) {
                    if (!pokemon.estaDerrotado()) {
                        todosDerrotados = false;
                        break;
                    }
                }

                if (todosDerrotados) {
                    cout << "Todos os seus Pokémons foram derrotados. A CPU venceu!\n";
                    batalhaContinua = false;
                } else {
                    cout << "Escolha outro Pokémon:\n";
                    for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
                        if (!pokemonsJogador[i].estaDerrotado()) {
                            cout << i + 1 << ". " << pokemonsJogador[i].nome << " - HP: " << pokemonsJogador[i].hpAtual << "/" << pokemonsJogador[i].hpMax << "\n";
                        }
                    }
                    cin >> indiceJogador;
                    indiceJogador--;
                    cout << "Você escolheu " << pokemonsJogador[indiceJogador].nome << "!\n";
                }
            }
        }
    }
}

void exibirMenu(vector<Jogador>& jogadores, const vector<Pokemon>& pokemons, const vector<Ataque>& ataques) {
    Jogador cpu("CPU");
    int escolha;
    int dificuldade = 1; // Padrão: Fácil

    do {
        cout << "----------------------------\n";
        cout << "\nBEM VINDO A BATALHA POKEMON\n";
        cout << "1. Inscrever Novo Jogador\n";
        cout << "2. Selecionar Dificuldade\n";
        cout << "3. Iniciar Batalha\n";
        cout << "4. Exibir Ranking\n";
        cout << "5. Sair\n";
        cout << "----------------------------\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;

        switch (escolha) {
            case 1: {
                string nomeJogador;
                cout << "Digite o nome do novo jogador: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nomeJogador);
                jogadores.emplace_back(nomeJogador);
                break;
            }
            case 2: {
                cout << "Níveis de dificuldades:\n";
                cout << "1. Fácil\n";
                cout << "2. Médio\n";
                cout << "3. Difícil\n";
                cout << "Digite o nível de dificuldade desejada: ";
                cin >> dificuldade;
                cout << "Dificuldade ajustada!\n";
                break;
            }
            case 3: {
                if (jogadores.empty()) {
                    cout << "Nenhum jogador cadastrado!\n";
                } else {
                    cout << "Jogadores registrados\n";
                    for (size_t i = 0; i < jogadores.size(); ++i) {
                        cout << i + 1 << ". " << jogadores[i].nome << "\n";
                    }
                    cout << "Escolha um jogador: ";
                    int jogadorEscolhido;
                    cin >> jogadorEscolhido;

                    if (jogadorEscolhido > 0 && jogadorEscolhido <= jogadores.size()) {
                        cout << "Iniciando batalha...\n";

                        auto pokemonsJogador = sortearPokemons(pokemons, 3);
                        auto pokemonsCPU = ajustarPokemonsCPU(pokemonsJogador, dificuldade, pokemons);
                        vector<Ataque> listaAtaques = carregarAtaques("ataques.txt");
                        if (listaAtaques.empty()) {
                            cerr << "Lista de ataques vazia!" << endl;
                            return;
                        }

                        mostrarPokemonsESeusAtaques(pokemonsJogador, pokemonsCPU, listaAtaques);
                        batalhaPokemon(pokemonsJogador, pokemonsCPU, dificuldade, listaAtaques);
                    } else {
                        cout << "Jogador inválido!\n";
                    }
                }
                break;
            }
            case 4: {
                exibirRanking(jogadores, cpu);
                break;
            }
            case 5:
                cout << "Saindo do jogo...\n";
                break;
            default:
                cout << "Opção inválida! Tente novamente.\n";
                break;
        }
    } while (escolha != 5);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<Pokemon> pokemons = carregarPokemons("pokemons.txt");
    vector<Ataque> listaAtaques = carregarAtaques("ataques.txt");
    if (listaAtaques.empty()) {
        cerr << "Lista de ataques vazia!" << endl;
        return 1;
    }

    vector<Jogador> jogadores;

    exibirMenu(jogadores, pokemons, listaAtaques);

    return 0;
}