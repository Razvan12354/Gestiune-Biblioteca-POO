#ifndef CATALOG_H
#define CATALOG_H

#include <vector>
#include <iostream>

template <typename T>
class Catalog {
private:
    std::vector<T*> elemente;

public:
    Catalog() = default;

    // Destructor
    ~Catalog() {
        for (T* elem : elemente) {
            delete elem;
        }
        elemente.clear();
    }

    // Adauga un element in catalog
    void adauga(T* elem) {
        if (elem != nullptr) {
            elemente.push_back(elem);
        }
    }

    // Sterge un element de la o anumita pozitie din vector
    void stergeLaIndex(int index) {
        if (index >= 0 && index < (int)elemente.size()) {
            elemente.erase(elemente.begin() + index);
        }
    }

    // Intoarce numarul de elemente din catalog
    int dimensiune() const {
        return (int)elemente.size();
    }

    // Verifica daca catalogul este gol
    bool esteGol() const {
        return elemente.empty();
    }

    // Operatorul [] pentru a accesa elementele direct din catalog
    T* operator[](int index) {
        if (index >= 0 && index < (int)elemente.size()) {
            return elemente[index];
        }
        return nullptr;
    }

    // Oferim acces la vectorul intern
    std::vector<T*>& getElemente() {
        return elemente;
    }

    // Functie sablon pentru cautare generica in interiorul catalogului
    template <typename Criteriu, typename Comparator>
    T* cautaElement(Criteriu valoare, Comparator comp) {
        for (T* elem : elemente) {
            if (elem != nullptr && comp(elem, valoare)) {
                return elem; // Am gasit elementul care respecta criteriul
            }
        }
        return nullptr; // Nu s-a gasit nimic
    }
};

#endif