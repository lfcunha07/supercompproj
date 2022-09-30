#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>

using namespace std;
int leafs = 0;

// REF https://www.techiedelight.com/erase-an-element-from-a-vector-by-index-in-cpp/

// Classe cidade
struct city
{
    double x, y;
	long unsigned int index;
};

// Classe percurso
struct route
{
    std::vector<city> stops;
	double distance;
};

// Calcula distancia
double distance(city origin, city dest)
{
    return sqrt(pow(dest.x - origin.x, 2) + pow(dest.y - origin.y, 2) * 1.0);
}

// Remove item de vetor
void remove(std::vector<city>& v, size_t index) 
{
    v.erase(v.begin() + index);
}

// Printa rota
void print_route(route rota) 
{
	std::vector<city> vect (rota.stops);
	double total_dist (rota.distance);

	if(!vect.empty()){
		cout << total_dist << " 1" << "\n";
		for(long unsigned int i=0; i<vect.size(); i++){
			cout << vect[i].index << " ";
		}
    	cout << "\n";
	}
}

// Busca exaustiva recursiva
route brute_force(vector<city> remaining_cities, vector<city> current_path)
{
    // Retorna se não houverem mais cidades a visitar
    if (remaining_cities.empty()){
		leafs++;
		route path_0;
		path_0.distance = 0;
		path_0.stops = current_path;
        return path_0;
    }

    std::vector<route> all_paths(remaining_cities.size()); // vetor de caminhos

	// Loop da recursão
	long unsigned int path = 0;
	bool loop = true;
	do{
		// Escolhendo um caminho dos remanescentes e o removendo das possibilidades
		std::vector<city> chosen_path (current_path);
        chosen_path.push_back(remaining_cities[path]);
        std::vector<city> new_remaining_cities (remaining_cities);
        remove(new_remaining_cities, path);

		// Chamando rescusão
        all_paths[path] = brute_force(new_remaining_cities, chosen_path);

		// Condição do loop
		if(path == remaining_cities.size() - 1){
			loop = false;
		}

		path ++;
	} while (loop);

    // Escolhendo o melhor dos vetores
	std::vector<city> best_chosen_path (all_paths[0].stops);
    double best_chosen_dist = 0;
	// Seleciona vetor
    for (long unsigned int i=0; i < all_paths.size(); i++){
		double round_dist = 0;
		route chosen_route (all_paths[i]);
		std::vector<city> chosen_path (chosen_route.stops);
		// Calcula distancia
		for (long unsigned int j=1; j<chosen_path.size(); j++){
			double dist {distance(chosen_path[j-1], chosen_path[j])};
			round_dist += dist;
		}
		double regress {distance(chosen_path[0], chosen_path[chosen_path.size()-1])};
		round_dist += regress;
		// Checando se a nova distancia e melhor
		if (round_dist < best_chosen_dist || best_chosen_dist == 0){
			best_chosen_dist = round_dist;
			best_chosen_path = chosen_path;
		}
    }

	// Criando rota de retorno
    route best_chosen_route;
	best_chosen_route.distance = best_chosen_dist;
	best_chosen_route.stops = best_chosen_path;
	
    return best_chosen_route;
}

// MAIN
int main(int argc, char const *argv[])
{
    // Pegando o numero de cidades
    long unsigned int n_city;
    cin >> n_city;

    // Pegando a coordenada das cidades e adicionando ao array
    std::vector<city> cidades(n_city);
    for(long unsigned int i=0; i<n_city; i++){
        city cidade;
        double x,y;
        cin >> x;
        cin >> y;
        cidade.x = x;
        cidade.y = y;
		cidade.index = i;
        cidades[i] = cidade;
    }

    // ------------------------------------------------ BUSCA EXAUSTIVA ------------------------------------------------
	std::vector<city> vazio;
	route result;
	result = brute_force(cidades, vazio);
	cerr << "num_leaf " << leafs << "\n";
	print_route(result); 
	return 0;
}