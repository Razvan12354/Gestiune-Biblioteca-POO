#include "Abonati.h"
#include "ExceptiiBiblioteca.h"
#include <cstring>

int Abonati::nrTotalAbonati = 0;

// Constructor cu parametri
Abonati::Abonati(const char *var_nume, const char *var_prenume, const char *var_CNP, 
                 const char *var_telefon, int var_varsta, int var_durataAbonament){
    
    if (!CNPValid(var_CNP)) {
        throw ExceptieValidare("CNP-ul trebuie sa aiba exact 13 caractere!");
    }

    if (!telefonValid(var_telefon)) {
        throw ExceptieValidare("Numarul de telefon trebuie sa aiba 10 cifre!");
    }

    nume = new char[strlen(var_nume) + 1];
    strcpy(nume,var_nume);

    prenume = new char[strlen(var_prenume)+1];
    strcpy(prenume,var_prenume);

    CNP = new char[strlen(var_CNP) + 1];
    strcpy(CNP, var_CNP);

    telefon = new char[strlen(var_telefon) + 1];
    strcpy(telefon, var_telefon);

    varsta = var_varsta;
    durataAbonament = var_durataAbonament;
    nrCartiImprumutate = 0;
    balantaPenalizari = 0.0;
    nrTotalAbonati++;
    this->scorIncredere = 60;
}

// Constructor de copiere
Abonati::Abonati(const Abonati& nou) {
    nume = new char[strlen(nou.nume) + 1];
    strcpy(nume, nou.nume);
    prenume = new char[strlen(nou.prenume) + 1];
    strcpy(prenume, nou.prenume);
    CNP = new char[strlen(nou.CNP) + 1];
    strcpy(CNP, nou.CNP);
    telefon = new char[strlen(nou.telefon) + 1];
    strcpy(telefon, nou.telefon);
    
    varsta = nou.varsta;
    durataAbonament = nou.durataAbonament;
    nrCartiImprumutate = nou.nrCartiImprumutate;
    balantaPenalizari = nou.balantaPenalizari;
    nrTotalAbonati++;
    this->scorIncredere = nou.scorIncredere;
}


// Validare telefon & CNP
bool Abonati::telefonValid(const char* tel) {
    return (tel && strlen(tel) == 10);
}

bool Abonati::CNPValid(const char* cnp) {
    return (cnp && strlen(cnp) == 13);
}

// Logica gestionare penalizari
void Abonati::adaugaPenalizare(double suma) {
    this->balantaPenalizari += suma;
    std::cout << "Balanta de penalizari a abonatului " << nume << " " << prenume << ", cu CNP " << CNP << ", a fost incarcata cu suma de " << suma << " RON. Balanta de penalizari curenta: " << balantaPenalizari << " RON." << std::endl;
}

void Abonati::platestePenalizare(double suma) {
    if (suma > balantaPenalizari) {
        std::cout << "Suma prea mare! Balanta actuala: " << balantaPenalizari << std::endl;
    } else {
        this->balantaPenalizari -= suma;
        std::cout << "Plata efectuata. Balanta ramasa: " << balantaPenalizari << std::endl;
    }
}

// Logica abonament
void Abonati::prelungireAbonament(int luni) {
    durataAbonament += luni;
    std::cout << "Abonament prelungit cu " << luni << " luni. Durata abonament curenta: " << durataAbonament << " luni." << std::endl;
}

bool Abonati::poateImprumuta() const {
    if (balantaPenalizari > 0) return false;
    if (durataAbonament <= 0) return false;
    return true;
}

// operator<<
std::ostream& operator<<(std::ostream& out, const Abonati& a) {
    a.afisare(out); // apeleaza do_print() din derivata
    return out;
}

// Trecere timp (scade durata abonamentului)
void Abonati::trecereTimp(int zile) {
    if (this->durataAbonament >= zile) {
        this->durataAbonament -= zile;
        std::cout << "Au trecut " << zile << " zile. Durata ramasa pentru " << nume << ": " << durataAbonament << " luni." << std::endl;
    } else {
        this->durataAbonament = 0;
        std::cout << "Abonamentul pentru " << nume << " a expirat!" << std::endl;
    }
}

// Modificare scor incredere
void Abonati::modificaScor(int puncte) {
    scorIncredere += puncte;
    if (scorIncredere > 100) scorIncredere = 100;
    if (scorIncredere < 0) scorIncredere = 0;
}

// Copy and swap
void swap(Abonati& first, Abonati& second) noexcept {
    char* tempNume = first.nume;
    first.nume = second.nume;
    second.nume = tempNume;

    char* tempPrenume = first.prenume;
    first.prenume = second.prenume;
    second.prenume = tempPrenume;

    char* tempCNP = first.CNP;
    first.CNP = second.CNP;
    second.CNP = tempCNP;

    char* tempTel = first.telefon;
    first.telefon = second.telefon;
    second.telefon = tempTel;

    int tempVarsta = first.varsta;
    first.varsta = second.varsta;
    second.varsta = tempVarsta;

    int tempDurata = first.durataAbonament;
    first.durataAbonament = second.durataAbonament;
    second.durataAbonament = tempDurata;

    int tempCarti = first.nrCartiImprumutate;
    first.nrCartiImprumutate = second.nrCartiImprumutate;
    second.nrCartiImprumutate = tempCarti;

    double tempBalanta = first.balantaPenalizari;
    first.balantaPenalizari = second.balantaPenalizari;
    second.balantaPenalizari = tempBalanta;
}

// operator=
Abonati& Abonati::operator=(const Abonati& nou) {
    if (this != &nou) {
        delete[] nume;
        delete[] prenume;
        delete[] CNP;
        delete[] telefon;

        nume = new char[strlen(nou.nume) + 1];
        strcpy(nume, nou.nume);

        prenume = new char[strlen(nou.prenume) + 1];
        strcpy(prenume, nou.prenume);

        CNP = new char[strlen(nou.CNP) + 1];
        strcpy(CNP, nou.CNP);

        telefon = new char[strlen(nou.telefon) + 1];
        strcpy(telefon, nou.telefon);

        varsta = nou.varsta;
        durataAbonament = nou.durataAbonament;
        nrCartiImprumutate = nou.nrCartiImprumutate;
        balantaPenalizari = nou.balantaPenalizari;
        this->scorIncredere = nou.scorIncredere;
    }
    return *this;
}

Abonati::~Abonati() {
    std::cout << "[Destructor Abonati] Se elibereaza: " << this->nume << std::endl;
    delete[] this->nume;
    delete[] this->prenume;
    delete[] this->CNP;
    delete[] this->telefon;
    nrTotalAbonati--;
}