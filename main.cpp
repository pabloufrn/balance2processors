#include <iostream>
#include <vector>
#include <fstream>
#include <utility> 

struct Entrada;

struct Vertice {
	int fluxo;
	int capacidade;
	Vertice* proximo;
	int rotulo;
};

struct Grafo {
	int ordem;
	int temanho;
	Vertice* listaAdjacencia;	
};

// ========================= Métodos de grafos

Grafo* construirGrafo( int ordem) {
	Grafo* G = new Grafo{ordem};
	G->temanho = 0;
	G->listaAdjacencia = new Vertice[ordem];
	for (int i = 0; i < ordem; ++i) {
		G->listaAdjacencia[i].fluxo = 0;
		G->listaAdjacencia[i].proximo = nullptr;
		G->listaAdjacencia[i].rotulo = i;
	}
	return G;
}
/**/
void destruirGrafo( Grafo* G) {
	Vertice* lista = G->listaAdjacencia;
	for (int i = 0; i < G->ordem; ++i) {
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
	G->temanho++;
}
/**/
void imprimirGrafo( Grafo* G) {
	Vertice* lista = G->listaAdjacencia;
	for (int i = 0; i < G->ordem; ++i) {
		Vertice* v = &lista[i];
		std::cout << v->rotulo << " -> ";
		v = v->proximo;
		while (v != nullptr) {
			std::cout << v->rotulo << "(" << v->capacidade << ") ";
			v = v->proximo;
		}
		std::cout << std::endl;
	}
}
/**/

// ========================= Leitura de arquivo

Grafo* carregarDoArquivo(std::string caminhoArquivo) {
	// Carregar arestas processadores
	std::ifstream arquivo(caminhoArquivo);
	std::string processadorUm, processadorDois;
	std::vector<std::pair<int, int>> tempos;
	if(!arquivo.good()){
		std::cerr << "arquivo invalido" << std::endl;
		std::exit(0);
	}
	while(arquivo.good() and (arquivo.peek() != -1)){
		getline(arquivo, processadorUm, ',');
		getline(arquivo, processadorDois);
		tempos.push_back(std::make_pair<int, int>(std::stoi(processadorUm), std::stoi(processadorDois)));
	}
	int ordem = tempos.size() + 2;
	auto G = construirGrafo(ordem);
	for ( int v = 2; v < G->ordem; ++v){
		incluirAresta(G, 0, v, tempos[v-2].first);
		incluirAresta(G, v, 1, tempos[v-2].second);
	}
	return G;
}
void carregarLinksDoArquivo(Grafo* G, std::string caminhoArquivo){
	std::ifstream arquivo(caminhoArquivo);
	std::string u, v, peso;
	std::vector<std::tuple<int, int, int>> links;
	if(!arquivo.good()){
		std::cerr << "arquivo invalido" << std::endl;
		std::exit(0);
	}
	while(arquivo.good() and (arquivo.peek() != -1)){
		getline(arquivo, u, ',');
		getline(arquivo, v, ',');
		getline(arquivo, peso);
		links.push_back(std::make_tuple<int, int>(std::stoi(u), std::stoi(v), std::stoi(peso)));
	}
	int ordem = tempos.size() + 2;
	auto G = construirGrafo(ordem);
	for ( int v = 2; v < G->ordem; ++v){
		incluirAresta(G, 0, v, tempos[v-2].first);
		incluirAresta(G, v, 1, tempos[v-2].second);
	}
	return G;
}





int main( int argv, char* args[]) {
	Grafo* G = carregarDoArquivo("gendata/casos_de_teste/casoquatro_0.csv");
	carregarLinksDoArquivo(G, "gendata/casos_de_teste/casoquatro_0_links_4_0.csv")
	imprimirGrafo(G);
	// destruirGrafo(G);
	return 0;
}
