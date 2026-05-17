#include <iostream>
#include <vector>
#include "GestiuneAbonati.h"
#include "ExceptiiBiblioteca.h"
#include "AbonatStudent.h"
#include "AbonatCopil.h"
#include "AbonatPremium.h"
#include "AbonatPensionar.h"

Abonati* autentificare(GestiuneAbonati& bibl) {
    std::string cnpIntrodus;
    std::cout << "=== LOGIN BIBLIOTECA ===" << std::endl;
    std::cout << "Introduceti CNP-ul pentru logare: ";
    std::cin >> cnpIntrodus;

    Abonati* gasit = bibl.gasesteAbonat(cnpIntrodus.c_str());
    
    if (gasit != nullptr) {
        std::cout << "Bine ai venit, " << gasit->getNume() << "!" << std::endl;
        return gasit;
    } else {
        std::cout << "CNP invalid! Utilizatorul nu exista." << std::endl;
        return nullptr;
    }
}

void returnareCarte(Abonati& abonat, GestiuneAbonati& bibl) {
    std::vector<Imprumuturi*>& toateImprumuturile = bibl.getListaImprumuturi();
    std::vector<int> indiciPersonali; // Salvam pozitiile din lista mare

    std::cout << "\n--- Cartile tale imprumutate ---" << std::endl;
    for (int i = 0; i < (int)toateImprumuturile.size(); i++) {
        if (std::string(toateImprumuturile[i]->getAbonat().getCNP()) == abonat.getCNP()) {
            indiciPersonali.push_back(i);
            std::cout << indiciPersonali.size() << ". Carte ID: " 
                      << toateImprumuturile[i]->getIdCarte() << std::endl;
        }
    }

    if (indiciPersonali.empty()) {
        std::cout << "Nu ai niciun imprumut activ." << std::endl;
        return;
    }

    std::cout << "Alege numarul imprumutului pentru returnare (sau 0 pentru anulare): ";
    int alegere;
    std::cin >> alegere;

    if (alegere > 0 && alegere <= (int)indiciPersonali.size()) {
        int indexInListaMare = indiciPersonali[alegere - 1];
        Imprumuturi* temporar = toateImprumuturile[indexInListaMare];

        bool esteIntarziat = (temporar->getZileScurse() > temporar->getPerioadaZile());
        int puncteDeModificat = 0;

        // Identificam tipul abonatului folosind dynamic_cast
        if (dynamic_cast<AbonatCopil*>(&abonat)) {
            puncteDeModificat = esteIntarziat ? -10 : 10;
        } else if (dynamic_cast<AbonatPremium*>(&abonat)) {
            puncteDeModificat = esteIntarziat ? -5 : 10;
        } else {
            puncteDeModificat = esteIntarziat ? -5 : 5; // Standard
        }

        abonat.modificaScor(puncteDeModificat);
        std::cout << "Scor incredere actualizat: " << abonat.getScorIncredere() 
                  << " (" << (puncteDeModificat > 0 ? "+" : "") << puncteDeModificat << ")" << std::endl;
                  
        int scurse = temporar->getZileScurse();
        int limita = temporar->getPerioadaZile();
        double taxaPenalizare = temporar->getTaxaPenalizare();

        if (scurse > limita) {
            int zileIntarziere = scurse - limita;
            double penalizareTotala = zileIntarziere * taxaPenalizare;

            std::cout << "Intarziere detectata! Taxa/zi: " << taxaPenalizare << " RON" << std::endl;
            std::cout << "Total de plata: " << penalizareTotala << " RON" << std::endl;

            abonat.adaugaPenalizare(penalizareTotala); }

        // Gasim obiectul Carte pentru a-i creste stocul
        Carti* c = bibl.gasesteCarte(temporar->getIdCarte());
        if (c) c->elibereazaCarte();

        // Stergem din memorie si din vector
        delete temporar;
        toateImprumuturile.erase(toateImprumuturile.begin() + indexInListaMare);

        // Salvare fisiere
        bibl.salveazaImprumuturi("imprumuturi.txt");
        bibl.salveazaCartiInFisier("carti.txt");
        bibl.salveazaAbonatiInFisier("abonati.txt");

        std::cout << "Cartea a fost returnata cu succes!" << std::endl;
    }
}

