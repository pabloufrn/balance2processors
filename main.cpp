#include <iostream>
#include <vector>
#include <fstream>
#include <utility> 
#include <tuple>
#include <unordered_set>
#include <queue>

#define VERTICE_FONTE 0 
#define VERTICE_SUMIDOURO 1

struct Aresta
{
	int capacidade;
	int fluxo;
};

struct Grafo {
	size_t ordem;
	size_t tamanho;
	Aresta** matrizAdjacencia;	
};

// ========================= Métodos de grafos


Grafo* construirGrafo( size_t ordem) {
	Grafo* G = new Grafo{ordem, 0};
	G->matrizAdjacencia = new Aresta*[ordem];
	for (int i = 0; i < ordem; ++i) {
		G->matrizAdjacencia[i] = new Aresta[ordem];
		for (int j = 0; j < ordem; ++j){
			auto aresta = G->matrizAdjacencia[i][j];
			aresta.fluxo = 0;
			aresta.capacidade = 0;
		}
	}
	return G;
}
/**/
void destruirGrafo( Grafo* G) {
	auto matriz = G->matrizAdjacencia;
	for (int i = 0; i < G->ordem; ++i) {
		delete matriz[i];
	}
	delete matriz;
	delete G;
}
/**/
void incluirAresta( Grafo* G, int v, int w, int capacidade) {
	G->matrizAdjacencia[v][w].capacidade = capacidade;
}
/**/
void imprimirGrafo( Grafo* G) {
	auto matriz = G->matrizAdjacencia;
	std::cout << "Ordem: " << G->ordem << std::endl;
	std::cout << "Tamanho: " << G->tamanho << std::endl;	
	std::cout << "Arestas da fonte: ";
	for ( int i = 0; i < G->ordem; ++i) {
		if(matriz[VERTICE_FONTE][i].capacidade != 0) {
			std::cout << "(" << VERTICE_FONTE << ", " << i << ") ";
		}
	}
	std::cout << std::endl;
	std::cout << "Arestas para o sumidouro: ";
	for ( int i = 0; i < G->ordem; ++i) {
		if(matriz[i][VERTICE_SUMIDOURO].capacidade != 0) {
			std::cout << "(" << i << ", " << VERTICE_SUMIDOURO << ") ";
		}
	}
	std::cout << std::endl;
	std::cout << "Demais arestas: ";
	for ( int i = 2; i < G->ordem; ++i) {
		for ( int j = 2; j < G->ordem; ++j) {
			if(matriz[i][j].capacidade != 0) {
				std::cout << "(" << i << ", " << j << ") ";
			}
		}
	}
	std::cout << std::endl;
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
		incluirAresta(G, VERTICE_FONTE, v, 10);
		incluirAresta(G, v, VERTICE_SUMIDOURO, 10);
	}
	return G;
}

void carregarLinksDoArquivo(Grafo* G, std::string caminhoArquivo){
	std::ifstream arquivo(caminhoArquivo);
	std::string u, v, capacidade;
	std::vector<std::tuple<int, int, int>> links;
	if(!arquivo.good()){
		std::cerr << "arquivo invalido" << std::endl;
		std::exit(0);
	}
	while(arquivo.good() and (arquivo.peek() != -1)){
		getline(arquivo, u, ',');
		getline(arquivo, v, ',');
		getline(arquivo, capacidade);
		links.push_back(std::make_tuple(std::stoi(u), std::stoi(v), std::stoi(capacidade)));
	}
	int size = links.size();
	for ( int l = 0; l < size; ++l){
		auto link = links[l];
		incluirAresta(G, std::get<0>(link), std::get<1>(link), std::get<2>(link));
		// incluirAresta(G, std::get<1>(link), std::get<0>(link), std::get<2>(link));
	}
}
/**/
// ========================= Cálculos e funções auxiliares

