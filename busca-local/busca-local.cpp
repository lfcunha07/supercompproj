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

            // Checando se a nova distancia e melhor
            if (round_dist < best_distance){
                best_distance = round_dist;
                best_city_index = city_index;
            } else {
                std::swap(city_index[i-1], city_index[i]);
            }
        }

        // Cout error
        cerr << "local: " << initial_distance << " ";
        for (long unsigned int i = 0; i < city_index.size(); i++)
        {
            cerr << initial_vector[i] << " ";
        }
        cerr << "\n";
    }

    // Printa a ordem das cidades
    cout << best_distance << " 0" << "\n";
    for(long unsigned int i=0; i<best_city_index.size(); i++){
        cout << best_city_index[i] << " ";
    }
    cout << "\n";

    // // DEBUG
    // double round_dist = 0;
    // for(long unsigned int i=1; i<best_city_index.size(); i++){
    //     double dist {distance(cidades[best_city_index[i-1]], cidades[best_city_index[i]])};
    //     cout << round_dist << " + " << dist << " = ";
    //     round_dist += dist;
    //     cout << round_dist << endl;
    // }		
    // double regress {distance(cidades[best_city_index[0]], cidades[best_city_index[best_city_index.size()-1]])};
    // cout << round_dist << " + " << regress << " = ";
    // round_dist += regress;
    // cout << round_dist << endl;


    return 0;
}