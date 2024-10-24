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

// Função para carregar os Pokémon de um arquivo
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

// Função para carregar os ataques de um arquivo
vector<Ataque> carregarAtaques(const string& caminhoArquivo) {
    vector<Ataque> listaAtaques;
    ifstream arquivo(caminhoArquivo);
    string linha;

    if (arquivo.is_open()) {
        getline(arquivo, linha); // Pula a primeira linha
        while (getline(arquivo, linha)) {
            string nome, tipo;
            size_t pos1 = linha.find(',');
            nome = linha.substr(0, pos1);
            tipo = linha.substr(pos1 + 1);
            listaAtaques.emplace_back(nome, tipo); // Cria e adiciona o ataque
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
    }
    return listaAtaques;
}

// Função para ajustar o nível dos Pokémon da CPU conforme a dificuldade
vector<Pokemon> ajustarPokemonsCPU(const vector<Pokemon>& pokemonsJogador, int dificuldade, const vector<Pokemon>& todosPokemons) {
    vector<Pokemon> pokemonsCPU;
    int nivelMaisFraco = min_element(pokemonsJogador.begin(), pokemonsJogador.end(), [](const Pokemon& p1, const Pokemon& p2) {
        return p1.nivel < p2.nivel;
    })->nivel;

    int nivelMaisForte = max_element(pokemonsJogador.begin(), pokemonsJogador.end(), [](const Pokemon& p1, const Pokemon& p2) {
        return p1.nivel < p2.nivel;
    })->nivel;

    for (const auto& pokemonJogador : pokemonsJogador) {
        Pokemon pokemonCPU = todosPokemons[rand() % todosPokemons.size()];

        // Ajustar nível conforme dificuldade
        if (dificuldade == 1) {
            pokemonCPU.nivel = rand() % (nivelMaisFraco + 1);
        } else if (dificuldade == 2) {
            pokemonCPU.nivel = rand() % (nivelMaisForte + 1);
        } else if (dificuldade == 3) {
            pokemonCPU.nivel = nivelMaisForte + (rand() % 5 + 1);
        }

        pokemonsCPU.push_back(pokemonCPU);
    }

    return pokemonsCPU;
}

// Função para escolher o ataque da CPU conforme a dificuldade
Ataque escolherAtaqueCPU(const Pokemon& pokemonCPU, int dificuldade) {
    if (dificuldade == 1) {
        // Escolhe o ataque de menor dano (primeiro da lista)
        return pokemonCPU.ataques.front();
    } else if (dificuldade == 2) {
        // Escolhe o ataque aleatoriamente
        return pokemonCPU.ataques[rand() % pokemonCPU.ataques.size()];
    } else if (dificuldade == 3) {
        // Escolhe o ataque de maior dano (último da lista)
        return pokemonCPU.ataques.back();
    }
    return pokemonCPU.ataques[0]; // Padrão (caso não entre nas condições)
}

void exibirMenu(vector<Jogador>& jogadores, const vector<Pokemon>& pokemons, const vector<Ataque>& ataques) {
    Jogador cpu("CPU");
    int escolha;
    int dificuldade = 1; // Dificuldade padrão (Fácil)

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

                        auto pokemonsJogador = carregarAtaques(pokemons, 3);
                        auto pokemonsCPU = ajustarPokemonsCPU(pokemonsJogador, dificuldade, pokemons);

                        cout << "Pokémons do jogador e seus ataques:\n";
                        for (const auto& pokemon : pokemonsJogador) {
                            cout << pokemon.nome << " (" << pokemon.tipo1 << "/" << pokemon.tipo2 << ")\n";
                        }

                        cout << "\nPokémons da CPU e seus ataques:\n";
                        for (const auto& pokemon : pokemonsCPU) {
                            cout << pokemon.nome << " (" << pokemon.tipo1 << "/" << pokemon.tipo2 << ")\n";
                        }
                    } else {
                        cout << "Jogador inválido!\n";
                    }
                }
                break;
            }
            case 4: {
                cout << "Ranking dos Jogadores:\n";
                for (const auto& jogador : jogadores) {
                    jogador.exibirRanking();
                }
                break;
            }
            case 5:
                cout << "Saindo do programa...\n";
                break;
            default:
                cout << "Opção inválida!\n";
                break;
        }

    } while (escolha != 5);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    vector<Jogador> jogadores;
    vector<Pokemon> pokemons = carregarPokemons("pokemons.csv");
    vector<Ataque> ataques = carregarAtaques("ataques.csv");

    // Atribuir ataques aos pokemons (exemplo básico)
    for (auto& pokemon : pokemons) {
        for (int i = 0; i < 4; ++i) {
            pokemon.adicionarAtaque(ataques[rand() % ataques.size()]);
        }
    }

    exibirMenu(jogadores, pokemons, ataques);

    return 0;
}