void navigareSiImprumut(Abonati& abonat, GestiuneAbonati& bibl) {
    // Luam listele direct din obiectul bibl
    std::vector<Carti*>& listaCarti = bibl.getListaCarti();
    std::vector<Imprumuturi*>& listaImprumuturi = bibl.getListaImprumuturi();

    int index = 0;
    char tasta;
    bool inMeniu = true;

    if (listaCarti.empty()) {
        std::cout << "Nu exista carti in biblioteca!" << std::endl;
        return;
    }

    while (inMeniu) {
        std::cout << "\n--- Navigare Biblioteca ---" << std::endl;
        std::cout << "Cartea " << index + 1 << " din " << listaCarti.size() << ":" << std::endl;
        
        Carti* c = listaCarti[index];
        std::cout << "Titlu: " << c->getTitlu() << " | Autor: " << c->getAutor() << std::endl;

        std::cout << "Stoc: " << c->getStoc() << " | Adulti: " << (c->esteDoarAdulti() ? "DA" : "NU") << std::endl;
        
        std::cout << "\n[a] Inapoi | [d] Inainte | [i] Imprumuta | [r] Returneaza | [t] Trecere o zi | [x] Iesire" << std::endl;
        std::cout << "Comanda: ";
        std::cin >> tasta;

        switch (tasta) {
            case 'a': 
                if (index > 0) index--;
                else index = listaCarti.size() - 1; 
                break;

            case 'd': 
                if (index < (int)listaCarti.size() - 1) index++;
                else index = 0; 
                break;

            case 'i': 
                try {

                    if (abonat.getBalantaPenalizari() >= 25) {
                        std::cout << "EROARE: Balanta ta de penalizari este " << abonat.getBalantaPenalizari() << " RON (Limita este de 25 RON)." << std::endl;
                        std::cout << "Te rugam sa achiti penalizarile la receptie." << std::endl;
                        break;
                    }  

                    if (abonat.getDurataAbonament() <= 0) {
                        std::cout << "EROARE: Abonamentul tau a expirat! Te rugam sa il reinoiesti." << std::endl;
                        break;
                    }  

                    if (listaCarti[index]->getStoc() <= 0) {
                        std::cout << "EROARE: Nu mai sunt exemplare disponibile!" << std::endl;
                        break;
                    }
                    // Cream imprumutul
                    Imprumuturi* nou = new Imprumuturi(abonat, *listaCarti[index], "01/01/2026", 14);
                    listaImprumuturi.push_back(nou);
                    
                    // Apelam salvarea folosind
                    bibl.adaugaImprumutInFisier(nou, "imprumuturi.txt");
                    bibl.salveazaCartiInFisier("carti.txt");
                    
                    std::cout << "SUCCES: Imprumut realizat si salvat!" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "EROARE: " << e.what() << std::endl;
                }
                break;
            
            case 'r':
                returnareCarte(abonat, bibl);
                break;
            
            case 't':
            {
                std::cout << "--- SIMULARE FINAL DE ZI ---" << std::endl;
                
                bibl.proceseazaFinalDeZi();
                
                bibl.salveazaAbonatiInFisier("abonati.txt");
                bibl.salveazaImprumuturi("imprumuturi.txt");
                
                std::cout << "Ziua a fost procesata si datele salvate!" << std::endl;
            }
            break;

            case 'x':
                inMeniu = false;
                break;

            default:
                std::cout << "Tasta invalida!" << std::endl;
        }
    }
}

int main() {
    GestiuneAbonati& biblioteca = GestiuneAbonati::getInstance();
    
    // Incarcam datele
    biblioteca.incarcaDinFisier("abonati.txt");
    biblioteca.incarcaCarti("carti.txt");
    biblioteca.incarcaImprumuturi("imprumuturi.txt");

    Abonati* abonatCurent = nullptr;

    while (abonatCurent == nullptr) {
        abonatCurent = autentificare(biblioteca);
    }

    navigareSiImprumut(*abonatCurent, biblioteca);

    return 0;
}