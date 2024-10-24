#ifndef BABY_H
#define BABY_H

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
    void registrarDerrota() { derrotas++; }
    void exibirRanking() const {
        cout << nome << " - Vitórias: " << vitorias 
             << ", Derrotas: " << derrotas 
             << ", Pontuação: " << pontuacao << '\n';
    }
};

#endif 