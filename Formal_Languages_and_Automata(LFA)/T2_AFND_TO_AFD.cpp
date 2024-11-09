#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

typedef struct
{
    string label = "";
    bool itsFinal, itsStart;
    //Transições que o estado faz
    vector<struct Transition> delta;
} State;

//Representa uma celula na tabela de transição
typedef struct Transition
{
    //Para quais estados a transição vai
    vector<State *> to;
    //Caracter que define a transição
    char character;
} Transition;

class Automata
{
    vector<State> estados;
    vector<State *> estadosIniciais;
    vector<State *> estadosFinais;
    vector<char> alfabeto;

public:
    //Le os estados que o usuario informar
    void lerEstados()
    {
        int start, end = -1, cont = 0, n, i;
        string buffer;
        State temp;
        getline(cin, buffer);
        n = buffer.size();
        //Conta quantos estados foram inseridos
        for (i = 0; i < n; i++)
        {
            if (buffer[i] == ',')
            {
                cont++;
            }
        }
        i = 0;

        do
        {
            //Splita a entrada
            start = end + 1;
            end = buffer.find(",", start);
            temp.label = buffer.substr(start, end - start);
            temp.itsFinal = false;
            temp.itsStart = false;
            //Adiciona o novo estado na lista de estados
            estados.push_back(temp);
            i++;
        } while (i < cont + 1);
    }
    //Le os simbolos do alfabeto que o usuario informar
    void lerAlfabeto()
    {
        int n, i;
        string buffer;
        getline(cin, buffer);
        n = buffer.size();
        for (i = 0; i < n; i++)
        {
            if (buffer[i] != ',')
            {
                alfabeto.push_back(buffer[i]);
            }
        }
    }
    //Le as transições que o usuario informar
    void lerFuncaoTransicao()
    {
        int i, j, k, m = estados.size(), n = alfabeto.size(), start, end = -1;
        string buffer, find;
        Transition temp;
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                temp.to.clear();
                cout << "(" << (estados[i].label) << "," << (temp.character = alfabeto[j]) << ") = ";
                getline(cin, buffer);
                if (buffer[0] != '-')
                {
                    do
                    {
                        //Splita a entrada
                        start = end + 1;
                        end = buffer.find(",", start);
                        find = buffer.substr(start, end - start);

                        //Procura o estado inserido para ver se ele existe e adiciona uma referencia ate ele
                        //na lista de adjacencia do estado
                        for (k = 0; k < m; k++)
                        {
                            if (estados[k].label == find)
                            {
                                temp.to.push_back(&estados[k]);
                            }
                        }

                    } while (end != -1);
                }
                else
                {
                    //Se o estado inserido é invalido, então nao ha transição para esse caracter e estado 
                    temp.to.push_back(NULL);
                }
                //Adiciona a transição
                estados[i].delta.push_back(temp);
            }
        }
    }

    void lerEstadosIniciais()
    {
        int start, end = -1, j, n;
        string buffer, find;
        getline(cin, buffer);
        n = buffer.size();

        do
        {
            //Splita a entrada
            start = end + 1;
            end = buffer.find(",", start);
            find = buffer.substr(start, end - start);
            n = estados.size();

            for (j = 0; j < n; j++)
            {
                if (estados[j].label == find)
                {
                    //Se o estado existe na lista de estados adiciona uma referencia ate ele 
                    //e altera seu atributo para ser considerado um estado inicial
                    estadosIniciais.push_back(&estados[j]);
                    estados[j].itsStart = true;
                }
            }

        } while (end != -1);
    }
    void lerEstadosFinais()
    {
        int start, end = -1, j, n;
        string buffer, find;
        getline(cin, buffer);
        n = buffer.size();

        do
        {
            //Splita entrada
            start = end + 1;
            end = buffer.find(",", start);
            find = buffer.substr(start, end - start);
            n = estados.size();

            for (j = 0; j < n; j++)
            {
                if (estados[j].label == find)
                {
                    //Se o estado existe na lista de estados adiciona uma referencia ate ele 
                    //e altera seu atributo para ser considerado um estado final
                    estadosFinais.push_back(&estados[j]);
                    estados[j].itsFinal = true;
                }
            }
        } while (end != -1);
    }
    //Verifica se duas transições implicam no mesmo conjunto de estados
    bool transicaoIgual(Transition a, Transition b)
    {
        int i, j, m = a.to.size(), n = b.to.size();
        //Se o caracter de entrada for diferente nao sao iguais
        if (a.character != b.character)
        {
            return false;
        }
        //Se a quantidade de estados que elas alcançam pra esse caracter de entrada forem diferentes
        //entao tambem é falso
        if (m != n)
        {
            return false;
        }
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (a.to[i] == b.to[j])
                {
                    j = n + 1;
                }
            }
            if (j == n)
            {
                //Se iterou por todos os estados alcançaveis e alguma transição de a 
                //nao foi encontrada em B entao retorna falso
                return false;
            }
        }
        //Se todas as iterações foram feitas e nunca foi retornado falso então é verdadeira
        return true;
    }

    //Verifica se dois estados possuem as mesmas transições
    bool estadoIgual(State a, State b)
    {
        int i, m = alfabeto.size();
        for (i = 0; i < m; i++)
        {
            if (!transicaoIgual(a.delta[i], b.delta[i]))
            {
                return false;
            }
        }
        return true;
    }

    //Verifica se uma referencia para um estado existe num vetor de ponteiros para estados
    bool existeElemConjunto(vector<State *> ptss, State *pts)
    {
        int i, n;
        //Se o conjunto for vazio, entao ela nao esta contida nele
        if (ptss.empty())
        {
            return false;
        }

        n = ptss.size();
        for (i = 0; i < n; i++)
        {
            if (ptss[i] == pts)
            {
                //Se encontrar a referencia no vetor, entao retorna verdadeiro
                return true;
            }
        }
        //Se nunca encontar entao retorna falso
        return false;
    }
    //Transofrma uma transição 
    State transicaoParaEstado(Transition x)
    {
        int i, j, k, n, m, y;
        n = x.to.size();
        m = alfabeto.size();
        State ret;
        Transition temp;
        ret.itsStart = false;
        ret.itsFinal = false;
        ret.label = "{";

        //Adiciona o nome do novo estado para ser igual a todos os estados alcancaveis por essa transição
        //E no caso de algum desses estados ser final, o estado que retornaremos tambem será
        for (i = 0; i < n; i++)
        {
            ret.label = ret.label + x.to[i]->label;
            if (x.to[i]->itsFinal)
            {
                ret.itsFinal = true;
            }
            if (i + 1 < n)
            {
                ret.label = ret.label + ",";
            }
        }
        //Para cada estado alcançado, para cada caracter do alfabeto, adiciona a transição no estado que queremos retornar 
        for (j = 0; j < m; j++)
        {
            temp.character = alfabeto[j];
            for (i = 0; i < n; i++)
            {
                y = x.to[i]->delta[j].to.size();
                for (k = 0; k < y; k++)
                {
                    //Se ja foi registrado que esse estado pode ser alcançado, nao adiciona ele
                    if (!existeElemConjunto(temp.to, x.to[i]->delta[j].to[k]))
                    {
                        temp.to.push_back(x.to[i]->delta[j].to[k]);
                    }
                }
            }
            //Adiciona a transição completa no estado que queremos retornar
            ret.delta.push_back(temp);
            temp.to.clear();
        }
        ret.label = ret.label + "}";
        return ret;
    }

    //Verifica se um estado está presente num vetor de estados
    bool verificaSeEstadoFoiInserido(vector<State> ss, State s)
    {
        int i, n = ss.size();
        //Procura por um estado com transições equivalentes no vetor
        //Retornando verdadeiro em caso de sucesso ou false em caso de fracasso  
        for (i = 0; i < n; i++)
        {
            if (estadoIgual(ss[i], s))
            {
                return true;
            }
        }
        return false;
    }

    //Converte AFN para AFD
    void converteParaAFD(Automata &AFD)
    {
        int i, j, k, z, n = alfabeto.size(), l = estadosIniciais.size(), y;
        Transition tempT;
        State tempS;
        tempS.label = "{";
        
        tempS.itsStart = true;
        tempS.itsFinal = false;
        //Cria o nome do estado inicial combinando todos os estados iniciais
        for (i = 0; i < l; i++)
        {
            tempS.label = tempS.label + estadosIniciais[i]->label;
            if(estadosIniciais[i]->itsFinal){
                tempS.itsFinal = true;
            }
            if (i + 1 < l)
            {
                tempS.label = tempS.label + ",";
            }
        }
        tempS.label = tempS.label + "}";

        //Para cada caracter, para cada estado inicial, adiciona a sua transição equivalente a uma 
        //transição temporaria
        for (i = 0; i < n; i++)
        {
            tempT.character = alfabeto[i];
            for (j = 0; j < l; j++)
            {
                y = estadosIniciais[j]->delta[i].to.size();
                for (k = 0; k < y; k++)
                {
                    //Se ja foi registrado que esse estado pode ser alcançado, não registra ele novamente
                    if (!existeElemConjunto(tempT.to, estadosIniciais[j]->delta[i].to[k]))
                    {
                        tempT.to.push_back(estadosIniciais[j]->delta[i].to[k]);
                    }
                }
            }
            tempS.delta.push_back(tempT);
            tempT.to.clear();
        }
        //Adiciona o estado inicial ao AFD
        AFD.estados.push_back(tempS);
        AFD.estadosIniciais.push_back(&AFD.estados[0]);
    
        j = 0;
        z = 0;
        //A partir do estado inicial, transforma as transições dele em estados
        //adiciona eles no com suas respectivas tansições no AFD, transformando essas transições em estados tambem
        //Ate que nenhuma transição nova seja inserida
        while(j < (k = AFD.estados.size())){
            for(i = 0; i < n; i++){
                tempS = transicaoParaEstado(AFD.estados[j].delta[i]);
                if(tempS.label != "{}" && !verificaSeEstadoFoiInserido(AFD.estados, tempS)){
                    AFD.estados.push_back(tempS);
                    z++;
                    if(tempS.itsFinal){
                        AFD.estadosFinais.push_back(&AFD.estados[z]);
                    }
                }
            }
            j++;
        }
        //Atribui o alfabeto ao AFD
        AFD.alfabeto = alfabeto;
    }
    //Mostra os elementos do alfabeto
    void mostrarAlfabeto(){
        int i, n = alfabeto.size();
        
        for(i = 0; i < n; i++){
            cout << alfabeto[i];
            if(i + 1 < n){
                cout << ",";
            }
        }
        cout << endl;
    }
    //Mostra os estados
    void mostrarEstados(){
        int i, n = estados.size();
        
        for(i = 0; i < n; i++){
            cout << estados[i].label;
            if(i + 1 < n){
                cout << ",";
            }
        }
        cout << endl;
    }
    //Mostra os estados iniciais
    void mostrarEstadosIniciais(){
        int i, n = estadosIniciais.size();
        for(i = 0; i < n; i++){
            cout << estadosIniciais[i]->label;
            if(i + 1 < n){
                cout << ",";
            }
        }
        cout << endl;
    }
