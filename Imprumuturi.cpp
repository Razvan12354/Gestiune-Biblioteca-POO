#include "Imprumuturi.h"
#include "ExceptiiBiblioteca.h"
#include "AbonatCopil.h"
#include <cstring>
#include <iostream>

float Imprumuturi::taxaPenalizarePeZi = 0.5;

// Constructor
Imprumuturi::Imprumuturi(Abonati& var_a, Carti& var_c, const char* var_dataImprumut, const int var_perioada)
    : abonat(var_a) {
    
    AbonatCopil* ptrCopil = dynamic_cast<AbonatCopil*>(&var_a);
    if (ptrCopil != nullptr && var_c.esteDoarAdulti()) {
        throw ExceptieVarsta(); 
    }

    // Alocare si copiere pentru campurile char*
    this->CNP = new char[strlen(var_a.getCNP()) + 1];
    strcpy(this->CNP, var_a.getCNP());

    this->idCarte = new char[strlen(var_c.getId()) + 1];
    strcpy(this->idCarte, var_c.getId());

    this->dataImprumut = new char[strlen(var_dataImprumut) + 1];
    strcpy(this->dataImprumut, var_dataImprumut);

    // Daca cartea e la promotie are termen 21 de zile in loc de 14
    if (var_c.esteLaPromotie()) {
        this->perioadaZile = 21; // Primeste 21 de zile
        std::cout << "\n>>> [PROMO] '" << var_c.getTitlu() << "' este la promotie! Termenul a fost extins la 21 de zile. <<<" << std::endl;
    } else {
        this->perioadaZile = var_perioada; // Ramane perioada standard (14 zile)
    }

    // Initializare pointeri si variabile simple
    this->perioadaZile = var_perioada;
    this->esteReturnata = false;
    this->carteImprumutata = &var_c;  // Legam pointerul de obiectul primit
    this->abonatAsociat = &var_a;    // Legam pointerul de obiectul primit
    this->zileScurse = 0;

    // Scadere stoc
    if (!this->carteImprumutata->rezervaCarte()) {
        delete[] this->CNP;
        delete[] this->idCarte;
        delete[] this->dataImprumut;
        throw ExceptieStocEpuizat("Stoc epuizat pentru aceasta carte.");
    }
}

// Constructor de copiere
Imprumuturi::Imprumuturi(const Imprumuturi& nou)
    : abonat(nou.abonat) {

    this->CNP = new char[strlen(nou.CNP) + 1];
    strcpy(this->CNP, nou.CNP);

    this->idCarte = new char[strlen(nou.idCarte) + 1];
    strcpy(this->idCarte, nou.idCarte);

    this->dataImprumut = new char[strlen(nou.dataImprumut) + 1];
    strcpy(this->dataImprumut, nou.dataImprumut);

    this->perioadaZile = nou.perioadaZile;
    this->esteReturnata = nou.esteReturnata;
    this->carteImprumutata = nou.carteImprumutata;
    this->abonatAsociat = nou.abonatAsociat;
}

const char* Imprumuturi::getIdCarte() const {
    if (this->carteImprumutata != nullptr) {
        return this->carteImprumutata->getId();
    }
    return "";
}

int Imprumuturi::getPerioadaZile() const {
    return this->perioadaZile;
}

double Imprumuturi::getTaxaPenalizare() const {
    return this->taxaPenalizarePeZi;
}

const char* Imprumuturi::getDataImprumut() const {
    return this->dataImprumut;
}

// Destructor
Imprumuturi::~Imprumuturi() {
    delete[] CNP;
    delete[] idCarte;
    delete[] dataImprumut;
}

// Finalizare imprumut
void Imprumuturi::finalizeazaImprumut(int var_zileReale) {
    if (esteReturnata) return;

    esteReturnata = true;
    if (carteImprumutata) {
        carteImprumutata->returneazaCarte();
    }

    if (var_zileReale > perioadaZile) {
        int zileIntarziere = var_zileReale - perioadaZile;
        StrategiePenalizare* strategie = nullptr;

        if (abonatAsociat->getScorIncredere() < 50) {
            strategie = new PenalizareStandard();
        } else {
            strategie = new PenalizareRedusa();
        }
        
        double penalizare = strategie->calculeazaPenalizare(zileIntarziere, taxaPenalizarePeZi);
        delete strategie;
        
        abonatAsociat->adaugaPenalizare(penalizare);
        std::cout << "Penalizare de " << penalizare << " RON aplicata pentru " << CNP << std::endl;
    }
}

// Operator=
Imprumuturi& Imprumuturi::operator=(const Imprumuturi& nou) {
    if (this != &nou) {
        delete[] CNP;
        delete[] idCarte;
        delete[] dataImprumut;

        this->CNP = new char[strlen(nou.CNP) + 1];
        strcpy(this->CNP, nou.CNP);

        this->idCarte = new char[strlen(nou.idCarte) + 1];
        strcpy(this->idCarte, nou.idCarte);

        this->dataImprumut = new char[strlen(nou.dataImprumut) + 1];
        strcpy(this->dataImprumut, nou.dataImprumut);

        this->perioadaZile = nou.perioadaZile;
        this->esteReturnata = nou.esteReturnata;
        this->carteImprumutata = nou.carteImprumutata;
        this->abonatAsociat = nou.abonatAsociat;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Imprumuturi& i) {
    out << "Imprumut: " << i.CNP << " | Carte: " << i.idCarte << " | Data: " << i.dataImprumut;
    return out;
}