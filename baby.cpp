#include 'baby.h'

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

vector<Ataque> carregarAtaques(const string& caminhoArquivo) {
    vector<Ataque> ataques;
    ifstream arquivo(caminhoArquivo);
    string linha;

    if (arquivo.is_open()) {
        while (getline(arquivo, linha)) {
            string nome, tipo;
            size_t pos = linha.find(',');
            nome = linha.substr(0, pos);
            tipo = linha.substr(pos + 1);
            ataques.emplace_back(nome, tipo);
        }
        arquivo.close();
    }
    return ataques;
}

vector<Pokemon> sortearPokemons(const vector<Pokemon>& lista, int quantidade) {
    vector<Pokemon> sorteados;
    vector<bool> indicesUsados(lista.size(), false);

    while (sorteados.size() < quantidade) {
        int indice = rand() % lista.size();
        if (!indicesUsados[indice]) {
            sorteados.push_back(lista[indice]);
            indicesUsados[indice] = true;
        }
    }
    return sorteados;
}

vector<Pokemon> ajustarPokemonsCPU(const vector<Pokemon>& pokemonsJogador, int dificuldade) {
    vector<Pokemon> pokemonsCPU;
    int maxNivel = 0;
    int minNivel = numeric_limits<int>::max();

    for (const auto& pokemon : pokemonsJogador) {
        if (pokemon.nivel > maxNivel) {
            maxNivel = pokemon.nivel;
        }
        if (pokemon.nivel < minNivel) {
            minNivel = pokemon.nivel;
        }
    }

    for (const auto& pokemon : pokemonsJogador) {
        Pokemon pokemonCPU = pokemon; // Copia o Pokémon do jogador
        if (dificuldade == 1) { // Fácil
            pokemonCPU.nivel = rand() % (minNivel + 1); // Nível ≤ Pokémon mais fraco
        } else if (dificuldade == 2) { // Médio
            pokemonCPU.nivel = rand() % (maxNivel + 1); // Nível ≤ Pokémon mais forte
        } else if (dificuldade == 3) { // Difícil
            pokemonCPU.nivel = maxNivel + (rand() % 5 + 1); // Nível ≥ Pokémon mais forte + 1 a 5
        }

        pokemonsCPU.push_back(pokemonCPU);
    }

    return pokemonsCPU;
}

Ataque escolherAtaqueCPU(const Pokemon& pokemonCPU, int dificuldade) {
    if (dificuldade == 1) {
        return pokemonCPU.ataques[0]; // Menor dano
    } else if (dificuldade == 2) {
        return pokemonCPU.ataques[rand() % pokemonCPU.ataques.size()]; // Aleatório
    } else { // Difícil
        return pokemonCPU.ataques.back(); // Maior dano
    }
}

void exibirPokemons(const vector<Pokemon>& pokemons) {
    for (const auto& pokemon : pokemons) {
        cout << pokemon.nome << " (" << pokemon.tipo1;
        if (!pokemon.tipo2.empty()) {
            cout << "/" << pokemon.tipo2;
        }
        cout << ") - Nível: " << pokemon.nivel << "\n";
        for (const auto& ataque : pokemon.ataques) {
            cout << " - " << ataque.nome << " (" << ataque.tipo << ")\n";
        }
    }
}

void exibirRanking(const vector<Jogador>& jogadores, const Jogador& cpu) {
    cout << "Ranking atualizado:\n";
    for (const auto& jogador : jogadores) {
        jogador.exibirRanking();
    }
    cpu.exibirRanking(); // Exibe o ranking da CPU
}

