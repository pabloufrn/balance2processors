#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <tuple>
#include <unordered_set>
#include <queue>
#include <iterator>
#include <cassert>

#define VERTICE_FONTE 0
#define VERTICE_SUMIDOURO 1
#define INFINITO 999999999

struct Aresta {
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
	G->tamanho += 1;
}
/**/
void imprimirGrafo( Grafo* G) {
	auto matriz = G->matrizAdjacencia;
	std::cout << "Ordem: " << G->ordem << std::endl;
	std::cout << "Tamanho: " << G->tamanho << std::endl;
	std::cout << "Arestas da fonte: ";
	for ( int i = 2; i < G->ordem; ++i) {
		std::cout 	<< "(" << VERTICE_FONTE << ", " << i << ")["
					<< (matriz[VERTICE_FONTE][i].capacidade == INFINITO
					? -1 : matriz[VERTICE_FONTE][i].capacidade) << "] ";
	}
	std::cout << std::endl;
	std::cout << "Arestas para o sumidouro: ";
	for ( int i = 2; i < G->ordem; ++i) {
		std::cout << "(" << i << ", " << VERTICE_SUMIDOURO << ")["
		<< ( matriz[i][VERTICE_SUMIDOURO].capacidade == INFINITO
		? -1 : matriz[i][VERTICE_SUMIDOURO].capacidade ) << "] ";
	}
	std::cout << std::endl;
	std::cout << "Demais arestas: ";
	for ( int i = 2; i < G->ordem; ++i) {
		for ( int j = 2; j < G->ordem; ++j) {
			if(matriz[i][j].capacidade != 0) {
				std::cout << "(" << i << ", " << j << ")["
				<< matriz[i][j].capacidade << "] ";
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
		if(tempos[v-2].second == -1){
			incluirAresta(G, VERTICE_FONTE, v, INFINITO);
		}
		else{
			incluirAresta(G, VERTICE_FONTE, v, tempos[v-2].second);
		}
		if(tempos[v-2].first == -1){
			incluirAresta(G, v, VERTICE_SUMIDOURO, INFINITO);
		}
		else{
			incluirAresta(G, v, VERTICE_SUMIDOURO, tempos[v-2].first);
		}
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
		incluirAresta(G, std::get<1>(link), std::get<0>(link), std::get<2>(link));
	}
}
/**/

// ========================= Cálculos e funções auxiliares

void processadorUmPrimeiro(Grafo* G) {
	auto total = 0u;
	auto matriz = G->matrizAdjacencia;
	std::unordered_set<int> processadorUm;
	std::vector<int> processadorDois;
	for ( int v = 0; v < G->ordem; ++v) {
		auto tempo = matriz[VERTICE_FONTE][v].capacidade;
		if(tempo == INFINITO){
			tempo = matriz[v][VERTICE_SUMIDOURO].capacidade;
			processadorDois.push_back(v);
		} else {
			processadorUm.insert(v);
		}
		total += tempo;
	}
	for ( auto& u: processadorDois){
		for ( int v = 2; v < G->ordem; ++v) {
			if(processadorUm.count(v) > 0)
				total += matriz[u][v].capacidade;
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
		if(tempo == INFINITO){
			tempo = matriz[VERTICE_FONTE][v].capacidade;
			processadorUm.push_back(v);
		} else {
			processadorDois.insert(v);
		}
		total += tempo;
	}
	for ( auto& u: processadorUm){
		for ( int v = 2; v < G->ordem; ++v) {
			if(processadorDois.count(v) > 0)
				total += matriz[u][v].capacidade;
		}
	}
	std::cout << "Alocar todos os módulos possíveis em P2: " <<
		total << " unidades de tempo." << std::endl;
}
/**/
// ========================= Algoritmos de fluxo em redes
int buscaLargura( Grafo* G, std::vector<int>& pai) {
	auto matriz = G->matrizAdjacencia;
    int u = -1, v = -1;
    int fluxoAumento = -1;
    // pai
    std::fill(pai.begin(), pai.end(), -1);
    //  fila da busca em largura
    std::queue<int> fila;
    fila.push(VERTICE_FONTE);
    while( !fila.empty() ){
    	u = fila.front();
    	fila.pop();
    	// v = vertice v na aresta (u, v)
    	for ( v = 1; v < G->ordem; ++v) {
    		if(matriz[u][v].capacidade - matriz[u][v].fluxo > 0 and pai[v] == -1) {
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
        v = VERTICE_SUMIDOURO;
        // fonte
        while (v != VERTICE_FONTE) {
        	u = pai[v];
        	matriz[u][v].fluxo += fluxoAumento;
    		matriz[v][u].fluxo -= fluxoAumento;
            v = u;
        }
    }
    return fluxoMaximo;
}

int balanceamento( Grafo *G) {
	int total = 0;
	auto matriz = G->matrizAdjacencia;
	std::vector<int> p1, p2;
	for ( int i = 2; i < G->ordem; ++i) {
		auto arestaFonte = matriz[VERTICE_FONTE][i];
		auto arestaSumidouro = matriz[i][VERTICE_SUMIDOURO];
		if(arestaSumidouro.capacidade == arestaSumidouro.fluxo) {
			total += arestaSumidouro.capacidade;
			p1.push_back(i);
		} else if(arestaFonte.capacidade == arestaFonte.fluxo) {
			total += arestaFonte.capacidade;
			p2.push_back(i);
		} else {
			std::cout << "Erro, módulo " << i << "não foi alocado. O algoritimo pode aumentar "
			<< fluxoMaximo(G) << " unidades de fluxo. Capacidade e fluxo das arestas fonte e sumidouro: " << std::endl;
			std::cout << arestaFonte.capacidade << " " << arestaFonte.fluxo << std::endl;
			std::cout << arestaSumidouro.capacidade << " " << arestaSumidouro.fluxo << std::endl;
			assert(arestaFonte.capacidade == arestaFonte.fluxo or arestaSumidouro.capacidade == arestaSumidouro.fluxo);
		}
	}
	for (auto &u : p1) {
		for(auto &v : p2) {
			if(matriz[u][v].capacidade - std::abs(matriz[v][u].fluxo) == 0) {
				total += matriz[u][v].capacidade;
			}
		}
	}
	std::cout << "Processador 1: ";
	std::copy(p1.begin(), p1.end(), std::ostream_iterator<int>( std::cout, " "));
	std::cout << std::endl;
	std::cout << "Processador 2: ";
	std::copy(p2.begin(), p2.end(), std::ostream_iterator<int>( std::cout, " "));
	std::cout << std::endl << "Total do corte: " << total << std::endl;
}

int main( int argv, char* args[]) {
	Grafo* G = carregarDoArquivo(args[1]);
	carregarLinksDoArquivo(G, args[2]);
	// Grafo* G = carregarDoArquivo("sampledata/sample_1.csv");
	// carregarLinksDoArquivo(G, "sampledata/sample_1_links_0.csv");
	imprimirGrafo(G);
	processadorUmPrimeiro(G);
	processadorDoisPrimeiro(G);
	std::cout << "Fluxo máximo: " << fluxoMaximo(G) << std::endl;
	balanceamento(G);
	destruirGrafo(G);
	return 0;
}
