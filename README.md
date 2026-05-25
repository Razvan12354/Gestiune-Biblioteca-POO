Sistem de Gestiune a Activitatii unei Biblioteci

 Proiectul este o aplicatie creata pentru a gestiona toate activitatile care au loc intr-o biblioteca moderna. Intr-o biblioteca reala exista mii de carti care trebuie tinute in evidenta, mii de abonati care vin si pleaca, dar si foarte multe imprumuturi active ce trebuie monitorizate si actualizate zi de zi.
 Prima problema (proiectul 1) a fost rezolvata prin crearea unui program ce leaga impreuna trei lucruri esentiale unei biblioteci: Cartile, Abonatii si Imprumuturile.
 Aplicatia memoreaza aceste date intr-un mod static si executa operatiuni in mod automat. In momentul in care un abonat incearca sa realizeze un imprumut, programul verifica daca acea carte mai este in stoc si scade numarul de carti disponibile. Programul calculeaza si aplica automat penalizarile in momentul in care un utilizator depaseste numarul de zile permise. O balata de penalizari de peste 25 RON sau un abonament expirat va impiedica abonatul din a mai realiza imprumuturi.
 
 In cel de-al doilea proiect am implementat diferite tipuri de clienti, asa ca am transformat clasa Abonati intr-o clasa parinte, iar din aceasta am creat 4 clase derivate: AbonatCopil, AbonatStudent, AbonatPremium, AbonatNormal.
Fiecare clasa noua a primit functii virtuale pentru a gestiona datele in mod diferit. Cand salvam un student in fisier, programul scrie si numele universitatii unde invata, iar cand salvam un abonat Premium, se adauga o taxa speciala de mentenanta. Tot in Proiectul 2 am inceput sa folosim comanda "dynamic_cast". Aceasta ne ajuta sa cautam prin lista generala de abonati si sa ii identificam doar pe cei care sunt studenti, pentru a le aplica anumite promotii. Pentru ca lucram cu pointeri si alocare dinamica, am folosit tehnica Copy-and-Swap in clasele mari pentru a fi siguri ca obiectele sunt copiate si sterse corect din memorie.

 In cel de-al treilea proiect am avut ca scop eliminarea tuturor defectelor din trecut. In aceasta etapa, am implementat doua sabloane de proiectare celebre, numite Singleton si Factory Method. De asemenea, am trecut la programarea generica folosind clase si functii sablon (Templates).
 
  Implementarea Design Patters:
 -Metoda 1: Singleton Pattern
 
 In versiunile anterioare ale programului, clasa principala "GestiuneAbonati" (ce functioneaza ca o baza de date pentru biblioteca) putea fi creata de mai multe ori in functia main. Acest lucru reprezenta un risc foarte mare. Daca din greseala cream doua obiecte de acest tip, aveam doua baze de date diferite in memorie, iar informatiile despre carti si imprumuturi se puteau amesteca.
 Pentru a repara aceasta problema, am transformat clasa "GestiuneAbonati" intr-un Singleton. Am mutat constructorul clasei in zona privata, ceea ce inseamna ca nimeni din afara clasei nu mai poate scrie comenzi de tipul "new GestiuneAbonati". Pentru a fi siguri ca acest obiect nu poate fi multiplicat sub nicio forma, am sters constructorul de copiere si operatorul=. Singurul mod in care putem folosi clasa acum este prin apelarea functiei statice publice "getInstance()". Aceasta functie creeaza o singura instanta a clasei prima data cand este apelata, iar la urmatoarele apelari returneaza exact acelasi obiect salvat deja in memorie.
 
 -Metoda 2: Factory Pattern
 
 O alta problema mare pe care o aveam in Proiectul 2 se afla in interiorul functiei "incarcaDinFisier". Acolo aveam o structura foarte lunga de tip "if-else". Programul citea o litera din fisier (C pentru Copil, S pentru Student, P pentru Premium sau N pentru Normal) si verifica manual ce obiect trebuie sa creeze cu "new AbonatStudent", "new AbonatCopil" etc. Daca pe viitor doream sa adaugam un nou tip de abonat in aplicatie, eram obligati sa modificam logica de citire.
 Pentru a rezolva aceasta situatie, am creat clasa "AbonatFactory". Aceasta clasa are o singura functie statica numita creeazaAbonat care primeste litera corespunzatoare tipului si toate datele citite din fisier (nume, prenume, cnp, telefon, varsta, durata, universitate, taxa). Fabrica se uita la litera primita si stie exact ce constructor sa apeleze, trimitand numarul corect de parametri cerut de fisierele noastre header (Abonat Student contine universitatea, Abonat Premium contine taxa).
 Dupa ce am implementat Factory, functia "incarcaDinFisier" a fost curatata. Acum procesul de citire este complet separat de procesul de creare a obiectelor. 

  -Metoda 3: Strategy Pattern

 In versiunile anterioare, calculul penalizarilot pentru returnarea cu intarziere a unei carti era hardcodat in functia "finalizeazaImprumut". Daca regulamentul sau logica calculului se schimba trebuia sa schimbam logica clasei Imprumuturi.
 Pentru a repara aceasta problema am implementat Strategy Pattern. Am extras algoritmii de calcul al penalizarilor si i-am mutat in clase separate. Am creat o interfata abstracta "StrategiePenalizare" cu metoda virtuala pura "calculeazaPenalizare", apoi am derivat din ea doua strategii: "PenalizareStandard" (aplica o taxa standard pentru abonatii cu un scor scazut) si "PenalizareRedusa (aplica o reducere de 25% pentru abonatii cu un scor ridicat).
 
 Programarea Generica: Clase si Functii Sablon (Templates):
 -Clasa Sablon "Catalog<T>"
 Inainte de modernizare, clasa "GestiuneAbonati" tinea datele in doi vectori separati din biblioteca standard: "std::vector<Carti*>" si "std::vector<Imprumuturi*>". Pentru fiecare dintre acesti doi vectori trebuia sa scriem functii aproape identice de adaugare si bucle repetitive in destructor pentru a elibera manual memoria ocupata de pointeri. Pentru a sterge acest cod duplicat, am proiectat clasa sablon "Catalog<T>". Aceasta clasa functioneaza ca un container universal care contine un vector de pointeri de tip generic T. Cel mai mare avantaj este ca acest Catalog are un destructor inteligent. In momentul in care aplicatia se inchide si obiectul catalog este distrus, destructorul sau parcurge automat toata lista interna de pointeri, aplica comanda delete pe fiecare element valid si elibereaza memoria RAM instantaneu.
 In fisierul "GestiuneAbonati.h" am inlocuit vectorii vechi si am creat doua cataloage folosind aceasta matrice: "Catalog<Carti> catalogCarti;"; "Catalog<Imprumuturi> catalogImprumuturi;". 
 -Functia Sablon "cautaElement":
 Pe langa clasa sablon, am creat si o functie sablon in interiorul catalogului, folosita pentru a cauta rapid elemente in liste: "template <typename Criteriu, typename Comparator>
