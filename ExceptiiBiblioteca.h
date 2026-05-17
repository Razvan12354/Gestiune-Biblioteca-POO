#ifndef EXCEPTIIBIBLIOTECA_H
#define EXCEPTIIBIBLIOTECA_H

#include <exception>
#include <string>

class ExceptieBiblioteca : public std::exception {
protected:
    std::string mesaj;
public:
    ExceptieBiblioteca(const std::string& m): mesaj(m) {}
    virtual const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

// Exceptie pentru date de identificare invalide (CNP, Telefon)
class ExceptieValidare : public ExceptieBiblioteca {
public:
    ExceptieValidare(const std::string& m): ExceptieBiblioteca("Eroare de validare: " + m) {}
};

// Exceptie pentru stoc epuizat
class ExceptieStocEpuizat : public ExceptieBiblioteca {
public:
    ExceptieStocEpuizat(const std::string& m): ExceptieBiblioteca("Eroare de stoc: " + m) {}
};

// Exceptie pentru regulamentul bibliotecii (penalizari, abonament expirat)
class ExceptieRegulament : public ExceptieBiblioteca {
public:
    ExceptieRegulament(const std::string& m): ExceptieBiblioteca("Eroare de regulament: " + m) {}
}; 

// Daca un abonat copil incearca sa imprumute o carte pentru adulti
class ExceptieVarsta : public std::exception {
public:
    const char* what() const noexcept override {
        return "Acces interzis: Copiii nu pot imprumuta carti pentru adulti.";
    }
};

#endif
