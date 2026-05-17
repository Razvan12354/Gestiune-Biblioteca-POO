#ifndef CARTI_H
#define CARTI_H

#include <iostream>

class Carti{
private:
    bool doarAdulti, laPromotie;
    char* titlu, *autor, *idCarte;
    int anAparitie, stocDisponibil;
    static int nrTotalCarti;

public:
    // Constructor
    Carti(const char *var_titlu, const char *var_autor, const char* var_idCarte, 
        int var_anAparitie, int var_stocDisponibil, bool adult, bool promo);
    
    Carti(const Carti& noua);

    // operator=
    Carti& operator=(const Carti& noua);

    // Getters + Metode ce inlocuiesc getterii
    bool rezervaCarte();
    void returneazaCarte();
    bool esteDisponibila() const;
    
    const char* getTitlu() const;

    const char* getId() const;

    int getStoc() const { return stocDisponibil; }

    static int getNrTotalCarti();

    const char* getAutor() const;

    int getAnAparitie() const;

    bool esteDoarAdulti() const { return doarAdulti; }
    bool esteLaPromotie() const { return laPromotie; }

    void scadeStoc();

    void elibereazaCarte();

    friend std::ostream& operator<<(std::ostream& out, const Carti& c);

    // Copy and swap
    friend void swap(Carti& first, Carti& second) noexcept;

    // Destructor
    ~Carti();
    };

#endif