#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// Estrutura para representar um ataque
struct Attack {
    string name;
    string type;
};

// Estrutura para representar um Pokémon
struct Pokemon {
    string name;
    string type1;
    string type2;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int speed;
    int sp_attack;
    int sp_defense;
    Attack attacks[4];
};

// Função para exibir os detalhes dos ataques
void displayAttacks(Pokemon& pokemon) {
    cout << "Escolha um ataque:\n";
    for (int i = 0; i < 4; ++i) {
        cout << i + 1 << ". " << pokemon.attacks[i].name << " (" << pokemon.attacks[i].type << ")\n";
    }
}

// Função para o jogador escolher um ataque
int chooseAttack() {
    int choice;
    cout << "Escolha um ataque: ";
    cin >> choice;
    return choice - 1; // Subtrai 1 para ajustar ao índice do array
}

// Função para simular uma batalha básica
void battle(Pokemon& playerPokemon, Pokemon& cpuPokemon) {
    cout << "Iniciando batalha...\n\n";
    cout << "Seu Pokémon: " << playerPokemon.name << " - HP: " << playerPokemon.hp << "/" << playerPokemon.max_hp << endl;
    cout << "Pokémon da CPU: " << cpuPokemon.name << " - HP: " << cpuPokemon.hp << "/" << cpuPokemon.max_hp << endl;

    int playerChoice;
    while (playerPokemon.hp > 0 && cpuPokemon.hp > 0) {
        cout << "Escolha uma ação:\n1. Atacar\n2. Trocar Pokémon\n";
        int action;
        cin >> action;

        if (action == 1) {
            displayAttacks(playerPokemon);
            playerChoice = chooseAttack();
            
            // Simula o ataque do jogador (sem cálculo de dano detalhado para simplificação)
            cout << playerPokemon.name << " usou " << playerPokemon.attacks[playerChoice].name << "!\n";

            // Reduz o HP do oponente de forma fixa (apenas para simulação)
            cpuPokemon.hp -= 20;
            if (cpuPokemon.hp < 0) cpuPokemon.hp = 0;
            cout << cpuPokemon.name << " - HP: " << cpuPokemon.hp << "/" << cpuPokemon.max_hp << endl;

            // Verifica se o oponente foi derrotado
            if (cpuPokemon.hp == 0) {
                cout << "Você derrotou o " << cpuPokemon.name << "!\n";
                break;
            }

            // Simula o ataque do oponente (para simplificação, escolha aleatória)
            srand(time(0));
            int cpuChoice = rand() % 4;
            cout << cpuPokemon.name << " usou " << cpuPokemon.attacks[cpuChoice].name << "!\n";

            // Reduz o HP do jogador de forma fixa (apenas para simulação)
            playerPokemon.hp -= 15;
            if (playerPokemon.hp < 0) playerPokemon.hp = 0;
            cout << playerPokemon.name << " - HP: " << playerPokemon.hp << "/" << playerPokemon.max_hp << endl;

            // Verifica se o jogador foi derrotado
            if (playerPokemon.hp == 0) {
                cout << "Seu Pokémon foi derrotado!\n";
                break;
            }
        } else {
            cout << "Trocar Pokémon não está disponível no momento.\n";
        }
    }
}

// Função principal
int main() {
    // Definir ataques de Hitmonchan
    Attack hitmonchanAttacks[4] = {
        {"Tackle", "Normal"},
        {"Fury Swipes", "Normal"},
        {"Fury Attack", "Normal"},
        {"Submission", "Lutador"}
    };

    // Definir ataques de Wigglytuff
    Attack wigglytuffAttacks[4] = {
        {"Rage", "Normal"},
        {"Tackle", "Normal"},
        {"Tri Attack", "Normal"},
        {"Fury Swipes", "Normal"}
    };

    // Criar Pokémon do jogador (Hitmonchan)
    Pokemon hitmonchan = {"Hitmonchan", "Lutador", "Nenhum", 100, 100, 105, 79, 76, 35, 35, hitmonchanAttacks};

    // Criar Pokémon da CPU (Wigglytuff)
    Pokemon wigglytuff = {"Wigglytuff", "Normal", "Fada", 100, 100, 70, 45, 30, 50, 50, wigglytuffAttacks};

    // Iniciar a batalha
    battle(hitmonchan, wigglytuff);

    return 0;
}
