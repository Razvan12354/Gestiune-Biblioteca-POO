#ifndef ABONATI_H
#define ABONATI_H

#include <iostream>

class Abonati{
protected:
    char*nume, *prenume, *telefon, *CNP;
    int varsta, durataAbonament, nrCartiImprumutate;
    double balantaPenalizari;
    static int nrTotalAbonati;
    int scorIncredere;
    virtual void do_print(std::ostream& out) const = 0;


public:
    // Constructori
    Abonati(const char *var_nume, const char *var_prenume, const char *var_CNP, 
            const char *var_telefon, int var_varsta, int var_durataAbonament);
    Abonati(const Abonati& nou);

    // Operator=
    Abonati& operator=(const Abonati& nou);

    // Desctructor Virtual
    virtual ~Abonati();

    // Constructor virtual (clone)
    virtual Abonati* clone() const = 0; // = 0 inseamna functie pur virtuala


    void afisare(std::ostream& out) const {
        do_print(out);
    }

    // Getters + Setters
    char* getCNP() const { return CNP; }
    char* getNume() const { return nume; }
    char* getPrenume() const { return prenume; }
    char* getTelefon() const { return telefon; }
    int getVarsta() const { return varsta; }
    int getNrCartiImprumutate() const { return nrCartiImprumutate; }
    double getBalantaPenalizari() const { return balantaPenalizari; }
    int getDurataAbonament() const { return durataAbonament; }
    void setNrCartiImprumutate(int nr) { nrCartiImprumutate = nr; }
    int getScorIncredere() const { return scorIncredere; }
    void setScorIncredere(int s) { scorIncredere = s; }

    // Metode 
    void adaugaPenalizare(double suma);
    static bool telefonValid(const char* tel);
    static bool CNPValid(const char* cnp);
    void platestePenalizare(double suma);
    void prelungireAbonament(int luni);
    bool poateImprumuta() const;
    void trecereTimp(int luni);
    void modificaScor(int puncte);

    virtual double calculScorSpecific() const { return (double)scorIncredere; }

    friend std::ostream& operator<<(std::ostream& out, const Abonati& a);

    // Copy and swap
    friend void swap(Abonati& first, Abonati& second) noexcept;
};

#endif