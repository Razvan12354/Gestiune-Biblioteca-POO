#ifndef ABONATSTUDENT_H
#define ABONATSTUDENT_H

#include "Abonati.h"

class AbonatStudent : public Abonati{
private:
    char* universitate;
protected:
    void do_print(std::ostream& out) const override; // Interfata non-virtuala
public:
    // Constructor care primeste parametri pentru baza si pentru derivata
    AbonatStudent(const char* var_nume, const char* var_prenume, const char* var_cnp, const char* var_nrTelefon,
                  int var_varsta, int var_durataAbonament, const char* var_universitate);

    // Constructor de copiere
    AbonatStudent(const AbonatStudent& nou);

    // Getters
    char* getUniversitate() const { return universitate; }

    // operator=
    AbonatStudent& operator=(const AbonatStudent& nou);

    // Destructor
    ~AbonatStudent();

    // Constructor virtual (clone)
    AbonatStudent* clone() const override;

    // Copy and swap
    friend void swap(AbonatStudent& first, AbonatStudent& second) noexcept;
};

#endif