void processadorUmPrimeiro(Grafo* G){
	auto total = 0u;
	auto matriz = G->matrizAdjacencia;
	std::unordered_set<int> processadorUm;
	std::vector<int> processadorDois;
	for ( int v = 0; v < G->ordem; ++v) {
		auto tempo = matriz[VERTICE_FONTE][v].capacidade;
		if(tempo == -1){
			tempo = matriz[v][VERTICE_SUMIDOURO].capacidade;
			processadorDois.push_back(v);
		} else {
			processadorUm.insert(v); 
		}
		total += tempo;
	}
	for ( auto& u: processadorDois){
		for ( int v = 2; v < G->ordem; ++v) {
			total += matriz[u][v].capacidade + matriz[v][u].capacidade;
		}
	}
	std::cout << "Alocar todos os módulos possíveis em P1: " << 
		total << " unidades de tempo." << std::endl;
}
void processadorDoisPrimeiro(Grafo* G){
	auto total = 0u;
	auto matriz = G->matrizAdjacencia;
	std::unordered_set<int> processadorDois;
	std::vector<int> processadorUm;
	for ( int v = 0; v < G->ordem; ++v) {
		auto tempo = matriz[v][VERTICE_SUMIDOURO].capacidade;
		if(tempo == -1){
			tempo = matriz[VERTICE_FONTE][v].capacidade;
			processadorUm.push_back(v);
		} else {
			processadorDois.insert(v); 
		}
		total += tempo;
	}
	for ( auto& u: processadorUm){
		for ( int v = 2; v < G->ordem; ++v) {
			total += matriz[u][v].capacidade + matriz[v][u].capacidade;
		}
	}
	std::cout << "Alocar todos os módulos possíveis em P1: " << 
		total << " unidades de tempo." << std::endl;
}
/**/
// ========================= Algoritmos de fluxo em redes
// Ao executar a BFS e passar o fluxo para um nó, atribua uma direção (opcional)


int buscaLargura( Grafo* G, std::vector<int>& pai) {
	auto matriz = G->matrizAdjacencia;
    int u, v;
    int fluxoAumento = -1;
    // pai 
    std::fill(pai.begin(), pai.end(), -1);
    pai[VERTICE_FONTE] = -2;
    //  fila da busca em largura
    std::queue<int> fila;
    fila.push(VERTICE_FONTE);
    while(!fila.empty()){ 
    	u = fila.front();
    	fila.pop();
    	// v = vertice v na aresta (u, v)
    	for ( int v = 0; v < G->ordem; ++v) {
    		if(matriz[u][v].capacidade - matriz[u][v].fluxo > 0 and pai[v] == -1){
    			pai[v] = u;
    			if(fluxoAumento == -1)
    				fluxoAumento = matriz[u][v].capacidade - matriz[u][v].fluxo;
    			else 
    				fluxoAumento = std::min(fluxoAumento, matriz[u][v].capacidade - matriz[u][v].fluxo);
    			if(v == VERTICE_SUMIDOURO)
    				return fluxoAumento;
    			fila.push(v); 
    		}
    	}
    }
    // não encontrou o sumidouro
    return 0;
}


int fluxoMaximo( Grafo* G) {
	auto matriz = G->matrizAdjacencia;
	// vetor para backtracking
	std::vector<int> pai(G->ordem);
	// vertices para auxilir o backtracking
	int u, v;
	// variaveis
    int fluxoMaximo = 0;
    int fluxoAumento = 0;
    while (fluxoAumento = buscaLargura(G, pai)) {
        fluxoMaximo += fluxoAumento;
        v = pai[VERTICE_SUMIDOURO];
        // fonte
        while (v != VERTICE_FONTE) {
        	u = pai[v];
        	G->matrizAdjacencia[u][v].fluxo -= fluxoAumento;
        	G->matrizAdjacencia[v][u].fluxo += fluxoAumento;
            v = u;
        }
        // std::cout << "Atual: (" << u << ", " << v << ")\n";
    }
    return fluxoMaximo;
}

int main( int argv, char* args[]) {
	Grafo* G = carregarDoArquivo("gendata/casos_de_teste/casoquatro_0.csv");
	carregarLinksDoArquivo(G, "gendata/casos_de_teste/casoquatro_0_links_4_0.csv");
	imprimirGrafo(G);
	processadorUmPrimeiro(G);
	processadorDoisPrimeiro(G);
	std::cout << "Fluxo máximo: " << fluxoMaximo(G) << std::endl;
	destruirGrafo(G);
	return 0;
}
