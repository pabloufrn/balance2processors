#include <iostream>
#include <vector>
#include <fstream>
#include <pair>

struct Entrada;

struct Vertice {
	int fluxo;
	int capacidade;
	Vertice* proximo;
	int rotulo;
};

struct Grafo {
	int cardinalidade;
	int comprimento;
	Vertice* listaAdjacencia;	
};

// ========================= Métodos de grafos

Grafo* construirGrafo( int cardinalidade) {
	Grafo* G = new Grafo{cardinalidade};
	G->comprimento = 0;
	G->listaAdjacencia = new Vertice[cardinalidade];
	for (int i = 0; i < cardinalidade; ++i) {
		G->listaAdjacencia[i].fluxo = 0;
		G->listaAdjacencia[i].proximo = nullptr;
		G->listaAdjacencia[i].rotulo = i;
	}
	return G;
}
/**/
void destruirGrafo( Grafo* G) {
	Vertice* lista = G->listaAdjacencia;
	for (int i = 0; i < G->cardinalidade; ++i) {
		Vertice* anterior = lista[i].proximo;
		Vertice* atual = anterior->proximo;
		while (atual != nullptr) {
			delete anterior;
			anterior = atual;
		}
	}
	delete lista;
	delete G;
}
/**/
void incluirAresta( Grafo* G, int v, int w, int capacidade) {
	Vertice* lista = G->listaAdjacencia;
	Vertice* verticeV = &lista[v];
	Vertice* novo = new Vertice();
	novo->proximo = verticeV->proximo;
	verticeV->proximo = novo;
	novo->fluxo = 0;
	novo->capacidade = capacidade;
	novo->rotulo = w;
	G->comprimento++;
}
/**/
void imprimirGrafo( Grafo* G) {
	Vertice* lista = G->listaAdjacencia;
	for (int i = 0; i < G->cardinalidade; ++i) {
		Vertice* v = &lista[i];
		std::cout << v->rotulo << " -> ";
		v = v->proximo;
		while (v != nullptr) {
			std::cout << v->rotulo << "(" << v->fluxo << ") ";
			v = v->proximo;
		}
		std::cout << std::endl;
	}
}
/**/

// ========================= Leitura de arquivo

Grafo* carregarDoArquivo(std::string nomeArquivo) {
	std::ifstream arquivo(nomeArquivo);
	std::string linha, processadorUm, processadorDois;
	std::vector<std::pair<std::int>> tempos;
	while(arquivo.good()){
		getline(file, processadorUm, ',');
		getline(file, processadorDois);
		tempos.push_back(processadorUm);
		tempos.push_back(processadorDois);
	}
	int cardinalidade = ((int) tempos.size() / 2) + 2;
	construirGrafo(cardinalidade);
	// fonte = 0
	// sumidouro = 1
	for (int v = 2; v < G->cardinalidade-2; ++v){
		incluirAresta(G, 0, v, tempos[i-2].first);
		incluirAresta(G, v, 1, tempos[i-2].second);
	}
}



int main( int argv, char* args[]) {
	Grafo* G = construirGrafo(4);
	imprimirGrafo(G);
	incluirAresta(G, 1, 2, 1);
	incluirAresta(G, 2, 1, 1);
	imprimirGrafo(G);
	destruirGrafo(G);
	// Vertice va = Vertice{1};
	// imprimirGrafo(*G);
	return 0;
}
