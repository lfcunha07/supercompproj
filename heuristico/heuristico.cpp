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
    }

    // ------------------------------------------------ HEURISTICA ------------------------------------------------
    int origin = 0;
    long unsigned int dest = 0;
    int min_city;
    double total_dist = 0;
    double min_dist = 0;
    city closest_city;
    std::vector<city> cidades_visitadas {cidades[origin]};
    std::vector<int> city_index {0};

    // Calcula distancia
    while(cidades_visitadas.size() != n_city){
        
        // Checa se estamos passando pela mesma cidade
        bool exists = false;
        for(long unsigned int i=0; i<cidades_visitadas.size(); i++){
            city & element = cidades_visitadas[i];
            if(element.x == cidades[dest].x && element.y == cidades[dest].y){
                exists = true;
            }
        }

        // Checa se distancia atual e menor que a anterior
        if(!exists){
            double dist {distance(cidades[origin], cidades[dest])};
            if(min_dist==0){
                min_dist = dist;
                closest_city = cidades[dest];
                min_city = dest;
            } else {
                if (dist < min_dist){
                    min_dist = dist;
                    closest_city = cidades[dest];
                    min_city = dest;
                }
            }
        }

        // Avanca de cidade
        dest++;

        // Caso tenha chegado na ultima cidade
        if(dest == n_city){
            cidades_visitadas.push_back(closest_city);
            city_index.push_back(min_city);
            total_dist += min_dist;
            min_dist = 0;
            origin = min_city;
            dest = 0;
        }

    }

    // Adiciona o retorno
    double regress {distance(cidades[city_index[0]], cidades[city_index[city_index.size()-1]])};
    total_dist += regress;

    // Printa a ordem das cidades
    cout << total_dist << " 0" << "\n";
    for(long unsigned int i=0; i<city_index.size(); i++){
        cout << city_index[i] << " ";
    }
    cout << "\n";
    return 0;
}