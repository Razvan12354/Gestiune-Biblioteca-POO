#include "GestiuneAbonati.h"
#include "AbonatStudent.h"
#include "AbonatCopil.h"
#include "AbonatPremium.h"
#include "AbonatNormal.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>

// Fabrica declarata local pentru a fi recunoscuta in metodele de mai jos
class AbonatFactory {
public:
    static Abonati* creeazaAbonat(char tip, const char* nume, const char* prenume, const char* cnp, const char* tel, 
                                  int varsta, int durata, const std::string& universitate, double taxa) {
        switch (tip) {
            case 'C': return new AbonatCopil(nume, prenume, cnp, tel, varsta, durata);
            case 'S': return new AbonatStudent(nume, prenume, cnp, tel, varsta, durata, universitate.c_str());
            case 'P': return new AbonatPremium(nume, prenume, cnp, tel, varsta, durata, taxa);
            case 'N': return new AbonatNormal(nume, prenume, cnp, tel, varsta, durata); 
            default:  return nullptr;
        }
    }
};

// Cautare abonat dupa CNP
Abonati* GestiuneAbonati::gasesteAbonat(const char* cnp) {
    for (auto a : lista) {
        if (std::strcmp(a->getCNP(), cnp) == 0) return a;
    }
    return nullptr;
}

// Cautare carte dupa ID
Carti* GestiuneAbonati::gasesteCarte(const char* id) {
    return catalogCarti.cautaElement(id, [](Carti* c, const char* valoareId) {
        return std::strcmp(c->getId(), valoareId) == 0;
    });
}

// Gaseste un imprumut dupa ID carte
Imprumuturi* GestiuneAbonati::gasesteImprumutDupaCarte(const char* idCarte) {
    return catalogImprumuturi.cautaElement(idCarte, [](Imprumuturi* imp, const char* valoareId) {
        return std::strcmp(imp->getIdCarte(), valoareId) == 0;
    });
}

// Scade o zi din durata abonamentului tuturor
void GestiuneAbonati::proceseazaFinalDeZi() {
    std::cout << "--- PROCESARE ZILNICA ---" << std::endl;
    for (auto a : catalogImprumuturi.getElemente()) {
        if (a != nullptr) {
            a->crestereZileScurse(1);
        }
    }
}

// Aplica promotie studenti folosind dynamic_cast
void GestiuneAbonati::aplicaPromotieStudenti(int zileExtra) {
    std::cout << "Se cauta studenti pentru prelungire..." << std::endl;
    for (auto p : lista) {
        AbonatStudent* s = dynamic_cast<AbonatStudent*>(p);
        if (s != nullptr) {
            s->prelungireAbonament(zileExtra);
            std::cout << "S-au adaugat " << zileExtra << " luni pentru: " << s->getNume() << std::endl;
        }
    }
}
// Sterge balanta de penalizari a unui abonat dupa CNP
void GestiuneAbonati::stergePenalizariAbonat(const std::string& cnpCautat) {
    bool gasit = false;
    for (auto p : lista) {
        if (p != nullptr && p->getCNP() == cnpCautat) {
            p->reseteazaPenalizari();
            std::cout << "\nPenalizarile abonatului cu CNP " << cnpCautat << " au fost sterse. (Sold curent: 0 RON)\n";
            gasit = true;
            break;
        }
    }
    if (gasit) {
        this->salveazaAbonatiInFisier("abonati.txt");
    }
    else{
        std::cout << "\n[EROARE] Nu s-a gasit niciun abonat cu CNP-ul specificat.\n";
    }
}

// Trecere timp
void GestiuneAbonati::trecereTimp(int zile) {

    for (auto p : lista) {
        if (p != nullptr) {
            p->trecereTimp(zile); 
        }
    }

    for (auto imp : getListaImprumuturi()) {
        if (imp != nullptr) {
            imp->setZileScurse(imp->getZileScurse() + zile);

    }
    }
    this->salveazaAbonatiInFisier("abonati.txt");
    this->salveazaImprumuturi("imprumuturi.txt");
}

// Incarcare carti din fisier
void GestiuneAbonati::incarcaCarti(const std::string& numeFisier) {
    std::ifstream f(numeFisier);
    if (!f.is_open()) return;

    std::string titlu, autor, id;
    int an, stoc;
    int adult, promo; 

    while (f >> std::quoted(titlu) >> std::quoted(autor) >> id >> an >> stoc >> adult >> promo) {
        catalogCarti.getElemente().push_back(new Carti(titlu.c_str(), autor.c_str(), id.c_str(), an, stoc, (bool)adult, (bool)promo));
    }
    f.close();
}