T* cautaElement(Criteriu valoare, Comparator comp);".
 Aceasta functie primeste o valoare pe care o cautam si o functie anonima speciala care decide daca elementul este cel corect. Am folosit aceasta functie generica in doua situatii diferite din fisierul "GestiuneAbonati.cpp":
-In functia "gasesteCarte": Cautam in "catalogCarti" folosind ID-ul cartii, iar functia compara sirurile de caractere cu ajutorul functiei clasice "std::strcmp".
-In functia "gasesteImprumutDupaCarte": Cautam in "catalogImprumuturi" folosind tot un ID de carte, dar de data aceasta functia intra inauntrul obiectului de tip imprumut si verifica daca ID-ul cartii luate coincide cu cel cautat. Datorita acestei functii sablon, am sters toate buclele repetitive de tip for pe care inainte le scriam de mana la fiecare operatiune de cautare.

 Bibliografie:
 1. https://refactoring.guru/design-patterns/abstract-factory/cpp/example#example-0 - recomandat pe github, explica Abstract Factory si are exemple de cod complete.
 2. https://refactoring.guru/design-patterns/singleton/cpp/example#lang-features - recomandat pe github, explica Singleton si are exemple de cod complete.
 3. https://www.youtube.com/watch?v=usmdZniV_Yw - Factory Design Pattern Tutorial.
 4. https://www.youtube.com/watch?v=p3OQDb4nWfg&t=141s - Templates in Modern C++.
 5. https://en.cppreference.com/cpp/language/class_template - Templates explicate + exemple de cod.
