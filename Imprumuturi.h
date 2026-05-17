#ifndef IMPRUMUTURI_H
#define IMPRUMUTURI_H

#include <iostream>
#include "Carti.h"
#include "Abonati.h"

class Imprumuturi{
private:
    char *CNP, *idCarte, *dataImprumut;
    int perioadaZile;
    bool esteReturnata;
    Carti *carteImprumutata;
    Abonati *abonatAsociat;
    Abonati& abonat;
    static float taxaPenalizarePeZi;
    int zileScurse;

public:
    // Constructor
    Imprumuturi(Abonati& var_a, Carti& var_c, const char* var_dataImprumut, const int var_perioada);

    Imprumuturi(const Imprumuturi& nou);


    ~Imprumuturi();

    // Getters + Setters
    const char* getCNP() const;

    const char* getIdCarte() const;

    const char* getDataImprumut() const;

    double getTaxaPenalizare() const;

    int getPerioadaZile() const;

    int getZileScurse() const { return zileScurse; }

    void setZileScurse(int nr) { zileScurse = nr; }

    // Finalizare imprumut + supraincarcare

    void finalizeazaImprumut();
    void finalizeazaImprumut(int var_zileReale);

    Abonati& getAbonat() { return abonat; }

    static void setTaxaPenalizare(float var_taxa);

    friend std::ostream& operator<<(std::ostream& out, const Imprumuturi& i);

    // Copy and swap
    friend void swap(Imprumuturi& first, Imprumuturi& second) noexcept;
    Imprumuturi& operator=(const Imprumuturi& nou);

    // Crestere counter zile scurse de cand a fost facut imprumutul
    void crestereZileScurse(int nr) { zileScurse += nr; }
};

#endif