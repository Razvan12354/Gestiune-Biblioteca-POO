#ifndef GESTIUNEABONATI_H
#define GESTIUNEABONATI_H

#include "Abonati.h"
#include "Carti.h"
#include "Imprumuturi.h"
#include "Catalog.h"
#include <string>
#include <vector>

class GestiuneAbonati {
private:
    std::vector<Abonati*> lista;
    
    Catalog<Carti> catalogCarti; // Catalog pentru carti
    Catalog<Imprumuturi> catalogImprumuturi; // Catalog pentru imprumuturi

    GestiuneAbonati() = default; // Constructor privat pentru singleton

public:
    GestiuneAbonati(const GestiuneAbonati& alta) = delete; // Interzice copierea
    GestiuneAbonati& operator=(const GestiuneAbonati& alta) = delete; // Interzice atribuirea

    static GestiuneAbonati& getInstance() {
        static GestiuneAbonati instance; // Creata o singura data la prima apelare
        return instance;
    }

    // Destructor
    ~GestiuneAbonati();

    // Metoda pentru adaugat abonati (primeste pointer la baza)
    void adaugaAbonat(const Abonati& a);

    // Metoda pentru Trecere timp
    void trecereTimp(int luni);

    // Acorda bonus de durata daca abonatul e Student (Dynamic_cast)
    void aplicaPromotieStudenti(int luniExtra);

    // Afisarea intregii liste
    void afiseazaToti() const;

    // Procesare final de zi
    void proceseazaFinalDeZi();

    Imprumuturi* getImprumut(size_t index) {
    if (index < (size_t)catalogImprumuturi.dimensiune()) {
            return catalogImprumuturi[(int)index];
        }
    return nullptr;
}

    // Citirea din fisier
    void incarcaDinFisier(const std::string& numeFisier);
    void incarcaCarti(const std::string& numeFisier);
    void incarcaImprumuturi(const std::string& numeFisier);

    Abonati* gasesteAbonat(const char* cnp);
    Carti* gasesteCarte(const char* id);

    std::vector<Abonati*>& getListaAbonati() { return lista; }
    std::vector<Carti*>& getListaCarti() { return catalogCarti.getElemente();; }
    std::vector<Imprumuturi*>& getListaImprumuturi() { return catalogImprumuturi.getElemente(); }

    void salveazaImprumuturi(const std::string& numeFisier);

    void adaugaImprumutInFisier(Imprumuturi* imp, const std::string& numeFisier);

    void salveazaCartiInFisier(const std::string& numeFisier);

    void salveazaAbonatiInFisier(const std::string& numeFisier);

    Imprumuturi* gasesteImprumutDupaCarte(const char* idCarte);
};

#endif