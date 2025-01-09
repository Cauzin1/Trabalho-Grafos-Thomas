#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <queue>

GrafoMatriz::GrafoMatriz(int vertices, bool dir, bool pVert, bool pArest) {
    nVertices = vertices;
    direcionado = dir;
    ponderadoVertices = pVert;
    ponderadoArestas = pArest;
    matrizAdj.resize(vertices, std::vector<int>(vertices, 0));
}

bool GrafoMatriz::ehBipartido() {
    std::vector<int> cor(nVertices, -1);

    std::function<bool(int, int)> bfs = [&](int src, int c) {
        std::queue<int> q;
        q.push(src);
        cor[src] = c;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < nVertices; ++v) {
                if (matrizAdj[u][v] != 0) {
                    if (cor[v] == -1) {
                        cor[v] = 1 - cor[u];
                        q.push(v);
                    } else if (cor[v] == cor[u]) {
                        return false;
                    }
                }
            }
        }
        return true;
    };

    for (int i = 0; i < nVertices; ++i) {
        if (cor[i] == -1) {
            if (!bfs(i, 0)) {
                return false;
            }
        }
    }

    return true;
}

int GrafoMatriz::nConexo() {
    std::vector<bool> visitado(nVertices, false);
    int componentesConexas = 0;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        for (int v = 0; v < nVertices; ++v) {
            if (matrizAdj[u][v] != 0 && !visitado[v]) {
                dfs(v);
            }
        }
    };

    for (int i = 0; i < nVertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
            componentesConexas++;
        }
    }

    return componentesConexas;
}

int GrafoMatriz::getGrau() {
    int maxGrau = 0;
    for (int i = 0; i < nVertices; ++i) {
        int grau = 0;
        for (int j = 0; j < nVertices; ++j) {
            if (matrizAdj[i][j] != 0) {
                grau++;
            }
        }
        if (grau > maxGrau) {
            maxGrau = grau;
        }
    }
    return maxGrau;
    
}

bool GrafoMatriz::ehCompleto() {
    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            if (i != j && matrizAdj[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool GrafoMatriz::ehArvore() {
    // Verifica se o grafo é conexo
    std::vector<bool> visitado(nVertices, false);
    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        for (int v = 0; v < nVertices; ++v) {
            if (matrizAdj[u][v] != 0 && !visitado[v]) {
                dfs(v);
            }
        }
    };

    dfs(0);
    for (bool v : visitado) {
        if (!v) return false; // Se algum vértice não foi visitado, o grafo não é conexo
    }

    // Verifica se o grafo tem nVertices - 1 arestas
    int numArestas = 0;
    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            if (matrizAdj[i][j] != 0) {
                numArestas++;
            }
        }
    }
    numArestas /= 2; // Cada aresta foi contada duas vezes

    return numArestas == nVertices - 1;
}

    bool GrafoMatriz::possuiArticulacao() {
        std::vector<bool> visitado(nVertices, false);
        std::vector<int> disc(nVertices, -1);
        std::vector<int> low(nVertices, -1);
        std::vector<int> parent(nVertices, -1);
        bool possuiArtic = false;

        std::function<void(int)> dfs = [&](int u) {
            static int tempo = 0;
            visitado[u] = true;
            disc[u] = low[u] = ++tempo;
            int filhos = 0;

            for (int v = 0; v < nVertices; ++v) {
                if (matrizAdj[u][v] != 0) {
                    if (!visitado[v]) {
                        filhos++;
                        parent[v] = u;
                        dfs(v);

                        low[u] = std::min(low[u], low[v]);

                        if (parent[u] == -1 && filhos > 1)
                            possuiArtic = true;

                        if (parent[u] != -1 && low[v] >= disc[u])
                            possuiArtic = true;
                    } else if (v != parent[u]) {
                        low[u] = std::min(low[u], disc[v]);
                    }
                }
            }
        };

        for (int i = 0; i < nVertices; ++i) {
            if (!visitado[i]) {
                dfs(i);
            }
        }

        return possuiArtic;
    }