// Incarcare imprumuturi din fisier
void GestiuneAbonati::incarcaImprumuturi(const std::string& numeFisier) {
    std::ifstream f(numeFisier);
    if (!f.is_open()) return;

    std::string cnp, idCarte, data;
    int perioada, scurse;

    while (f >> cnp >> idCarte >> data >> perioada >> scurse) {
        Abonati* ab = gasesteAbonat(cnp.c_str());
        Carti* ct = gasesteCarte(idCarte.c_str());

        if (ab && ct) {
            Imprumuturi* nou = new Imprumuturi(*ab, *ct, data.c_str(), perioada);
            nou->setZileScurse(scurse);
            this->catalogImprumuturi.getElemente().push_back(nou);
        }
    }
    f.close();
}

// Incarcare abonati folosind Factory Method
void GestiuneAbonati::incarcaDinFisier(const std::string& numeFisier) {
    std::ifstream f(numeFisier);
    if (!f.is_open()) return;

    char tip;
    while (f >> tip) {
        std::string n, p, cnp, tel;
        int v, d, scor;
        double balanta;

        if (!(f >> std::quoted(n) >> std::quoted(p) >> cnp >> tel >> v >> d >> balanta >> scor)) {
            continue; 
        }

        std::string universitate = "";
        double taxa = 0.0;
        if (tip == 'S') {
            f >> std::quoted(universitate);
        } else if (tip == 'P') {
            f >> taxa;
        }

        Abonati* nou = nullptr;
        try {
            nou = AbonatFactory::creeazaAbonat(tip, n.c_str(), p.c_str(), cnp.c_str(), tel.c_str(), v, d, universitate, taxa);

            if (nou) {
                nou->setScorIncredere(scor);
                nou->adaugaPenalizare(balanta);
                this->lista.push_back(nou);
            }
        } catch (const std::exception& e) {
            std::cerr << "Eroare la incarcarea abonatului " << n << ": " << e.what() << std::endl;
        }
    }
    f.close();
}

// Adaugare abonat in lista prin clonare polimorfica
void GestiuneAbonati::adaugaAbonat(Abonati* a) {
    lista.push_back(a);
}

// Afisarea tuturor abonatilor
void GestiuneAbonati::afiseazaToti() const {
    for (size_t i = 0; i < lista.size(); ++i) {
        std::cout << *lista[i] << std::endl;
    }
}

// Salvare imprumuturi in fisier
void GestiuneAbonati::salveazaImprumuturi(const std::string& numeFisier) {
    std::ofstream f(numeFisier);
    if (!f.is_open()) return;

    for (auto imp : catalogImprumuturi.getElemente()) {
        f << imp->getAbonat().getCNP() << " " 
          << imp->getIdCarte() << " "
          << imp->getDataImprumut() << " "
          << imp->getPerioadaZile() << " "
          << imp->getZileScurse() << "\n";
    }
    f.close();
}

// Adaugare a unui imprumut in fisier (append)
void GestiuneAbonati::adaugaImprumutInFisier(Imprumuturi* imp, const std::string& numeFisier) {
    std::ofstream f(numeFisier, std::ios::app);
    if (!f.is_open()) return;

    f << imp->getAbonat().getCNP() << " " 
      << imp->getIdCarte() << " " 
      << imp->getDataImprumut() << " "
      << imp->getPerioadaZile() << " "
      << imp->getZileScurse() << "\n";
    f.close();
}

// Salvare carti in fisier
void GestiuneAbonati::salveazaCartiInFisier(const std::string& numeFisier) {
    std::ofstream f(numeFisier);
    if (!f.is_open()) return;

    for (auto c : catalogCarti.getElemente()) {
        f << std::quoted(c->getTitlu()) << " " 
          << std::quoted(c->getAutor()) << " " 
          << c->getId() << " " 
          << c->getAnAparitie() << " "
          << c->getStoc() << " " 
          << (c->esteDoarAdulti() ? 1 : 0) << " " 
          << (c->esteLaPromotie() ? 1 : 0) << "\n";
    }
    f.close(); 
}

