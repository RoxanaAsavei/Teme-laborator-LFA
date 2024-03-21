#include <bits/stdc++.h>
#define VALMAX 10000
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

int N; // nr de stari
map<int, int> stari; // val_nodului - al catalea a fost citit (ii asociez fiecarui nod un indice) ex: 149292 -> 0
vector<int> indice_stare; // fix continutul din stari, doar ca cheie devine valoare si valoarea devine cheie ex: 0 -> 149292
int M; // nr de tranzitii
vector<pair<int, char> > graf[VALMAX + 5]; // modelarea grafului
// pt fiecare nod o sa tin minte un vector cu elemente de forma (vecin, litera cu care ajung in vecin)

int init_state; // starea initiala

int ct_finale; // nr de stari finale
bitset<VALMAX+5> finale; // marcam cu 1 starile care sunt finale 

int nr_cuvinte;
vector<string> cuvinte;

queue<pair<int, int> > coada; // (indice nod, indicele ultimei litere din cuvant cu care am ajuns in starea asta)

void Citire() {
    int x, y;
    char litera;
    fin >> N;
    for(int i = 0; i < N; ++i) {
            fin  >> x;
            stari[x] = i;  // nodului x ii asociez indicele i
            indice_stare.push_back(x);
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
        finale[stari[x]] = 1;
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

void Afisare_drum(int nod, int indice_litera, vector<pair<int, int> > ascendenti[]) {

    if(indice_litera != -1) { // nu e starea initiala
        // ma uit sa vad de unde as fi putut ajunge aici
        int i;
        for( i = 0; i < ascendenti[nod].size(); ++i) {
            if(ascendenti[nod][i].second == indice_litera) {
                break; 
            }
        }
        Afisare_drum(ascendenti[nod][i].first, indice_litera - 1, ascendenti);
    }
    fout << indice_stare[nod] << " ";
}

void Acceptat(string cuvant) { // afiseaza DA daca cuvantul e acceptat + drumul aferent, altfel afiseaza NU
    vector<pair<int, int> > ascendenti[VALMAX + 5];   
    /**
     * Folosim ascendenti doar pentru cerinta suplimentara (afisarea drumului pentru cuvintele acceptate)
     * Facem un BFS sa vedem daca cuvantul e acceptat sau nu si in timp ce facem asta, pentru fiecare nod x in 
     * care ajungem vom tine in 
     * ascendenti[x] perechi de forma (nod din care am ajuns in x, pozitia literei din cuvant care m-a adus in x)
    */
    int p =  0, n = cuvant.length(); // p -> indicele literei curente
    Initializare_coada();
    coada.push({stari[init_state], -1}); // pun indicele starii initiale
    while(p < n) {
        bool ok = 0; // ca sa vad daca pot continua sau nu
        while(!coada.empty() && coada.front().second == p - 1) {// stare pentru care s-a putut obtine prefixul 0..p-1
            int nod_curent = coada.front().first; // incerc sa vad daca pot continua cu litera de la poz p
            coada.pop();
            // ma uit sa vad daca pot continua cu ce imi trebuie
            for(int i = 0; i < graf[nod_curent].size(); ++i)
                if(graf[nod_curent][i].second == cuvant[p]) {
                    coada.push({graf[nod_curent][i].first, p});
                    ascendenti[graf[nod_curent][i].first].push_back({nod_curent, p});
                    ok = 1;
            }
        }

        if(ok == 1) p++; // merg mai departe, life's good
        else { // lipsa de tranzitie
            fout << "NU\n";
            return;
        } 
    }

    while(!coada.empty()) {
        int posibil = coada.front().first;
        coada.pop();
        if(finale[posibil] == 1) {
            fout << "DA\n";
            Afisare_drum(posibil, n - 1, ascendenti);
            fout << "\n";
            return;
        }
    }

    fout << "NU\n";

}

void Rezolvare() {
    for(int i = 0; i < cuvinte.size(); ++i) {
       Acceptat(cuvinte[i]);
    }

}

int main() {
   Citire();
   Rezolvare();
    return 0;
}
