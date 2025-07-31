#include "grafos.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cfloat>
using namespace std;

int buscarIndice(const char* nombre, char** nombres, int total) {
    for (int i = 0; i < total; ++i) {
        if (strcmp(nombre, nombres[i]) == 0)
            return i;
    }
    return -1;
}

// Nueva función auxiliar para agregar nodo y redimensionar arreglo si es necesario
void agregarSiNoExiste(const string& nombre, char*** nombres, int& totalNodos, int& capacidad, Grafo& grafo) {
    if (buscarIndice(nombre.c_str(), *nombres, totalNodos) == -1) {
        if (totalNodos == capacidad) {
            int nuevaCap = capacidad * 2;
            char** temp = new char*[nuevaCap];
            for (int i = 0; i < capacidad; ++i) temp[i] = (*nombres)[i];
            delete[] *nombres;
            *nombres = temp;
            capacidad = nuevaCap;
        }
        (*nombres)[totalNodos] = new char[64];
        strncpy((*nombres)[totalNodos], nombre.c_str(), 63);
        (*nombres)[totalNodos][63] = '\0';
        grafo.agregarNodo(nombre);
        totalNodos++;
    }
}

void dijkstra(const Grafo& grafo, int origen, double* dist, int* prev, int n) {
    bool* visitado = new bool[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = DBL_MAX;
        prev[i] = -1;
        visitado[i] = false;
    }
    dist[origen] = 0;

    for (int count = 0; count < n; ++count) {
        double minDist = DBL_MAX;
        int u = -1;
        for (int i = 0; i < n; ++i) {
            if (!visitado[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visitado[u] = true;

        Nodo* nodo = grafo.obtenerNodo(u);
        for (Arista* a = nodo->adyacentes; a; a = a->sig) {
            int v = a->destino;
            double peso = a->peso;
            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
            }
        }
    }
    delete[] visitado;
}

int main() {
    Grafo grafo(true);

    const char* rutaCSV = "bin/MOCK_DATA.csv";

    ifstream file1(rutaCSV);
    if (!file1.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return 1;
    }
    string linea;
    getline(file1, linea);

    int capacidad = 32, totalNodos = 0;
    char** nombres = new char*[capacidad];

    while (getline(file1, linea)) {
        stringstream ss(linea);
        string origen, destino, tiempoStr;
        getline(ss, origen, ',');
        getline(ss, destino, ',');
        getline(ss, tiempoStr, ',');

        agregarSiNoExiste(origen, &nombres, totalNodos, capacidad, grafo);
        agregarSiNoExiste(destino, &nombres, totalNodos, capacidad, grafo);
    }
    file1.close();

    ifstream file2(rutaCSV);
    getline(file2, linea);
    while (getline(file2, linea)) {
        stringstream ss(linea);
        string origen, destino, tiempoStr;
        getline(ss, origen, ',');
        getline(ss, destino, ',');
        getline(ss, tiempoStr, ',');

        double tiempo = atof(tiempoStr.c_str());
        int idxOrigen = buscarIndice(origen.c_str(), nombres, totalNodos);
        int idxDestino = buscarIndice(destino.c_str(), nombres, totalNodos);
        grafo.agregarArista(idxOrigen, idxDestino, tiempo);
    }
    file2.close();

    int opcion;
    do {
        cout << "\nMenu de opciones:\n";
        cout << "1. Calcular rutas desde un nodo\n";
        cout << "11. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                cout << "\nNodos disponibles:\n";
                for (int i = 0; i < totalNodos; ++i) {
                    cout << i << ": " << nombres[i] << endl;
                }
                cout << "Ingrese el numero del nodo de inicio: ";
                int idxCentro;
                cin >> idxCentro;
                cin.ignore();
                if (idxCentro < 0 || idxCentro >= totalNodos) {
                    cout << "Nodo invalido.\n";
                    break;
                }

                double* dist = new double[totalNodos];
                int* prev = new int[totalNodos];
                dijkstra(grafo, idxCentro, dist, prev, totalNodos);

                cout << "\nRutas mas rapidas desde '" << nombres[idxCentro] << "':\n";
                double mejorTiempo = DBL_MAX;
                int mejorDestino = -1;
                for (int i = 0; i < totalNodos; ++i) {
                    if (i == idxCentro) continue;
                    cout << "- Hasta " << nombres[i] << ": ";
                    if (dist[i] == DBL_MAX) {
                        cout << "No hay ruta\n";
                    } else {
                        cout << dist[i] << " minutos. Ruta: ";
                        int* ruta = new int[totalNodos];
                        int pasos = 0, at = i;
                        while (at != -1 && pasos < totalNodos) {
                            ruta[pasos++] = at;
                            at = prev[at];
                        }
                        for (int j = pasos - 1; j >= 0; --j) {
                            cout << nombres[ruta[j]];
                            if (j > 0) cout << " -> ";
                        }
                        cout << "\n";
                        if (dist[i] < mejorTiempo) {
                            mejorTiempo = dist[i];
                            mejorDestino = i;
                        }
                        delete[] ruta;
                    }
                }
                if (mejorDestino != -1) {
                    cout << "\nRuta mas optima desde '" << nombres[idxCentro] << "' es hasta '" << nombres[mejorDestino]
                        << "' con " << mejorTiempo << " minutos.\n";
                    int* rutaOptima = new int[totalNodos];
                    int pasos = 0, at = mejorDestino;
                    while (at != -1 && pasos < totalNodos) {
                        rutaOptima[pasos++] = at;
                        at = prev[at];
                    }
                    cout << "Camino optimo: ";
                    for (int j = pasos - 1; j >= 0; --j) {
                        cout << nombres[rutaOptima[j]];
                        if (j > 0) cout << " -> ";
                    }
                    cout << "\n";
                    delete[] rutaOptima;
                } else {
                    cout << "\nNo hay rutas disponibles desde este nodo.\n";
                }
                delete[] dist;
                delete[] prev;
                break;
            }
            case 11:
                cout << "Saliendo del programa.\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 11);

    for (int i = 0; i < totalNodos; ++i) delete[] nombres[i];
    delete[] nombres;
    return 0;
}
