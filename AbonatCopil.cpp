#include "AbonatCopil.h"

// Constructor apelare baza
AbonatCopil::AbonatCopil(const char* var_nume, const char* var_prenume, const char* var_cnp, const char* var_nrTelefon, int var_varsta, int var_durataAbonament) 
    : Abonati(var_nume, var_prenume, var_cnp, var_nrTelefon, var_varsta, var_durataAbonament) {}

// Implementarea clone()
AbonatCopil* AbonatCopil::clone() const {
    return new AbonatCopil(*this);
}

// Afisare specifica
void AbonatCopil::do_print(std::ostream& out) const {
    out << "Abonat COPIL   | " << nume << " " << prenume 
        << " | Varsta: " << varsta << " ani ";
}

void swap(AbonatCopil& first, AbonatCopil& second) noexcept {
    swap(static_cast<Abonati&>(first), static_cast<Abonati&>(second));
}

AbonatCopil& AbonatCopil::operator=(const AbonatCopil& nou) {
    if (this != &nou) {
        AbonatCopil copie(nou);
        swap(*this, copie);
    }
    return *this;
}

AbonatCopil::~AbonatCopil() {
    std::cout << "[Destructor AbonatCopil] Obiect distrus." << std::endl;
}