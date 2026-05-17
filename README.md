# Sistem Gestiune Biblioteca

Proiect de gestiune a activitatii unei biblioteci. Sistemul permite gestionarea stocurilor de carti,
a diferitelor tipuri de abonati (Copil, Student, Premium, Pensionar) si procesarea imprumuturilor cu calcul 
automat al penalizarilor.

# Compilare:
g++ *.cpp -o biblioteca

# Mod de functionare:

La rulare trebuie sa introducem CNP-ul persoanei in nuele careia vrem sa imprumutam sau sa returnam o carte. Avem un switch cu 6 optiuni:
1) .[a] Inapoi -> navigheaza la stanga prin biblioteca de carti.
2) .[b] Inainte -> navigheaza la dreapta prin biblioteca de carti
3) .[i] Imprumuta -> verifica daca abonatul este eligibil sa imprumute (nu are penalizari mai mari de 25 RON si are un abonament activ) si salveaza imprumutul in "imprumuturi.txt".
4) .[r] Returneaza -> deschide un nou meniu ce arata toate cartile imprumutate de abonat (numerotate de la 1). Utilizatorul trebuie sa introduca numarul corespunzator cartii pe care acesta vrea sa o returneaza. La returnare se va compara perioada de zile a imprumutului (default la 14) cu zilele scurse (folosing functia "Trece o zi"). Daca termenul a fost depasit se aplica o taxa de penalizare.
5) .[t] Trece o zi -> Scade o zi din abonamentul tuturor abonatilor si adauga o zi timpului scurs fiecarui imprumut.
6) .[x] Iesire -> Inchide programul.

# Gestiunea diferitelor tipuri de Abonati:
Abonati: Clasa de baza care gestioneaza datele personale (nume, CNP, telefon) si balanta penalizarilor

Abonat Copil: Penalizari mai mari la scorul de incredere in caz de intarziere.

Abonat Student: Beneficiaza de promotii (prelungiri de abonament) si are reduceri la penalizari.

Abonat Premium: Plateste o taxa de mentenanta, dar are penalizari financiare reduse si bonusuri de scor mai mari.

Abonat Pensionar: Reguli standard de imprumut adaptate categoriei de varsta.

# Gestiunea Resurselor:
- Carti: Gestioneaza titlul, autorul si stocul disponibil.
- Imprumuturi: Realizeaza legatura intre un Abonat si o Carte. Calculeaza penalizarile daca perioada
de returnare este depasita.
- GestiuneAbonati: Simuleaza trecerea timpului pentru toti abonatii (poate fi folosit la procesarea finalului de zi), aplica bonusuri.

# Gestiune Carti si Imprumuturi:
Catalog de Carti: Navigare interactiva prin lista de carti disponibile, cu detalii despre autor, an si stoc.

Sistem de Imprumut: Verifica automat daca abonatul are dreptul sa imprumute (verificare abonament expirat sau penalizari financiare neachitate).

Returnare cu Feedback: La returnare, sistemul calculeaza automat zilele scurse si aplica penalizari financiare daca este cazul.

# Sistem Scor de Incredere:
Fiecare abonat are un Scor de Incredere (de la 0 la 100, initial 60):

+Puncte: Pentru returnarea cartilor la timp (creste reputatia).

-Puncte: Pentru intarzieri (scade reputatia si poate bloca imprumuturile viitoare).

# Concepte OOP folosite:
Abstractizare: Clasa de baza Abonati este abstracta, definind interfata pentru toate tipurile de utilizatori prin metoda pur virtuala clone().

Mostenire: Toate tipurile specifice de abonati mostenesc clasa Abonati.

Polimorfism: Folosirea dynamic cast pentru a aplica logica specifica fiecarui tip de abonat si utilizarea functiilor virtuale (do_print) pentru afisare.

Gestiune Resurse: Implementare Constructor de copiere, Operator= si Destructor pentru a gestiona memoria alocata dinamic pentru char*.

Salvarea: Datele sunt salvate si incarcate automat din fisiere text (abonati.txt, carti.txt, imprumuturi.txt).
