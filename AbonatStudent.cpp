#include "AbonatStudent.h"
#include <cstring>


// Constructor
AbonatStudent::AbonatStudent(const char* var_nume, const char* var_prenume, const char* var_cnp, const char* var_nrTelefon,
                         int var_varsta, int var_durataAbonament, const char* var_universitate) : Abonati(var_nume, var_prenume, var_cnp, var_nrTelefon, var_varsta, var_durataAbonament) {
    universitate = new char[strlen(var_universitate) + 1];
    strcpy(universitate, var_universitate);
}

//  Constructor de copiere
AbonatStudent::AbonatStudent(const AbonatStudent& nou) : Abonati(nou) {
    universitate = new char[std::strlen(nou.universitate) + 1];
    std::strcpy(universitate, nou.universitate);
}


// Copy and swap
void swap(AbonatStudent& first, AbonatStudent& second) noexcept {
    swap(static_cast<Abonati&>(first), static_cast<Abonati&>(second));

    char* tempUni = first.universitate;
    first.universitate = second.universitate;
    second.universitate = tempUni;
}

// Operator=
AbonatStudent& AbonatStudent::operator=(const AbonatStudent& nou) {
    AbonatStudent copie(nou);

    swap(*this, copie);
    
    return *this;
}


// Clone
AbonatStudent* AbonatStudent::clone() const {
    return new AbonatStudent(*this);
}

// Afisare operator<<
void AbonatStudent::do_print(std::ostream& out) const {
    out << "Abonat STUDENT | " << nume << " " << prenume 
        << " | Universitate: " << universitate;
}

// Destructor
AbonatStudent::~AbonatStudent() {
    std::cout << "[Destructor AbonatStudent] Se elibereaza universitatea: " << this->universitate << std::endl;
    delete[] this->universitate;
}