#ifndef ABONATPENSIONAR_H
#define ABONATPENSIONAR_H
#include "Abonati.h"

class AbonatPensionar : public Abonati {
protected:
    void do_print(std::ostream& out) const override {
        out << "Abonat PENSIONAR | " << nume << " " << prenume;
    }
public:
    using Abonati::Abonati; // Mosteneste constructorii din clasa Abonati
    AbonatPensionar* clone() const override {
        return new AbonatPensionar(*this);
    }
};
#endif