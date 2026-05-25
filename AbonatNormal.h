#ifndef ABONATNORMAL_H
#define ABONATNORMAL_H
#include "Abonati.h"

class AbonatNormal : public Abonati {
protected:
    void do_print(std::ostream& out) const override {
        out << "Abonat NORMAL | " << nume << " " << prenume;
    }
public:
    using Abonati::Abonati; // Mosteneste constructorii din clasa Abonati
    AbonatNormal* clone() const override {
        return new AbonatNormal(*this);
    }
};
#endif