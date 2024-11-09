#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

class AFNtoAFD {
public:
    struct AFN {
        set<string> estados;
        set<string> finais;
        map<pair<string, char>, vector<string>> transicoes;
        string estadoInicial;
        set<char> alfabeto;
    };

    struct AFD {
        vector<set<string>> estados;
        map<pair<set<string>, char>, set<string>> transicoes;
        set<set<string>> finais;
        set<string> estadoInicial;
        set<char> alfabeto;
    };

    AFD converter(AFN afn) {
        AFD afd;
        queue<set<string>> fila;
        map<set<string>, set<string>> novoEstado;

        afd.alfabeto = afn.alfabeto; // Copia o alfabeto do AFN para o AFD

        // Inicializa o fecho-e do estado inicial
        set<string> estadoInicial = eFecho(afn, { afn.estadoInicial });
        fila.push(estadoInicial);
        novoEstado[estadoInicial] = estadoInicial;
        afd.estadoInicial = estadoInicial;

        while (!fila.empty()) {
            set<string> estadoAtual = fila.front();
            fila.pop();
            afd.estados.push_back(estadoAtual);

            for (char simbolo : afn.alfabeto) {
                if (simbolo == '&') continue; // Ignora o simbolo epsilon

                set<string> novoConjunto;

                for (const string& estado : estadoAtual) {
                    auto it = afn.transicoes.find({estado, simbolo});
                    if (it != afn.transicoes.end()) {
                        for (const string& destino : it->second) {
                            set<string> fechoEpsilonDestino = eFecho(afn, { destino });
                            novoConjunto.insert(fechoEpsilonDestino.begin(), fechoEpsilonDestino.end());
                        }
                    }
                }

                if (novoConjunto.empty()) {
                    novoConjunto.insert("{}"); // Adiciona o estado vazio
                }

                if (novoEstado.find(novoConjunto) == novoEstado.end()) {
                    fila.push(novoConjunto);
                    novoEstado[novoConjunto] = novoConjunto;
                }

                afd.transicoes[{estadoAtual, simbolo}] = novoConjunto;
            }
        }

        // Identifica estados finais no AFD
        for (auto& estado : afd.estados) {
            for (const string& subEstado : estado) {
                if (afn.finais.find(subEstado) != afn.finais.end()) {
                    afd.finais.insert(estado);
                    break;
                }
            }
        }

        return afd;
    }

    void imprimirAFD(AFD afd) {
        // Imprimir estado inicial
        cout << "AFN Convertido" << endl;
        cout << "q0: (";
        imprimirConjunto(afd.estadoInicial);
        cout << ")" << endl;

        // Imprimir conjunto de estados
        cout << "Q: (";
        bool primeiro = true;
        for (auto& estado : afd.estados) {
            if (!primeiro) cout << ", ";
            cout << "{";
            imprimirConjunto(estado);
            cout << "}";
            primeiro = false;
        }
        cout << ")" << endl;

        // Imprimir conjunto de estados finais
        cout << "F: (";
        primeiro = true;
        for (auto& estadoFinal : afd.finais) {
            if (!primeiro) cout << ", ";
            cout << "{";
            imprimirConjunto(estadoFinal);
            cout << "}";
            primeiro = false;
        }
        cout << ")" << endl;

        // Imprimir alfabeto
        cout << "A: (";
        primeiro = true;
        for (char simbolo : afd.alfabeto) {
            if (simbolo == '&') continue; // Ignora epsilon
            if (!primeiro) cout << ", ";
            cout << simbolo;
            primeiro = false;
        }
        cout << ")" << endl;

        // Imprimir tabela de transicoes
        cout << "Tabela de Transicoes do AFD:" << endl;
        for (auto& trans : afd.transicoes) {
            if (trans.first.first.count("{}") == 0) { // Ignora transicoes do estado vazio
                cout << "{";
                imprimirConjunto(trans.first.first);
                cout << "} --" << trans.first.second << "--> {";
                imprimirConjunto(trans.second);
                cout << "}" << endl;
            }
        }
    }

private:
    // Funcao que calcula o fecho-e de um conjunto de estados
    set<string> eFecho(AFN& afn, set<string> estados) {
        set<string> fecho = estados;
        queue<string> fila;

        for (const string& estado : estados) {
            fila.push(estado);
        }

        while (!fila.empty()) {
            string estadoAtual = fila.front();
            fila.pop();

            auto it = afn.transicoes.find({estadoAtual, '&'}); // Transicoes vazias
            if (it != afn.transicoes.end()) {
                for (const string& destino : it->second) {
                    if (fecho.find(destino) == fecho.end()) {
                        fecho.insert(destino);
                        fila.push(destino);
                    }
                }
            }
        }

        return fecho;
    }

    void imprimirConjunto(set<string> conjunto) {
        bool primeiro = true;
        for (const string& estado : conjunto) {
            if (!primeiro) cout << ",";
            cout << estado;
            primeiro = false;
        }
    }
};

int main() {
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        AFNtoAFD::AFN afn;

        // Leitura dos estados
        cout << "Insira os estados: ";
        string input;
        getline(cin, input);
        stringstream ss(input);
        string estado;
        while (getline(ss, estado, ',')) {
            afn.estados.insert(estado);
        }

        // Leitura do estado inicial
        while (true) {
            cout << "Insira o estado inicial: ";
            cin >> afn.estadoInicial;
            if (afn.estados.find(afn.estadoInicial) != afn.estados.end()) {
                break;
            } else {
                cout << "Estado inicial invalido! Tente novamente." << endl;
            }
        }

        // Leitura dos estados finais
        cin.ignore();  // Para ignorar o caractere de nova linha apos a leitura anterior
        while (true) {
            cout << "Insira os estados finais: ";
            getline(cin, input);
            stringstream ss2(input);
            bool valido = true;
            while (getline(ss2, estado, ',')) {
                if (afn.estados.find(estado) != afn.estados.end()) {
                    afn.finais.insert(estado);
                } else {
                    valido = false;
                    afn.finais.clear();
                    cout << "Estado final invalido encontrado: " << estado << ". Tente novamente." << endl;
                    break;
                }
            }
            if (valido) break;
        }

        // Leitura do alfabeto
        cout << "Insira o alfabeto separado por virgulas (Exemplo: a,b): ";
        getline(cin, input);
        stringstream ss3(input);
        string simboloStr;
        while (getline(ss3, simboloStr, ',')) {
            afn.alfabeto.insert(simboloStr[0]);  // Supondo que cada simbolo seja um unico caractere
        }
        afn.alfabeto.insert('&');  // Adiciona o simbolo epsilon ao alfabeto

        // Leitura das transicoes
        for (const string& estado : afn.estados) {
            for (char simbolo : afn.alfabeto) {
                cout << "Delta(" << estado << ", " << simbolo << "): ";
                getline(cin, input);
                if (!input.empty()) {
                    stringstream ss4(input);
                    string destino;
                    while (getline(ss4, destino, ',')) {
                        afn.transicoes[{estado, simbolo}].push_back(destino);
                    }
                }
            }
        }

        // Conversao e exibicao do AFD final
        AFNtoAFD conversor;
        AFNtoAFD::AFD afd = conversor.converter(afn);
        conversor.imprimirAFD(afd);

        // Pergunta ao usuario se deseja continuar
        cout << "Deseja fornecer outro AFN? (s/n): ";
        cin >> continuar;
        cin.ignore();  // Limpa o buffer de entrada
    }

    cout << "Encerrando o programa..." << endl;
    return 0;
}