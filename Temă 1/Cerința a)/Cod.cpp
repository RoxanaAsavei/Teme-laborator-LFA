#include <bits/stdc++.h>
#define VALMAX 10000
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

int N; // nr de stari
map<int, int> stari; // val_nodului - al catalea a fost citit (ii asociez fiecarui  nod un indice)
int M; // nr de tranzitii
vector<pair<int, char> > graf[VALMAX + 5]; // modelarea grafului
// pt fiecare nod o sa tin minte un vector cu elemente de forma (vecin, litera cu care ajung in vecin)

int init_state; // starea initiala

int ct_finale; // nr de stari finale
vector<int> stari_finale;

int nr_cuvinte;
vector<string> cuvinte;

void Citire() {
    int x, y;
    char litera;
    fin >> N;
    for(int i = 1; i <= N; ++i) {
            fin  >> x;
            stari[x] = i;  // nodului x ii asociez indicele i
        }
    fin >> M;
    for(int i = 1; i <= M; ++i) {
        fin >> x >> y >> litera;
        graf[stari[x]].push_back({stari[y], litera});

    }

    fin >> init_state;
    fin >> ct_finale;
    for(int i = 1; i <= ct_finale; ++i) {
        fin >> x;
        stari_finale.push_back(stari[x]);
    }

    fin >> nr_cuvinte;
    string cuvant;
    for(int i = 1; i <= nr_cuvinte; ++i) {
        fin >> cuvant;
        cuvinte.push_back(cuvant);
    }

}

int DFS(string cuvant, int stare_curenta, int poz_curenta, int lungime) {
        // caut sa vad daca pot ajunge cu litera care imi trebuie mie
        for(int i = 0; i < graf[stare_curenta].size(); ++i) {
            if(graf[stare_curenta][i].second == cuvant[poz_curenta]) {
                if(poz_curenta == lungime - 1) { // am putut obtine tot cuvantul, pana la ultima litera
                    return graf[stare_curenta][i].first;
                }
                else { // pot continua cu urmatoarea litera
                    return DFS(cuvant,  graf[stare_curenta][i].first, poz_curenta + 1, lungime);
                }
            }
        }

        return -1; // lipsa de tranzitie

}


// ret 1 daca cuvantul este acceptat de DFA, 0 in caz contrar
bool Acceptat(string cuvant) {
    int starea_de_oprire = DFS(cuvant, stari[init_state], 0, cuvant.length());
    if(starea_de_oprire == -1) {
        return 0;
    }

    for(int i = 0; i < stari_finale.size(); ++i) {
        if(starea_de_oprire == stari_finale[i]) {
            return 1;
        }
    }

    return 0;
}

void Rezolvare() {
    for(int i = 0; i < cuvinte.size(); ++i) {
        bool ans = Acceptat(cuvinte[i]);
        if(ans == 1) {
            fout << "DA" << endl;
        }
        else {
            fout << "NU" << endl;
        }
    }

}

int main() {
   Citire();
   Rezolvare();
    return 0;
}
