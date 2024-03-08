#include <bits/stdc++.h>
#define VALMAX 10000
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

int N;
map<int, int> stari; // val_nodului - al catalea a fost citit
int M;
vector<pair<int, char> > graf[VALMAX + 5];
vector<string> cuvinte;
int nr_cuvinte;
int init_state;
vector<int> stari_finale;
int ct_finale;
queue<pair<int, int> > coada; // (ind_nod, ind_litera_cu_care_am_ajuns_aici)

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

void Initializare_coada() {
    while(!coada.empty()) {
        coada.pop();
    }
}

// ret 1 daca cuvantul apartine, 0 in caz contrar

bool Apartine(string cuvant) {
    int p =  0, n = cuvant.length();
    Initializare_coada();
    coada.push({stari[init_state], -1}); // pun indicele starii initiale
    while(p < n) {
        bool ok = 0; // ca sa vad daca pot continua sau n
        while(!coada.empty() && coada.front().second == p - 1) {// stare pentru care s-a putut obtine prefixul 0..p-1
            int nod_curent = coada.front().first; // inerc sa vad daca pot continua cu litera de la poz p
            coada.pop();
            // ma uit sa vad daca pot continua cu ce imi trebuie
            for(int i = 0; i < graf[nod_curent].size(); ++i)
                if(graf[nod_curent][i].second == cuvant[p]) {
                    coada.push({graf[nod_curent][i].first, p});
                    ok = 1;
            }
        }

        if(ok == 1) p++;
        else return 0;
    }

    while(!coada.empty()) {
        int posibil = coada.front().first;
        coada.pop();
        for(int i = 0; i < stari_finale.size(); ++i) {
            if(posibil == stari_finale[i])
                return 1;
        }
    }

    return 0;

}

void Rezolvare() {
    for(int i = 0; i < cuvinte.size(); ++i) {
        bool ans = Apartine(cuvinte[i]);
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