//Mostra os estados finais
    void mostrarEstadosFinais(){
        int i, n = estadosFinais.size();
        for(i = 0; i < n; i++){
            cout << estadosFinais[i]->label;
            if(i + 1 < n){
                cout << ",";
            }
        }
        cout << endl;
    }
    //Mostra a tabela de transição
    void mostrarTabelaTransicao(){
        int i, j, m = estados.size(), n = alfabeto.size();
        State tempS;
        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                //Transforma a transição em estado para acessar o seu nome
                tempS = transicaoParaEstado(estados[i].delta[j]);
                if(estados[i].itsStart){
                    cout << "->";
                }
                if(estados[i].itsFinal){
                    cout << "*";
                }
                cout << "(" << estados[i].label << "," << alfabeto[j] << ") = " << tempS.label << endl;  
            }
        }
    }

    void clear(){
        alfabeto.clear();
        estadosIniciais.clear();
        estadosFinais.clear();
        estados.clear();
    }
};

int main()
{

    Automata afn, afd;
    int flag;

    do{
        cout << "#################### Conversor de AFN pra AFD ####################" << endl;
        cout << "Insira o conjunto de estados: " << endl;
        afn.lerEstados();
        cout << "Insira o conjunto de estados iniciais: " << endl;
        afn.lerEstadosIniciais();
        cout << "Insira o conjunto de estados finais: " << endl;
        afn.lerEstadosFinais();
        cout << "Insira o conjunto de símbolos do alfabeto: " << endl;
        afn.lerAlfabeto();
        cout << "#################### Funcao de Transicao ####################" << endl;
        afn.lerFuncaoTransicao();

        afn.converteParaAFD(afd);

        cout << "#################### AFD ####################" << endl;
        cout << "Estados:" << endl;
        afd.mostrarEstados();
        cout << "Alfabeto:" << endl;
        afd.mostrarAlfabeto();
        cout << "Tabela de Transicao:" << endl;
        afd.mostrarTabelaTransicao();

        cout << "Deseja inserir um novo automato?\n[0] - Nao\n[1] - Sim\n########################################" << endl;
        cin >> flag;
        if(!flag){
            return 0;
        }
        afn.clear();
        afd.clear();
        cin.ignore();
        system("clear");
        }while(true);   

    return 0;
}