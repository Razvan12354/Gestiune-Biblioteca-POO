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

    // Functie friend template
    template <typename U>
    friend void afiseazaInfoCatalog(const Catalog<U>& cat, const std::string& eticheta);

    // Destructor
    ~Catalog() {
        for (T* elem : elemente) {
            delete elem;
        }
        elemente.clear();
    }
};

// Implementare Functie friend template
template <typename U>
void afiseazaInfoCatalog(const Catalog<U>& cat, const std::string& eticheta) {
    std::cout << "[Catalog " << eticheta << "] In acest moment sunt inregistrate " 
              << cat.elemente.size() << " elemente in memorie.\n";
}

template <typename T, typename Element>
int numaraElementeCuConditie(Catalog<T>& catalog, Element conditie) {
    int contor = 0;
    for (int i = 0; i < catalog.dimensiune(); ++i) {
        T* elem = catalog[i];
        if (elem != nullptr && conditie(elem)) {
            contor++;
        }
    }
    return contor;
}

#endif