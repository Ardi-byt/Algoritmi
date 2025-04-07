#include <string>
#include <sstream>
#include <fstream>
using namespace std;

ofstream out;

string inputText(const string& pot) {
	ifstream input(pot);
	stringstream sstream;

	if (!input.is_open()) {
		return string();
	}

	sstream << input.rdbuf();
	return sstream.str();
}

void izpis_KMPnext(const int* polje, int len) {
	for (int i = 0; i < len; ++i) {
		out << polje[i] << ' ';
	}
	out << endl;
}

void KMP(const string& text, const string& vzorec) {
    int m = vzorec.length();
    int n = text.length();

    if (m == 0 || n == 0) {
        return;
    }
    //Priprava tabele kmpNext
    int* kmpNext = new int[m];
    kmpNext[0] = -1;    //prvi kmpNext = -1
    if (m > 1) kmpNext[1] = 0;
    //Izracun tabele kmpNext za 2 indeksa naprej
    for (int i = 2; i < m; i++) {
        int j = kmpNext[i - 1];
        //Iskanje ujemanja med prefiksom in sufiksom
        while (j > 0 && vzorec[j] != vzorec[i - 1]) {
            j = kmpNext[j]; //Ce ni ujemanja poskusi s krajsim prefiksom
        }
        if (vzorec[j] == vzorec[i - 1]) {
            j++; //Ce je ujemanje povecaj dolzino prefiksa
        }
        kmpNext[i] = j; //Shrani dolzino najdaljsega ujemajocega prefiksa
    }

    izpis_KMPnext(kmpNext, m);

    int i = 0, j = 0;
    //Glavna zanka za iskanje vzorca v besedilu
    //j predstavlja trenutno pozicijo v besedilu ki jo preverjamo
    while (j <= n - m) {
        i = 0;
        //Preverjamo ujemanje znakov vzorca z besedilom od pozicije j
        while (i < m && vzorec[i] == text[j + i]) {
            i++; //i se veca dokler se znaki ujemajo
        }

        //Popolno ujemanje nasli smo vzorec na poziciji j
        if (i == m) {
            out << j << ' '; //Izpisemo indeks pojavitve v out.txt
            j += m; //Preskocimo celoten vzorec in ne iscemo vec pojavitev
        }
        else {
            if (i == 0) { //Neujemanje
                j++; //Ce ni ujemanje v prvem znaku se premaknemo naprej
            }
            else {//Ce je bilo delno ujemanje uporabimo kmpNext za preskok
                j += (i - kmpNext[i]); //i-kmpNext[i] nam pove koliko mest lahko varno preskocimo
            }
        }
    }

    delete[] kmpNext; //Sprosti pomnilnik
}

void Sunday(const string& text, const string& vzorec) {
    int m = vzorec.length();
    int n = text.length();

    if (m == 0 || n == 0) {
        return;
    }
    //Priprava tabele BCH
    int BCH[256]; //Tabela za vse mozne ASCII znake
    
    //Inicializiramo tabelo za vse znake ki niso v vzorcu nastavimo m+1
    for (int i = 0; i < 256; i++) {
        BCH[i] = m + 1;
    }

    //Za znake v vzorcu nastavimo vrednosti po formuli BCH[vzorec[i]] = m-1
    for (int i = 0; i < m; i++) {
        BCH[(unsigned char)vzorec[i]] = m - i;
    }

    //Iskanje vzorca v besedilu
    int j = 0; //Zacetna pozicija
    while (j <= n - m) { //Dokler lahko vzorec se stavimo v besedilo
        int i = 0; //Stevec ujemajocih znakov
        //Preverjanje ujemanja znakov vzorca z besedilom
        while (i < m && vzorec[i] == text[j + i]) {
            i++; //Povecujemo dokler se znaki ujemajo
        }

        if (i == m) { //Popolno ujemanje nasli smo vzorec na j
            out << j << ' '; //Izpisemo indeks kje se pojavi
            j += m; //Preskocimo celoten vzorec torej ne iscemo ponovitev
        }
        else {
            if (j + m < n) { //Neujemanje
                //Ce nismo na koncu besedila uporabimo BCH tabelo za preskok
                j += BCH[(unsigned char)text[j + m]];
                //Pogledamo znak ki je takoj za trenutnim oknom in uporabimo njegovo vrednost iz BCH
            }
            else {
                break; //Ce je konec besedila zakljucimo
            }
        }
    }
}


int main(int argc, const char* const argv[]) {
	if (argc != 4) {
		return -1;
	}

	string text = inputText(argv[3]);
	string vzorec = argv[2];
	out.open("out.txt");

	if (!out) {
		return -2;
	}

	if (argv[1][0] == '0') {
		KMP(text, vzorec);
	}
	else {
        Sunday(text, vzorec);
	}


	return 0;
}