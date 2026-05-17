#include "Carti.h"
#include <cstring>

int Carti::nrTotalCarti = 0;

// Constructor
Carti::Carti(const char* var_titlu, const char* var_autor, const char* var_idCarte, int var_anAparitie, int var_stocDisponibil, bool adult, bool promo){
    this->titlu = new char[strlen(var_titlu)+1];
    strcpy(this->titlu,var_titlu);

    this->autor = new char[strlen(var_autor)+1];
    strcpy(this->autor,var_autor);

    this->idCarte = new char[strlen(var_idCarte)+1];
    strcpy(this->idCarte,var_idCarte);

    this->anAparitie = var_anAparitie;

    this->stocDisponibil = var_stocDisponibil;

    this->doarAdulti = adult;

    this->laPromotie = promo;

    nrTotalCarti++;
}

// Constructor de copiere
Carti::Carti(const Carti& noua){
    this->titlu = new char[strlen(noua.titlu)+1];
    strcpy(this->titlu,noua.titlu);

    this->autor = new char[strlen(noua.autor)+1];
    strcpy(this->autor,noua.autor);

    this->idCarte = new char[strlen(noua.idCarte)+1];
    strcpy(this->idCarte,noua.idCarte);

    this->anAparitie = noua.anAparitie;

    this->stocDisponibil = noua.stocDisponibil;

    this->doarAdulti = noua.doarAdulti;

    this->laPromotie = noua.laPromotie;

    nrTotalCarti++;
}

// Copy and swap
void swap(Carti& first, Carti& second) noexcept {
    char* tempTitlu = first.titlu;
    first.titlu = second.titlu;
    second.titlu = tempTitlu;

    char* tempAutor = first.autor;
    first.autor = second.autor;
    second.autor = tempAutor;

    char* tempId = first.idCarte;
    first.idCarte = second.idCarte;
    second.idCarte = tempId;

    int tempAn = first.anAparitie;
    first.anAparitie = second.anAparitie;
    second.anAparitie = tempAn;

    int tempStoc = first.stocDisponibil;
    first.stocDisponibil = second.stocDisponibil;
    second.stocDisponibil = tempStoc;
}

// operator=
Carti& Carti::operator=(const Carti& noua) {
    Carti copie(noua);
    swap(*this, copie);
    return *this;
}

// Rezervare carte
bool Carti::rezervaCarte() {
    if (this->stocDisponibil > 0) {
        this->stocDisponibil--;
        std::cout << "Rezervare reusita pentru cartea " << this->titlu << ". Stoc ramas: " << this->stocDisponibil << std::endl;
        return true;
    } else {
        std::cout << "Rezervare esuata pentru cartea " << this->titlu << ". Stoc epuizat!" << std::endl;
        return false;        
}}

// Returnare carte
void Carti::returneazaCarte() {
    this->stocDisponibil++;
    std::cout << "Cartea " << this->titlu << " a fost returnata. Stoc nou: " << this->stocDisponibil << std::endl;
}

// Verificare disponibilitate carte
bool Carti::esteDisponibila() const {
    if (this->stocDisponibil > 0) {
        std::cout << "Cartea " << this->titlu << " este disponibila pentru imprumut. Stoc curent: " << this->stocDisponibil << std::endl;
        return true;
    }
    std::cout << "Cartea " << this->titlu << " nu este disponibila pentru imprumut. Stoc curent: " << this->stocDisponibil << std::endl;
    return false;
}

// Getters
const char* Carti::getTitlu() const {
    return this->titlu;
}

const char* Carti::getId() const {
    return this->idCarte;
}

const char* Carti::getAutor() const {
    return this->autor;
}

int Carti::getAnAparitie() const {
    return this->anAparitie;
}

int Carti::getNrTotalCarti() {
    return nrTotalCarti;
}

void Carti::scadeStoc() {
    if (this->stocDisponibil > 0) {
        this->stocDisponibil--;
    }
}

void Carti::elibereazaCarte() {
    this->stocDisponibil++;
}

// Afisare operator<<
std::ostream& operator<<(std::ostream& out, const Carti& c) {
    out << "Carte [ID: " << c.idCarte << "]: \"" << c.titlu 
        << "\" de " << c.autor << " | An: " << c.anAparitie 
        << " | Stoc: " << c.stocDisponibil;
    return out;
}

// Destructor
Carti::~Carti() {
    std::cout << "[Destructor Carti] Se elibereaza: " << this->titlu << std::endl;
    delete[] this->titlu;
    delete[] this->autor;
    delete[] this->idCarte;
    nrTotalCarti--;
}