#include "Grafo.h"
#include <iostream>

int main() {
    // Criando um grafo usando matriz de adjacência
    GrafoMatriz grafoMatriz(5, true, false, true);
    std::cout << "Ordem do grafo (Matriz): " << grafoMatriz.getOrdem() << std::endl;

    // Testando algumas funcionalidades em GrafoMatriz
    std::cout << "É bipartido (Matriz)? " << (grafoMatriz.ehBipartido() ? "Sim" : "Não") << std::endl;

    // Criando um grafo usando lista encadeada
    GrafoLista grafoLista(5, false, true, false);
    std::cout << "Ordem do grafo (Lista): " << grafoLista.getOrdem() << std::endl;

    // Testando algumas funcionalidades em GrafoLista
    std::cout << "Número de componentes conexas (Lista): " << grafoLista.nConexo() << std::endl;

    // Testando a leitura de um grafo de arquivo
    grafoMatriz.carregaGrafo("grafo.txt");
    std::cout << "Número de componentes conexas após carregar (Matriz): " << grafoMatriz.nConexo() << std::endl;

    // Inserindo um grafo manualmente
    grafoLista.novoGrafo("Grafo Criado Manualmente");
    std::cout << "Grau máximo do grafo (Lista): " << grafoLista.getGrau() << std::endl;

    return 0;
}
