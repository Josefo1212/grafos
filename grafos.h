#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Arista {
    int destino;
    double peso;
    Arista* sig;
    Arista(int destino, double peso) : destino(destino), peso(peso), sig(nullptr) {}
};

struct Nodo {
    string nombre;
    Arista* adyacentes;
    Nodo* sig;
    Nodo(const string& nombre) : nombre(nombre), adyacentes(nullptr), sig(nullptr) {}
};

class Grafo {
private:
    Nodo* inicio;
    bool dirigido;
public:
    Grafo(bool dirigido = true);
    ~Grafo();
    int agregarNodo(const string& nombre);
    void agregarArista(int origen, int destino, double peso);
    int buscarNodo(const string& nombre) const;
    Nodo* obtenerNodo(int indice) const;
    int cantidadNodos() const;
};
