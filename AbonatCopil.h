#ifndef ABONATCOPIL_H
#define ABONATCOPIL_H

#include "Abonati.h"

class AbonatCopil : public Abonati {
protected:
    void do_print(std::ostream& out) const override; // Interfata non-virtuala

public:
    AbonatCopil(const char* var_nume, const char* var_prenume, const char* var_cnp, const char* var_nrTelefon, int var_varsta, int var_durataAbonament);
    
    // Implementare clone
    AbonatCopil* clone() const override;

    // operator=
    AbonatCopil& operator=(const AbonatCopil& nou);

    // prietena
    friend void swap(AbonatCopil& first, AbonatCopil& second) noexcept;

    virtual ~AbonatCopil();
};

#endif