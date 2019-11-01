#include <iostream>
#include <vector>
#include <fstream>
#include <utility> 
#include <tuple>
#include <unordered_set>


struct Vertice {
	int fluxo;
	int capacidade;
	Vertice* proximo;
	int rotulo;
};

struct Grafo {
	int ordem;
	int tamanho;
	Vertice* listaAdjacencia;	
};

// ========================= Métodos de grafos

Grafo* construirGrafo( int ordem) {
	Grafo* G = new Grafo{ordem};
	G->tamanho = 0;
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
		Vertice* atual = lista[i].proximo;
		Vertice* proximo = nullptr;
		while (atual != nullptr) {
			proximo = atual->proximo;
			delete atual;
			atual = proximo;
		}
	}
	delete lista;
	delete G;
}
/**/
void incluirAresta( Grafo* G, int v, int w, int capacidade, int offset = 0) {
	Vertice* lista = G->listaAdjacencia;
	Vertice* verticeV = &lista[v];
	Vertice* novo = new Vertice();
	for (int i = 0; i < offset; ++i){
		verticeV = verticeV->proximo;
	}
	novo->proximo = verticeV->proximo;
	verticeV->proximo = novo;
	novo->fluxo = 0;
	novo->capacidade = capacidade;
	novo->rotulo = w;
	G->tamanho++;
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
		links.push_back(std::make_tuple(std::stoi(u), std::stoi(v), std::stoi(peso)));
	}
	int size = links.size();
	for ( int l = 0; l < size; ++l){
		auto link = links[l];
		incluirAresta(G, std::get<0>(link), std::get<1>(link), std::get<2>(link), 1);
		incluirAresta(G, std::get<1>(link), std::get<0>(link), std::get<2>(link), 1);
	}
}
void processadorUmPrimeiro(Grafo* G){
	auto total = 0u;
	Vertice* lista = G->listaAdjacencia;
	Vertice* v = lista->proximo;
	std::unordered_set<int> processadorUm;
	std::vector<int> processadorDois;
	while(v != nullptr){
		auto tempo = v->capacidade;
		if(tempo == -1){
			tempo = lista[v->rotulo].proximo->capacidade;
			processadorDois.push_back(v->rotulo);
		} else {
			processadorUm.insert(v->rotulo); 
		}
		total += tempo;
		v = v->proximo;
	}
	for ( auto& u: processadorDois){
		v = lista[u].proximo->proximo;
		while(v != nullptr){
			if(processadorUm.count(v->rotulo) > 0)
				total += v->capacidade;
			v = v->proximo;
		}
	}
	std::cout << "Alocar todos os módulos possíveis em P1: " << 
		total << " unidades de tempo." << std::endl;
}
void processadorDoisPrimeiro(Grafo* G){
	auto total = 0u;
	Vertice* lista = G->listaAdjacencia;
	Vertice* v = nullptr;
	std::vector<int> processadorUm;
	std::unordered_set<int> processadorDois;
	for (int i = 2; i < G->ordem; ++i) {
		v = &lista[i];
		auto tempo = v->proximo->capacidade;
		if(tempo == -1){
			v = lista[0].proximo;
			while(v->rotulo != i){
				v = v->proximo;
			}
			tempo = v->capacidade;
			processadorUm.push_back(v->rotulo);
		} else {
			processadorDois.insert(v->rotulo);
		}
		total += tempo;
	}
	for ( auto& u: processadorUm){
		v = lista[u].proximo->proximo;
		while(v != nullptr){
			if(processadorDois.count(v->rotulo) > 0)
				total += v->capacidade;
			v = v->proximo;
		}
	}
	std::cout << "Alocar todos os módulos possíveis em P2: " << 
		total << " unidades de tempo." << std::endl;
}

/* todo: fazer o algoritmo de fluxo máximo e rodar
*/

int main( int argv, char* args[]) {
	Grafo* G = carregarDoArquivo("gendata/casos_de_teste/casoquatro_0.csv");
	carregarLinksDoArquivo(G, "gendata/casos_de_teste/casoquatro_0_links_4_0.csv");
	imprimirGrafo(G);
	processadorUmPrimeiro(G);
	processadorDoisPrimeiro(G);
	destruirGrafo(G);
	return 0;
}
