#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>
#include <omp.h>
#include <chrono>
#include <ranges>

using namespace std;
using namespace std::chrono;
using std::begin, std::end;

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

// Busca Local
std::vector<int> local_search(std::vector<int> city_index, long unsigned int n_city)
{
    city cidades[n_city];
    double best_distance = 0;
    std::vector<int> best_city_index (city_index);
    for (long unsigned int i=0; i<10*n_city; i++){
        // Criando vetor aleatorio
        std::random_device rd; 
        auto rng = std::default_random_engine { rd() };
        std::shuffle(std::begin(city_index), std::end(city_index), rng);

        // Calculando distancia inicial
        double initial_distance = 0;
        std::vector<int> initial_vector (city_index);
        for(long unsigned int i=1; i<city_index.size(); i++){
            double dist {distance(cidades[city_index[i-1]], cidades[city_index[i]])};
            initial_distance += dist;
        }
        double regress {distance(cidades[city_index[0]], cidades[city_index[city_index.size()-1]])};
        initial_distance += regress;

        if(best_distance == 0 || best_distance > initial_distance){
            best_distance = initial_distance;
            best_city_index = city_index;
        }   

        // Realizando Busca Local
        for (long unsigned int i=1; i<city_index.size(); i++){

            // Invertendo ordem no vetor
            std::swap(city_index[i-1], city_index[i]);

            // Recalculando distancia 
            double round_dist = 0;
            for(long unsigned int i=1; i<city_index.size(); i++){
                double dist {distance(cidades[city_index[i-1]], cidades[city_index[i]])};
                round_dist += dist;
            }		
            double regress {distance(cidades[city_index[0]], cidades[city_index[city_index.size()-1]])};
            round_dist += regress;

            // Variável da menor busca local
            if(round_dist < initial_distance){
                initial_distance = round_dist;
                initial_vector = city_index;
            }

            // Checando se a nova distancia é melhor
            if (round_dist < best_distance){
                best_distance = round_dist;
                best_city_index = city_index;
            } else {
                std::swap(city_index[i-1], city_index[i]);
            }
        }
    
        // // Cout error
        // cerr << "local: " << initial_distance << " ";
        // for (long unsigned int i = 0; i < city_index.size(); i++)
        // {
        //     cerr << initial_vector[i] << " ";
        // }
        // cerr << "\n";
    }
    return city_index;
}


// MAIN
int main(int argc, char const *argv[])
{
    // Tempo
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

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

    std::vector<std::vector<int>> best_city_index;
    std::vector<double> best_distances;
    std::vector<int> city_distance;
    #pragma omp parallel
    {
        city_distance = local_search(city_index, n_city);
        double best_distance = 0;

        for(long unsigned int i=1; i<city_distance.size(); i++){
            double dist {distance(cidades[city_distance[i-1]], cidades[city_distance[i]])};
            best_distance += dist;
        }
        double regress {distance(cidades[city_distance[0]], cidades[city_distance[city_distance.size()-1]])};
        best_distance += regress;
        #pragma omp critical
        {
            best_city_index.push_back(city_distance);
            best_distances.push_back(best_distance);
        }
    }

    double min_dist = best_distances[0];
    long unsigned int c_val = 0;
    for(long unsigned int i=1; i<best_distances.size(); i++){
        if(best_distances[i] < min_dist){
            min_dist = best_distances[i];
            c_val = i;
        }
    }
    city_distance = best_city_index[c_val];

    // Printa a ordem das cidades
    cout << min_dist << " 0" << "\n";
    for(long unsigned int i=0; i<city_distance.size(); i++){
        cout << city_distance[i] << " ";
    }
    cout << "\n";

    end = std::chrono::system_clock::now();
  
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
    std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}