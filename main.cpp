#include <iostream>
#include <vector>
#include "GestiuneAbonati.h"
#include "ExceptiiBiblioteca.h"
#include "AbonatStudent.h"
#include "AbonatCopil.h"
#include "AbonatPremium.h"
#include "AbonatNormal.h"

Abonati* autentificare(GestiuneAbonati& bibl) {
    std::string cnpIntrodus;
    std::cout << "=== LOGIN BIBLIOTECA ===" << std::endl;
    std::cout << "Introduceti CNP-ul pentru logare (sau 'x' pentru a renunta): ";
    std::cin >> cnpIntrodus;

    if (cnpIntrodus == "x" || cnpIntrodus == "X") {
        std::cout << "Logare anulata." << std::endl;
        return nullptr;
    }

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
    std::vector<int> persoana; // Salvam pozitiile din lista mare

    std::cout << "\n--- Cartile tale imprumutate ---" << std::endl;
    for (int i = 0; i < (int)toateImprumuturile.size(); i++) {
        if (std::string(toateImprumuturile[i]->getAbonat().getCNP()) == abonat.getCNP()) {
            persoana.push_back(i);
            std::cout << persoana.size() << ". Carte ID: " 
                      << toateImprumuturile[i]->getIdCarte() << std::endl;
        }
    }

    if (persoana.empty()) {
        std::cout << "Nu ai niciun imprumut activ." << std::endl;
        return;
    }

    std::cout << "Alege numarul imprumutului pentru returnare (sau 0 pentru anulare): ";
    int alegere;
    std::cin >> alegere;

    if (alegere > 0 && alegere <= (int)persoana.size()) {
        int indexInListaMare = persoana[alegere - 1];
        Imprumuturi* temporar = toateImprumuturile[indexInListaMare];

        bool esteIntarziat = (temporar->getZileScurse() > temporar->getPerioadaZile());
        int puncteDeModificat = 0;

        // Identificam tipul abonatului folosind dynamic_cast
        if (dynamic_cast<AbonatPremium*>(&abonat)) {
            puncteDeModificat = esteIntarziat ? -5 : 10;
        } else {
            puncteDeModificat = esteIntarziat ? -10 : 5; // Standard
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

void verificaImprumuturiDupaCarte(GestiuneAbonati& bibl) {
    std::string idCarteCautat;
    std::cout << "Introduceti ID-ul cartii cautate: ";
    std::cin >> idCarteCautat;

    std::vector<Imprumuturi*>& toateImprumuturile = bibl.getListaImprumuturi();
    bool gasitCelPutinUnul = false;
    int contor = 1;

    std::cout << "\n=== TOATE IMPRUMUTURILE ACTIVE PENTRU CARTEA ID: " << idCarteCautat << " ===" << std::endl;

    for (auto imp : toateImprumuturile) {
        // Preluam ID-ul cartii sub forma de const char*
        if (std::string(imp->getIdCarte()) == idCarteCautat) {
            gasitCelPutinUnul = true;
            
            std::cout << "\n" << contor << ". ";
            std::cout << *imp << std::endl;

            std::cout << "   > Perioada: " << imp->getPerioadaZile() << " zile" << std::endl;
            std::cout << "   > Zile scurse: " << imp->getZileScurse() << " zile" << std::endl;
            std::cout << "   > Taxa penalizare curenta: " << imp->getTaxaPenalizare() << " RON/zi" << std::endl;
            
            // Verifica daca este depasit termenul
            if (imp->getZileScurse() > imp->getPerioadaZile()) {
                int intarziere = imp->getZileScurse() - imp->getPerioadaZile();
                std::cout << "   > STARE: [!] INTARZIAT CU " << intarziere << " ZILE" << std::endl;
            } else {
                std::cout << "   > STARE: In termen legal" << std::endl;
            }
            
            contor++;
        }
    }

    if (!gasitCelPutinUnul) {
        std::cout << "Nu s-au gasit imprumuturi active pentru acest ID de carte." << std::endl;
    }
}

void adaugaAbonatManual(GestiuneAbonati& bibl) {
    std::cout << "\n--- ADAUGARE ABONAT NOU ---" << std::endl;
    char tip;
    std::string nume, prenume, cnp, telefon;
    int varsta, durata;

    std::cout << "Tip abonat (C - Copil, S - Student, P - Premium, N - Normal): ";
    std::cin >> tip;
    tip = std::toupper(tip);

    std::cout << "Nume: "; std::cin >> nume;
    std::cout << "Prenume: "; std::cin >> prenume;
    std::cout << "CNP (13 caractere): "; std::cin >> cnp;
    std::cout << "Telefon (10 cifre): "; std::cin >> telefon;
    std::cout << "Varsta: "; std::cin >> varsta;
    std::cout << "Durata abonament (zile): "; std::cin >> durata;

    // Verificare CNP unic
    if (bibl.gasesteAbonat(cnp.c_str()) != nullptr) {
    std::cout << "EROARE: Exista deja un abonat inregistrat cu acest CNP!" << std::endl;
    return;
}

    try {
        if (tip == 'S') {
            std::string univ;
            std::cout << "Universitate: ";
            std::cin.ignore();
            std::getline(std::cin, univ);
            
            Abonati* nouS = new AbonatStudent(nume.c_str(), prenume.c_str(), cnp.c_str(), telefon.c_str(), varsta, durata, univ.c_str());
            bibl.adaugaAbonat(nouS);
        } 
        else if (tip == 'C') {
            Abonati* nouC = new AbonatCopil(nume.c_str(), prenume.c_str(), cnp.c_str(), telefon.c_str(), varsta, durata);
            bibl.adaugaAbonat(nouC);
        } 
        else if (tip == 'P') {
            double taxa;
            std::cout << "Taxa Premium: "; std::cin >> taxa;
            Abonati* nouP = new AbonatPremium(nume.c_str(), prenume.c_str(), cnp.c_str(), telefon.c_str(), varsta, durata, taxa);
            bibl.adaugaAbonat(nouP);
        } 
        else if (tip == 'N') {
            Abonati* nouN = new AbonatNormal(nume.c_str(), prenume.c_str(), cnp.c_str(), telefon.c_str(), varsta, durata);
            bibl.adaugaAbonat(nouN);
        } 
        else {
            std::cout << "Tip invalid!" << std::endl;
            return;
        }

        bibl.salveazaAbonatiInFisier("abonati.txt");
        std::cout << "SUCCES: Abonatul a fost salvat in baza de date si in fisier!" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "EROARE la validare: " << e.what() << std::endl;
    }
}

void adaugaCarteManual(GestiuneAbonati& bibl) {
    std::cout << "\n--- ADAUGARE CARTE NOUA ---" << std::endl;
    std::string titlu, autor, idCarte;
    int an, stoc;
    char adultOpt, promoOpt;
    bool doarAdulti, laPromotie;

    std::cin.ignore();
    std::cout << "Titlu carte: "; std::getline(std::cin, titlu);
    std::cout << "Autor: "; std::getline(std::cin, autor);
    std::cout << "ID Carte: "; std::cin >> idCarte;
    std::cout << "An aparitie: "; std::cin >> an;
    std::cout << "Stoc initial: "; std::cin >> stoc;
    
    std::cout << "Este doar pentru adulti? (d/n): "; std::cin >> adultOpt;
    doarAdulti = (adultOpt == 'd' || adultOpt == 'D');

    std::cout << "Este la promotie? (d/n): "; std::cin >> promoOpt;
    laPromotie = (promoOpt == 'd' || promoOpt == 'D');

    // Verificare ID unic
    if (bibl.gasesteCarte(idCarte.c_str()) != nullptr) {
    std::cout << "EROARE: Acest ID de carte este deja atribuit altei carti din catalog!" << std::endl;
    return;
    }
    // Cream obiectul carte
    Carti* nouaCarte = new Carti(titlu.c_str(), autor.c_str(), idCarte.c_str(), an, stoc, doarAdulti, laPromotie);
    
    // O adaugam in catalogul clasei GestiuneAbonati
    bibl.getListaCarti().push_back(nouaCarte);

    // Salvam noul catalog in fisier
    bibl.salveazaCartiInFisier("carti.txt");
    std::cout << "SUCCES: Cartea a fost adaugata in catalog si salvata in fisier!" << std::endl;
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

        std::cout << "Stoc: " << c->getStoc() << " | Adulti: " << (c->esteDoarAdulti() ? "DA" : "NU") 
        << " | Promotie:" << (c->esteLaPromotie() ? "DA" : "NU") << std::endl;

        std::cout << "\n[a] Inapoi | [d] Inainte | [i] Imprumuta | [r] Returneaza | [x] Iesire" << std::endl;
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
    
    // Incarcam seturile bazei de date din fisiere
    biblioteca.incarcaDinFisier("abonati.txt");
    biblioteca.incarcaCarti("carti.txt");
    biblioteca.incarcaImprumuturi("imprumuturi.txt");

    char comandaAdmin;
    bool programInRulare = true;

    while (programInRulare) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "      SISTEM GESTIUNE BIBLIOTECA    " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Autentificare abonat\n";
        std::cout << "2. [ADMIN] Aplica promotie studenti\n";
        std::cout << "3. [ADMIN] Simuleaza trecere timp global\n";
        std::cout << "4. [ADMIN] Afiseaza toti abonatii din sistem\n";
        std::cout << "5. [ADMIN] Adauga ABONAT manual\n";
        std::cout << "6. [ADMIN] Adauga CARTE manual\n";
        std::cout << "7. [ADMIN] Cauta imprumuturi dupa ID Carte\n";
        std::cout << "8. [ADMIN] Sterge penalizarile unui abonat\n";
        std::cout << "9. [ADMIN] Retrogradare Abonat Premium\n";
        std::cout << "p  [ADMIN] Prelungeste abonamentul\n";
        std::cout << "s. [ADMIN] Afiseaza statistici globale\n";
        std::cout << "x. Inchide programul\n";
        std::cout << "Alegeti optiunea: ";
        std::cin >> comandaAdmin;

        switch (comandaAdmin) {
            case '1': {
                Abonati* abonatLogat = autentificare(biblioteca);
                if (abonatLogat != nullptr) {
                    // Daca s-a gasit abonatul intra in bucla de navigare carti
                    navigareSiImprumut(*abonatLogat, biblioteca);
                }
                break;
            }
            case '2': {
                std::cout << "\n--- ACORDARE ZILE BONUS DE ABONAMENT STUDENTILOR ---" << std::endl;
                int zileBonus;
                std::cout << "Cate zile suplimentare doriti sa oferiti studentilor? ";
                if (std::cin >> zileBonus) {
                    biblioteca.aplicaPromotieStudenti(zileBonus);
                    biblioteca.salveazaAbonatiInFisier("abonati.txt");
                    std::cout << "Promotia a fost aplicata!" << std::endl;
                }
                break;
            }
            case '3': {
                std::cout << "\n--- SIMULARE AVANS TIMP (ZILE) ---" << std::endl;
                int zileTrecute;
                std::cout << "Cate zile trec in sistem? ";
                if (std::cin >> zileTrecute) {
                    biblioteca.trecereTimp(zileTrecute);
                    biblioteca.salveazaAbonatiInFisier("abonati.txt");
                    std::cout << "Valabilitatea tuturor abonamentelor a fost actualizata!" << std::endl;
                }
                break;
            }
            case '4':{
                std::cout << "\n--- LISTA MEMBRI INREGISTRATI ---" << std::endl;
                biblioteca.afiseazaToti();
                break;
            }       
            case '5':{
                adaugaAbonatManual(biblioteca);
                break;
            }        
            case '6':{
                adaugaCarteManual(biblioteca);
                break;
            }
            case '7':{
            verificaImprumuturiDupaCarte(biblioteca);
            break;
            }

            case '8':{
                std::string cnpIntrodus;
                std::cout << "Introduceti CNP-ul abonatului pentru stergerea penalizarilor: ";
                std::cin >> cnpIntrodus;
                biblioteca.stergePenalizariAbonat(cnpIntrodus);
                break;
            }

            case '9':{
                std::string cnpModificare;
                std::cout << "\n--- RETROGRADARE ABONAT PREMIUM ---\n";
                std::cout << "Introduceti CNP-ul abonatului: ";
                std::cin >> cnpModificare;
                biblioteca.DowngradePremium(cnpModificare);
                break;
            }

            case 's':{
                std::cout << "\n--- STATISTICI BIBLIOTECA ---\n";
                // Afiseaza numarul total de carti si imprumuturi
                afiseazaInfoCatalog(biblioteca.getCatalogCarti(), "Carti");
                afiseazaInfoCatalog(biblioteca.getCatalogImprumuturi(), "Imprumuturi Active");

                std::cout << "----------------------------------------\n";

                // Apeleaza functia libera template pentru a numara elementele
                int cartiLaPromo = numaraElementeCuConditie(biblioteca.getCatalogCarti(), [](Carti* c) {
                    return c->esteLaPromotie();
                });

                int cartiAdulti = numaraElementeCuConditie(biblioteca.getCatalogCarti(), [](Carti* c) {
                    return c->esteDoarAdulti();
                });

                int imprumuturiRestante = numaraElementeCuConditie(biblioteca.getCatalogImprumuturi(), [](Imprumuturi* imp) {
                    return !imp->getEsteReturnata() && (imp->getZileScurse() > imp->getPerioadaZile());
                });

                std::cout << "-> Carti aflate la promotie:     " << cartiLaPromo << "\n";
                std::cout << "-> Carti exclusiv pentru adulti: " << cartiAdulti << "\n";
                std::cout << "-> Imprumuturi cu termen depasit: " << imprumuturiRestante << "\n";
                std::cout << "========================================\n\n";
                
                break;
            }
            case 'p': {
                std::string cnpModificare;
                int zileExtra;
                std::cout << "\n--- PRELUNGIRE ABONAMENT ---\n";
                std::cout << "Introduceti CNP-ul abonatului: ";
                std::cin >> cnpModificare;
                std::cout << "Introduceti numarul de zile de adaugat: ";
                std::cin >> zileExtra;

                biblioteca.prelungesteAbonament(cnpModificare, zileExtra);
                break;
            }
            case 'x':{
                std::cout << "Inchidere aplicatie." << std::endl;
                programInRulare = false;
                std::exit(0);
                break;
            }
            default:
                std::cout << "Comanda invalida! Reincercati." << std::endl;
        }
    }
}