bool GrafoMatriz::possuiPonte() {
    std::vector<bool> visitado(nVertices, false);
    std::vector<int> disc(nVertices, -1);
    std::vector<int> low(nVertices, -1);
    std::vector<int> parent(nVertices, -1);
    bool possuiPonte = false;

    std::function<void(int)> dfs = [&](int u) {
        static int tempo = 0;
        visitado[u] = true;
        disc[u] = low[u] = ++tempo;

        for (int v = 0; v < nVertices; ++v) {
            if (matrizAdj[u][v] != 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    dfs(v);

                    low[u] = std::min(low[u], low[v]);

                    if (low[v] > disc[u])
                        possuiPonte = true;
                } else if (v != parent[u]) {
                    low[u] = std::min(low[u], disc[v]);
                }
            }
        }
    };

    for (int i = 0; i < nVertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
        }
    }

    return possuiPonte;
}

void GrafoMatriz::carregaGrafo(const std::string &arquivo) {
    std::ifstream file(arquivo);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << arquivo << std::endl;
        return;
    }

    file >> nVertices;
    matrizAdj.resize(nVertices, std::vector<int>(nVertices, 0));

    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            file >> matrizAdj[i][j];
        }
    }

    file.close();
}

void GrafoMatriz::novoGrafo(const std::string &descricao) {

    std::cout << "Criando um novo grafo: " << descricao << std::endl;
    std::cout << "Digite o número de vertices: ";
    std::cin >> nVertices;

    matrizAdj.resize(nVertices, std::vector<int>(nVertices, 0));

    std::cout << "Digite a matriz de adjacencia:" << std::endl;
    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            std::cin >> matrizAdj[i][j];
        }
    }
}

GrafoLista::GrafoLista(int vertices, bool dir, bool pVert, bool pArest) {
    nVertices = vertices;
    direcionado = dir;
    ponderadoVertices = pVert;
    ponderadoArestas = pArest;
    listaAdj.resize(vertices, nullptr);
}

GrafoLista::~GrafoLista() {
    for (auto no : listaAdj) {
        while (no != nullptr) {
            No *temp = no;
            no = no->proximo;
            delete temp;
        }
    }
}

bool GrafoLista::ehBipartido() {
    std::vector<int> cor(nVertices, -1);

    std::function<bool(int, int)> bfs = [&](int src, int c) {
        std::queue<int> q;
        q.push(src);
        cor[src] = c;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (No* no = listaAdj[u]; no != nullptr; no = no->proximo) {
                int v = no->vertice;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    q.push(v);
                } else if (cor[v] == cor[u]) {
                    return false;
                }
            }
        }
        return true;
    };

    for (int i = 0; i < nVertices; ++i) {
        if (cor[i] == -1) {
            if (!bfs(i, 0)) {
                return false;
            }
        }
    }

    return true;
    
}

int GrafoLista::nConexo() {
    std::vector<bool> visitado(nVertices, false);
    int componentesConexas = 0;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        for (No* no = listaAdj[u]; no != nullptr; no = no->proximo) {
            int v = no->vertice;
            if (!visitado[v]) {
                dfs(v);
            }
        }
    };

    for (int i = 0; i < nVertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
            componentesConexas++;
        }
    }

    return componentesConexas;
}

int GrafoLista::getGrau() {
    int maxGrau = 0;
    for (int i = 0; i < nVertices; ++i) {
        int grau = 0;
        for (No* no = listaAdj[i]; no != nullptr; no = no->proximo) {
            grau++;
        }
        if (grau > maxGrau) {
            maxGrau = grau;
        }
    }
    return maxGrau;
    
}

bool GrafoLista::ehCompleto() {
    for (int i = 0; i < nVertices; ++i) {
        int grau = 0;
        for (No* no = listaAdj[i]; no != nullptr; no = no->proximo) {
            grau++;
        }
        if (grau != nVertices - 1) {
            return false;
        }
    }
    return true;
}

