#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>

using namespace std;

// Classe cidade
struct city
{
    double x, y;
};

// Calcula distancia
double distance(city origin, city dest)
{
    return sqrt(pow(dest.x - origin.x, 2) + pow(dest.y - origin.y, 2) * 1.0);
}

// Cria vetor aleatorio
vector<long> &myFunction(vector<long> &results){
    for ( auto &i : results) {
        i*=i;
    }  
   return results;
}

// Printa vetor
void print_vector(const vector<int> &vect) 
{
    long unsigned int i;
    for (i=0; i<vect.size()-1; i++){
        cout << vect[i] << " -> "; 
    }
    cout << vect[i] << " FIM\n"; 
}

// MAIN
int main(int argc, char const *argv[])
{
    // Pegando o numero de cidades
    long unsigned int n_city;
    cin >> n_city;
    std::vector<int> city_index;

    // Pegando a coordenada das cidades e adicionando ao array
    city cidades[n_city];
    for(long unsigned int i=0; i<n_city; i++){
        city cidade;
        double x,y;
        cin >> x;
        cin >> y;
        cidade.x = x;
        cidade.y = y;
        cidades[i] = cidade;
        city_index.push_back(i);
    }

    // ------------------------------------------------ BUSCA LOCAL ------------------------------------------------
    double best_distance = 0;
    std::vector<int> best_city_index (city_index);
    for (long unsigned int i=0; i<10*n_city; i++){

        // Criando vetor aleatorio
        std::random_device rd; 
        auto rng = std::default_random_engine { rd() };
        std::shuffle(std::begin(city_index), std::end(city_index), rng);
        for(long unsigned int i=0; i<city_index.size(); i++){
            best_city_index.at(i) = city_index[i];
        }

        // Calculando distancia inicial
        double initial_distance = 0;
        for(long unsigned int i=1; i<city_index.size(); i++){
            long unsigned int new_origin = city_index[i-1];
            long unsigned int new_dest = city_index[i];
            double dist {distance(cidades[new_origin], cidades[new_dest])};
            initial_distance += dist;
        }
        if(best_distance == 0 || best_distance > initial_distance){
            best_distance = initial_distance;
        }

        // Realizando Busca Local
        for (long unsigned int i=0; i<city_index.size()-1; i++){

            // Invertendo ordem no vetor
            std::swap(city_index[i], city_index[i+1]);

            // Recalculando distancia 
            double round_dist = 0;
            for(long unsigned int i=1; i<city_index.size(); i++){
                long unsigned int new_origin = city_index[i-1];
                long unsigned int new_dest = city_index[i];
                double dist {distance(cidades[new_origin], cidades[new_dest])};
                round_dist += dist;
            }		
            double regress {distance(cidades[city_index[0]], cidades[city_index[city_index.size()-1]])};
		    round_dist += regress;

            // Checando se a nova distancia e melhor
            if (round_dist >= best_distance){
                std::swap(city_index[i+1], city_index[i]);
            } else {
                best_distance = round_dist;
                for(long unsigned int i=0; i<city_index.size(); i++){
                    best_city_index.at(i) = city_index[i];
                }
            }
            initial_distance = round_dist;
        }

        // Cout error
        cerr << "local: " << initial_distance << " ";
        for (size_t i = 0; i < city_index.size(); i++)
        {
            cerr << city_index[i] << " ";
        }
        cerr << "\n";
    }

    // Printa a ordem das cidades
    cout << best_distance << " 0" << "\n";
    for(long unsigned int i=0; i<best_city_index.size(); i++){
        cout << best_city_index[i] << " ";
    }
    cout << "\n";
    return 0;
}