void batalhaPokemon(vector<Pokemon>& pokemonsJogador, vector<Pokemon>& pokemonsCPU, int dificuldade) {
    int indexJogador = 0;
    int indexCPU = 0;

    while (indexJogador < pokemonsJogador.size() && indexCPU < pokemonsCPU.size()) {
        Pokemon& pokemonJogador = pokemonsJogador[indexJogador];
        Pokemon& pokemonCPU = pokemonsCPU[indexCPU];

        while (!pokemonJogador.estaDerrotado() && !pokemonCPU.estaDerrotado()) {
            // Turno do jogador
            cout << "Seu Pokémon: \n";
            pokemonJogador.exibirStatus();
            cout << "Pokémon do adversário: \n";
            pokemonCPU.exibirStatus();

            cout << "Escolha uma ação:\n";
            cout << "1. Atacar\n";
            cout << "2. Trocar de Pokémon\n";
            int acao;
            cin >> acao;

            if (acao == 1) {
                // Jogador escolhe ataque
                cout << "Escolha um ataque:\n";
                for (size_t i = 0; i < pokemonJogador.ataques.size(); ++i) {
                    cout << i + 1 << ". " << pokemonJogador.ataques[i].nome << "\n";
                }
                int escolhaAtaque;
                cin >> escolhaAtaque;

                int dano = rand() % 20 + 10;  // Dano entre 10 e 30
                pokemonCPU.receberDano(dano);
                cout << pokemonJogador.nome << " usou " << pokemonJogador.ataques[escolhaAtaque - 1].nome
                     << " e causou " << dano << " de dano!\n";
            } else if (acao == 2) {
                // Trocar de Pokémon
                cout << "Escolha um Pokémon para trocar:\n";
                for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
                    if (!pokemonsJogador[i].estaDerrotado()) {
                        cout << i + 1 << ". " << pokemonsJogador[i].nome << "\n";
                    }
                }
                cin >> indexJogador;
                indexJogador--; // Para alinhar com o índice do vetor
                cout << "Você trocou para " << pokemonsJogador[indexJogador].nome << "!\n";
                continue; // Pular o turno de ataque
            }

            if (pokemonCPU.estaDerrotado()) {
                cout << pokemonCPU.nome << " foi derrotado!\n";
                indexCPU++;
                if (indexCPU < pokemonsCPU.size()) {
                    cout << "O adversário escolheu " << pokemonsCPU[indexCPU].nome << "!\n";
                }
                break;
            }

            // Turno da CPU
            Ataque ataqueCPU = escolherAtaqueCPU(pokemonCPU, dificuldade);
            int danoCPU = rand() % 20 + 10;
            pokemonJogador.receberDano(danoCPU);
            cout << pokemonCPU.nome << " usou " << ataqueCPU.nome << " e causou " << danoCPU << " de dano!\n";

            if (pokemonJogador.estaDerrotado()) {
                cout << pokemonJogador.nome << " foi derrotado!\n";
                indexJogador++;
                if (indexJogador < pokemonsJogador.size()) {
                    cout << "Você escolheu " << pokemonsJogador[indexJogador].nome << "!\n";
                }
                break;
            }
        }
    }

    if (indexJogador >= pokemonsJogador.size()) {
        cout << "Todos os seus Pokémons foram derrotados! A CPU venceu!\n";
    } else if (indexCPU >= pokemonsCPU.size()) {
        cout << "Todos os Pokémons da CPU foram derrotados! Você venceu!\n";
    }
}

void iniciarBatalha(vector<Pokemon>& pokemonsJogador, vector<Pokemon>& pokemonsCPU, int dificuldade) {
    // Jogador escolhe o Pokémon para iniciar a batalha
    cout << "Escolha seu Pokémon para iniciar a batalha:\n";
    for (size_t i = 0; i < pokemonsJogador.size(); ++i) {
        cout << i + 1 << ". " << pokemonsJogador[i].nome << "\n";
    }
    int escolhaJogador;
    cin >> escolhaJogador;

    // Garantir que o jogador começa com o Pokémon correto
    Pokemon& pokemonJogador = pokemonsJogador[escolhaJogador - 1];

    // Inicia a batalha com os vetores inteiros de Pokémons
    batalhaPokemon(pokemonsJogador, pokemonsCPU, dificuldade);
}


void exibirMenu(vector<Jogador>& jogadores, const vector<Pokemon>& pokemons, const vector<Ataque>& ataques) {
    Jogador cpu("CPU");
    int escolha;
    int dificuldade = 1; // Padrão: Fácil

    do {
        cout << "_____________________________\n";
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
                cout << "Niveis de dificuldades:\n";
                cout << "1. Fácil\n";
                cout << "2. Médio\n";
                cout << "3. Difícil\n";
                cout << "Digite o nivel de dificuldade desejada: ";
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
                        
                        // Sortear Pokémons do jogador e da CPU
                        auto pokemonsJogador = sortearPokemons(pokemons, 3);
                        auto pokemonsCPU = ajustarPokemonsCPU(pokemonsJogador, dificuldade);

                        // Adicionar ataques para Pokémons do jogador
                        for (auto& pokemon : pokemonsJogador) {
                            for (int i = 0; i < 4; ++i) {
                                pokemon.adicionarAtaque(ataques[rand() % ataques.size()]);
                            }
                        }

                        // Adicionar ataques para Pokémons da CPU
                        for (auto& pokemon : pokemonsCPU) {
                            for (int i = 0; i < 4; ++i) {
                                pokemon.adicionarAtaque(ataques[rand() % ataques.size()]);
                            }
                        }

                        cout << "Pokémons do jogador e seus ataques:\n";
                        exibirPokemons(pokemonsJogador);
                        cout << "\nPokémons da CPU e seus ataques:\n";
                        exibirPokemons(pokemonsCPU);

                        // Iniciar a batalha real entre o jogador e a CPU
                        iniciarBatalha(pokemonsJogador, pokemonsCPU, dificuldade);
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
    vector<Ataque> ataques = carregarAtaques("ataques.txt");

    vector<Jogador> jogadores;

    exibirMenu(jogadores, pokemons, ataques);

    return 0;
}