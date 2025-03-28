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

    int* kmpNext = new int[m];
    kmpNext[0] = -1;
    if (m > 1) kmpNext[1] = 0;

    for (int i = 2; i < m; i++) {
        int j = kmpNext[i - 1];
        while (j > 0 && vzorec[j] != vzorec[i - 1]) {
            j = kmpNext[j];
        }
        if (vzorec[j] == vzorec[i - 1]) {
            j++;
        }
        kmpNext[i] = j;
    }

    izpis_KMPnext(kmpNext, m);

    int i = 0, j = 0;
    while (j <= n - m) {
        i = 0;
        while (i < m && vzorec[i] == text[j + i]) {
            i++;
        }

        if (i == m) {
            out << j << ' ';
            j += m;
        }
        else {
            if (i == 0) {
                j++;
            }
            else {
                j += (i - kmpNext[i]);
            }
        }
    }

    delete[] kmpNext;
}

void Sunday(const string& text, const string& vzorec) {
    int m = vzorec.length();
    int n = text.length();

    if (m == 0 || n == 0) {
        return;
    }

    int BCH[256];

    for (int i = 0; i < 256; i++) {
        BCH[i] = m + 1;
    }

    for (int i = 0; i < m; i++) {
        BCH[(unsigned char)vzorec[i]] = m - i;
    }

    int j = 0;
    while (j <= n - m) {
        int i = 0;
        while (i < m && vzorec[i] == text[j + i]) {
            i++;
        }

        if (i == m) {
            out << j << ' ';
            j += m;
        }
        else {
            if (j + m < n) {
                j += BCH[(unsigned char)text[j + m]];
            }
            else {
                break;
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