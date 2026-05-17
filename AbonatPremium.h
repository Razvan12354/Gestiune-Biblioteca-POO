#ifndef ABONATPREMIUM_H
#define ABONATPREMIUM_H

#include "Abonati.h"

class AbonatPremium : public Abonati {
private:
    double taxaPremium;

protected:
    void do_print(std::ostream& out) const override; // Interfata non-virtuala

public:
    // Constructor
    AbonatPremium(const char* var_nume, const char* var_prenume, const char* var_cnp, const char* var_nrTelefon, 
                  int var_varsta, int var_durataAbonament, double var_taxaPremium);
    
    // Constructor virtual (clone)
    AbonatPremium* clone() const override;

    // Getters
    int getTaxaMentenanta() const { return taxaPremium; }

    // operator=
    AbonatPremium& operator=(const AbonatPremium& nou);

    friend void swap(AbonatPremium& first, AbonatPremium& second) noexcept;

    virtual ~AbonatPremium();
};

#endif