// Salvare abonati in fisier (cu dynamic_cast pentru a scrie campurile specifice fiecarui tip)
void GestiuneAbonati::salveazaAbonatiInFisier(const std::string& numeFisier) {
    std::ofstream f(numeFisier);
    if (!f.is_open()) return;

    for (auto a : lista) {
        char tip = 'R'; 
        if (dynamic_cast<AbonatStudent*>(a)) tip = 'S';
        else if (dynamic_cast<AbonatCopil*>(a)) tip = 'C';
        else if (dynamic_cast<AbonatPremium*>(a)) tip = 'P';
        else if (dynamic_cast<AbonatNormal*>(a)) tip = 'N';

        f << tip << " "
          << std::quoted(a->getNume()) << " "
          << std::quoted(a->getPrenume()) << " "
          << a->getCNP() << " "
          << a->getTelefon() << " "
          << a->getVarsta() << " "
          << a->getDurataAbonament() << " "
          << a->getBalantaPenalizari() << " "
          << a->getScorIncredere() << " ";

        if (tip == 'S') {
            AbonatStudent* s = dynamic_cast<AbonatStudent*>(a);
            f << std::quoted(s->getUniversitate());
        } else if (tip == 'P') {
            AbonatPremium* p = dynamic_cast<AbonatPremium*>(a);
            f << p->getTaxaMentenanta();
        }
        f << "\n";
    }
    f.close();
}

// Downgrade de la Premium la Normal
void GestiuneAbonati::DowngradePremium(const std::string& cnpCautat) {
    int indexGasit = -1;
    
    // Cauta abonatul in lista
    for (size_t i = 0; i < lista.size(); ++i) {
        if (lista[i] != nullptr && lista[i]->getCNP() != nullptr && 
            cnpCautat == lista[i]->getCNP()) {
            indexGasit = i;
            break;
        }
    }

    if (indexGasit == -1) {
        std::cout << "\n[EROARE] Nu s-a gasit niciun abonat cu CNP-ul: " << cnpCautat << "\n";
        return;
    }

    Abonati* vechiulAbonat = lista[indexGasit];

    // Verifica daca abonatul e premium
    if (dynamic_cast<AbonatPremium*>(vechiulAbonat) == nullptr) {
        std::cout << "\n[AVERTISMENT] Abonatul cu CNP-ul " << cnpCautat << " nu este de tip Premium. Downgrade anulat.\n";
        return;
    }

    // Extrage toate datele din obiectul vechi folosind getterii din Abonati.h
    std::string nume = vechiulAbonat->getNume();
    std::string prenume = vechiulAbonat->getPrenume();
    std::string telefon = vechiulAbonat->getTelefon();
    int varsta = vechiulAbonat->getVarsta();
    int durata = vechiulAbonat->getDurataAbonament();
    double penalizariVechi = vechiulAbonat->getBalantaPenalizari();
    int scorVechi = vechiulAbonat->getScorIncredere();
    int nrCarti = vechiulAbonat->getNrCartiImprumutate();

    // 4. Construieste noul obiect de tip Normal ('N')
    Abonati* abonatNou = AbonatFactory::creeazaAbonat('N', nume.c_str(), prenume.c_str(), 
                                                       cnpCautat.c_str(), telefon.c_str(), 
                                                       varsta, durata, "", 0.0);

    if (abonatNou != nullptr) {
        // Pastreaza scorul de incredere, numarul de carti imprumutate si penalizarile
        abonatNou->setScorIncredere(scorVechi);
        abonatNou->setNrCartiImprumutate(nrCarti);
        
        if (penalizariVechi > 0) {
            abonatNou->adaugaPenalizare(penalizariVechi);
        }

        // Curata memoria obiectului vechi si il inlocuieste in lista
        delete vechiulAbonat;
        lista[indexGasit] = abonatNou;

        std::cout << "\n[SUCCES] Abonatul Premium cu CNP " << cnpCautat << " a fost transformat in Abonat Normal.\n";

        this->salveazaAbonatiInFisier("abonati.txt");
    } else {
        std::cout << "\n[EROARE] Eroare la downgrade.\n";
    }
}

//Prelungeste abonamentul lui abonat cu un numar de zile specificat
void GestiuneAbonati::prelungesteAbonament(const std::string& cnpCautat, int nrZile) {
    if (nrZile <= 0) {
        std::cout << "\n[EROARE] Numarul de zile trebuie sa fie pozitiv.\n";
        return;
    }

    Abonati* abonat = gasesteAbonat(cnpCautat.c_str());

    if (abonat == nullptr) {
        std::cout << "\n[EROARE] Nu s-a gasit niciun abonat cu CNP-ul: " << cnpCautat << "\n";
        return;
    }

    abonat->prelungireAbonament(nrZile);

    std::cout << "\n[SUCCES] Abonamentul lui " << abonat->getNume() << " " << abonat->getPrenume() 
              << " a fost prelungit cu " << nrZile << " zile.\n";

    this->salveazaAbonatiInFisier("abonati.txt");
}

// Destructor
GestiuneAbonati::~GestiuneAbonati() {
    for (Abonati* a : lista) {
        delete a;
    }
    lista.clear();
}