#include <bits/stdc++.h>
#define VALMAX 100000
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

int nr_cuvinte;
vector<string> cuvinte;

vector<int> lambda_inchidere[VALMAX + 5];
bitset<VALMAX + 5> vizitat; // pt a sti ce stari am pus deja intr-o inchidere

queue<int> C; // in coada punem starile curente, cele de la care trebuie sa avansam

set<int> urm_stari;
bitset<VALMAX + 5> finale;

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
        fin >> x >> litera >> y;
        graf[stari[x]].push_back({stari[y], litera});

    }

    fin >> init_state;
    fin >> ct_finale;
    for(int i = 1; i <= ct_finale; ++i) {
        fin >> x;
        finale[stari[x]] = 1;
    }

    fin >> nr_cuvinte;
    string cuvant;
    for(int i = 1; i <= nr_cuvinte; ++i) {
        fin >> cuvant;
        cuvinte.push_back(cuvant);
    }

}

void DFS(int nod_start, int nod_curent) {
    vizitat[nod_curent] = 1;
    lambda_inchidere[nod_start].push_back(nod_curent);
    for(int i = 0; i < graf[nod_curent].size(); ++i)
        if(graf[nod_curent][i].second == '.' && !vizitat[graf[nod_curent][i].first])
            DFS(nod_start, graf[nod_curent][i].first);
}



void Lambda_inchidere(int nod) {
    vizitat.reset(); // momentan nu e niciun nod vizitat
    DFS(nod, nod); // incep sa parcurg din nodul respectiv, mergand doar pe muchii cu lambda
}

void Construire_lamda_inchideri() {
    for(int i = 1; i <= N; ++i)
        Lambda_inchidere(i);
}


void Afisare_lambda_inchideri() {
    for(int i = 1; i <= N; ++i) {
        cout << "Inchiderea lui " << i << " este: ";
        for(int j = 0; j < lambda_inchidere[i].size(); ++j) {
            cout << lambda_inchidere[i][j] << " ";
        }
        cout << endl;
    }
}

bool Acceptat(string cuvant) {
// intre oricare 2 litere ii dau voie sa puna un nr de lambda-uri >= 0
// ii dau voie sa puna lambda-uri si inainte de prima litera, dar si dupa ultima litera
    int length  = cuvant.length();
    C.push(stari[init_state]);
    for(int i = 0; i < 2 * length + 1; ++i) {
        // cand i e par, pun lamda-uri, cand e impar, trebuie sa pun litera cuvant[i / 2]
        if(i % 2 == 0) {
            while(!C.empty()) {
                int nod_curent = C.front();
                C.pop();
                // pun toate starile din lambda inchiderea nodului curent
                for(int j = 0; j < lambda_inchidere[nod_curent].size(); ++j) {
                    urm_stari.insert(lambda_inchidere[nod_curent][j]);
                }
            }
        }

        else {
            // trebuie sa ma duc cu litera cuvant[i / 2]
            while(!C.empty()) {
                int nod_curent = C.front();
                C.pop();
                for(int j = 0; j < graf[nod_curent].size(); ++j) {
                    if(graf[nod_curent][j].second == cuvant[i / 2]) {
                        urm_stari.insert(graf[nod_curent][j].first);
                    }
                }
            }
        }

        if(urm_stari.empty()) {  // lipsa de tranzitivitate
            return 0; 
        }

        // rasturnam continutul setului in coada
        for(auto stare : urm_stari) {
            C.push(stare);
        }

        urm_stari.clear();
    }

    // vad daca in coada am ceva stari finale
    while(!C.empty()) {
        int nod_curent = C.front();
        C.pop();
        if(finale[nod_curent]) {
            return 1;
        }
    }
    return 0;
}

void Initializare_coada() {
    while(!C.empty()) {
        C.pop();
    }
}

void Rezolvare() {
    for(int i = 0; i < nr_cuvinte; ++i) {
        Initializare_coada();
        bool acceptat = Acceptat(cuvinte[i]);
        if(acceptat) {
            fout << "DA" << endl;
        }
        else {
            fout << "NU" << endl;
        }
    }
}

int main() {
    Citire();
    Construire_lamda_inchideri();
    Afisare_lambda_inchideri(); // calculeaza bine inchiderile
    Rezolvare();
    return 0;
}
