// Implementação do Trabalho 1 - Teoria dos Grafos

#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

class Grafo {
protected:
    int nVertices;
    bool direcionado;
    bool ponderadoVertices;
    bool ponderadoArestas;

public:
    virtual ~Grafo() {}

    virtual bool ehBipartido() = 0;
    virtual int nConexo() = 0;
    virtual int getGrau() = 0;
    virtual int getOrdem();
    virtual bool ehDirecionado();
    virtual bool verticePonderado();
    virtual bool arestaPonderada();
    virtual bool ehCompleto() = 0;
    virtual bool ehArvore() = 0;
    virtual bool possuiArticulacao() = 0;
    virtual bool possuiPonte() = 0;
    virtual void carregaGrafo(const std::string &arquivo) = 0;
    virtual void novoGrafo(const std::string &descricao) = 0;
};

class GrafoMatriz : public Grafo {
private:
    std::vector<std::vector<int>> matrizAdj;

public:
    GrafoMatriz(int vertices, bool dir, bool pVert, bool pArest);
    bool ehBipartido() override;
    int nConexo() override;
    int getGrau() override;
    bool ehCompleto() override;
    bool ehArvore() override;
    bool possuiArticulacao() override;
    bool possuiPonte() override;
    void carregaGrafo(const std::string &arquivo) override;
    void novoGrafo(const std::string &descricao) override;
};

class GrafoLista : public Grafo {
private:
    struct No {
        int destino;
        int peso;
        No *proximo;
        int vertice;
    };

    std::vector<No *> listaAdj;

public:
    GrafoLista(int vertices, bool dir, bool pVert, bool pArest);
    ~GrafoLista();
    bool ehBipartido() override;
    int nConexo() override;
    int getGrau() override;
    bool ehCompleto() override;
    bool ehArvore() override;
    bool possuiArticulacao() override;
    bool possuiPonte() override;
    void carregaGrafo(const std::string &arquivo) override;
    void novoGrafo(const std::string &descricao) override;
};

#endif // GRAFO_H