bool GrafoLista::ehArvore() {
    
    std::vector<bool> visitado(nVertices, false);
    std::vector<int> parent(nVertices, -1);
    bool temCiclo = false;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        for (No* no = listaAdj[u]; no != nullptr; no = no->proximo) {
            int v = no->vertice;
            if (!visitado[v]) {
                parent[v] = u;
                dfs(v);
            } else if (v != parent[u]) {
                temCiclo = true;
            }
        }
    };

    dfs(0);
    for (bool v : visitado) {
        if (!v) return false; // Se algum vértice não foi visitado, o grafo não é conexo
    }

    return !temCiclo;
}

bool GrafoLista::possuiArticulacao() {
    
    std::vector<bool> visitado(nVertices, false);
    std::vector<int> disc(nVertices, -1);
    std::vector<int> low(nVertices, -1);
    std::vector<int> parent(nVertices, -1);
    bool possuiArtic = false;

    std::function<void(int)> dfs = [&](int u) {
        static int tempo = 0;
        visitado[u] = true;
        disc[u] = low[u] = ++tempo;
        int filhos = 0;

        for (No* no = listaAdj[u]; no != nullptr; no = no->proximo) {
            int v = no->vertice;
            if (!visitado[v]) {
                filhos++;
                parent[v] = u;
                dfs(v);

                low[u] = std::min(low[u], low[v]);

                if (parent[u] == -1 && filhos > 1)
                    possuiArtic = true;

                if (parent[u] != -1 && low[v] >= disc[u])
                    possuiArtic = true;
            } else if (v != parent[u]) {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < nVertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
        }
    }

    return possuiArtic;
}

bool GrafoLista::possuiPonte() {
    
    std::vector<bool> visitado(nVertices, false);
    std::vector<int> disc(nVertices, -1);
    std::vector<int> low(nVertices, -1);
    std::vector<int> parent(nVertices, -1);
    bool possuiPonte = false;

    std::function<void(int)> dfs = [&](int u) {
        static int tempo = 0;
        visitado[u] = true;
        disc[u] = low[u] = ++tempo;

        for (No* no = listaAdj[u]; no != nullptr; no = no->proximo) {
            int v = no->vertice;
            if (!visitado[v]) {
                parent[v] = u;
                dfs(v);

                low[u] = std::min(low[u], low[v]);

                if (low[v] > disc[u])
                    possuiPonte = true;
            } else if (v != parent[u]) {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < nVertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
        }
    }

    return possuiPonte;
}

void GrafoLista::carregaGrafo(const std::string &arquivo) {
    std::ifstream file(arquivo);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << arquivo << std::endl;
        return;
    }

    file >> nVertices;
    listaAdj.resize(nVertices, nullptr);

    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            int peso;
            file >> peso;
            if (peso != 0) {
                No* novoNo = new No{j, peso, listaAdj[i]};
                listaAdj[i] = novoNo;
            }
        }
    }

    file.close();
}

void GrafoLista::novoGrafo(const std::string &descricao) {
    std::cout << "Criando um novo grafo: " << descricao << std::endl;
    std::cout << "Digite o número de vertices: ";
    std::cin >> nVertices;

    listaAdj.resize(nVertices, nullptr);

    std::cout << "Digite a matriz de adjacencia:" << std::endl;
    for (int i = 0; i < nVertices; ++i) {
        for (int j = 0; j < nVertices; ++j) {
            int peso;
            std::cin >> peso;
            if (peso != 0) {
                No* novoNo = new No{j, peso, listaAdj[i]};
                listaAdj[i] = novoNo;
            }
        }
    }
}

int Grafo::getOrdem() {
    return nVertices;
}

bool Grafo::ehDirecionado() {
    return direcionado;
}

bool Grafo::verticePonderado() {
    return ponderadoVertices;
}

bool Grafo::arestaPonderada() {
    return ponderadoArestas;
}
