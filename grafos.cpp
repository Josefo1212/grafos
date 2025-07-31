#include "grafos.h"

Grafo::Grafo(bool dirigido) : inicio(nullptr), dirigido(dirigido) {}

Grafo::~Grafo() {
    Nodo* actual = inicio;
    while (actual) {
        Arista* a = actual->adyacentes;
        while (a) {
            Arista* tempA = a;
            a = a->sig;
            delete tempA;
        }
        Nodo* tempN = actual;
        actual = actual->sig;
        delete tempN;
    }
}

int Grafo::agregarNodo(const string& nombre) {
    Nodo* nuevo = new Nodo(nombre);
    if (!inicio) {
        inicio = nuevo;
        return 0;
    }
    int idx = 0;
    Nodo* actual = inicio;
    while (actual->sig) {
        actual = actual->sig;
        idx++;
    }
    actual->sig = nuevo;
    return idx + 1;
}

int Grafo::buscarNodo(const string& nombre) const {
    Nodo* actual = inicio;
    int idx = 0;
    while (actual) {
        if (actual->nombre == nombre)
            return idx;
        actual = actual->sig;
        idx++;
    }
    return -1;
}

Nodo* Grafo::obtenerNodo(int indice) const {
    Nodo* actual = inicio;
    int idx = 0;
    while (actual) {
        if (idx == indice)
            return actual;
        actual = actual->sig;
        idx++;
    }
    return nullptr;
}

int Grafo::cantidadNodos() const {
    Nodo* actual = inicio;
    int count = 0;
    while (actual) {
        count++;
        actual = actual->sig;
    }
    return count;
}

void Grafo::agregarArista(int origen, int destino, double peso) {
    Nodo* nodoOrigen = obtenerNodo(origen);
    Nodo* nodoDestino = obtenerNodo(destino);
    if (!nodoOrigen || !nodoDestino)
        return;
    Arista* nueva = new Arista(destino, peso);
    if (!nodoOrigen->adyacentes) {
        nodoOrigen->adyacentes = nueva;
    } else {
        Arista* actual = nodoOrigen->adyacentes;
        while (actual->sig)
            actual = actual->sig;
        actual->sig = nueva;
    }
    if (!dirigido) {
        Arista* nueva2 = new Arista(origen, peso);
        if (!nodoDestino->adyacentes) {
            nodoDestino->adyacentes = nueva2;
        } else {
            Arista* actual2 = nodoDestino->adyacentes;
            while (actual2->sig)
                actual2 = actual2->sig;
            actual2->sig = nueva2;
        }
    }
}
