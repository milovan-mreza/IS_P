#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Racun {
protected:
    string nazivBanke;
    string tipKartice;
    string brojRacuna;
    int godinaVazenja;
public:
    Racun(string nBanke, string tKartice, string bRacuna, int gVazenja = 2023)
        : nazivBanke(nBanke), tipKartice(tKartice), brojRacuna(bRacuna), godinaVazenja(gVazenja) {}

    virtual bool sifraRacunaValidna() const = 0;

    friend ostream& operator<<(ostream& os, const Racun& r) {
        os << "Banka: " << r.nazivBanke << ", Tip kartice: " << r.tipKartice
            << ", Broj racuna: " << r.brojRacuna << ", Godina vazenja: " << r.godinaVazenja;
        return os;
    }
    int getGodinaVazenja() const {
        return godinaVazenja;
    }
    string getBrojRacuna() const {
        return brojRacuna;
    }
};

class DevizniRacun : public Racun {
public:
    DevizniRacun(string nBanke, string tKartice, string bRacuna, int gVazenja = 2023)
        : Racun(nBanke, tKartice, bRacuna, gVazenja) {}

    bool sifraRacunaValidna() const override {
        return all_of(brojRacuna.begin(), brojRacuna.end(), [](char c) {
            return isdigit(c) || islower(c);
            });
    }
};

class DinarskiRacun : public Racun {
public:
    DinarskiRacun(string nBanke, string tKartice, string bRacuna, int gVazenja = 2023)
        : Racun(nBanke, tKartice, bRacuna, gVazenja) {}

    bool sifraRacunaValidna() const override {
        return none_of(brojRacuna.begin(), brojRacuna.end(), [](char c) {
            return islower(c);
            }) && all_of(brojRacuna.begin(), brojRacuna.end(), [](char c) {
                return isdigit(c) || isupper(c);
                });
    }
};

int main() {
    ifstream inputFajl("input.txt");
    ofstream outputFajl("output.txt");

    if ((!inputFajl.is_open()) || (!outputFajl.is_open())) {
        cout << "Greska pri otvaranju fajla" << endl;
        return 1;
    }

    vector<unique_ptr<Racun>> racuni;
    string oznaka, nazivBanke, tipKartice, brojRacuna;
    int godinaVazenja;

    while (inputFajl >> oznaka >> nazivBanke >> tipKartice >> brojRacuna >> godinaVazenja) {
        if (godinaVazenja == 0) {
            godinaVazenja = 2023;
        }
        if (oznaka == "I") {
            racuni.push_back(make_unique<DevizniRacun>(nazivBanke, tipKartice, brojRacuna, godinaVazenja));
        } else if (oznaka == "D") {
            racuni.push_back(make_unique<DinarskiRacun>(nazivBanke, tipKartice, brojRacuna, godinaVazenja));
        }
    }

    stable_sort(racuni.begin(), racuni.end(), [](const unique_ptr<Racun>& a, const unique_ptr<Racun>& b) {
        return a->getGodinaVazenja() > b->getGodinaVazenja();
        });

    auto minGodinaRacun = min_element(racuni.begin(), racuni.end(), [](const unique_ptr<Racun>& a, const unique_ptr<Racun>& b) {
        return a->getGodinaVazenja() < b->getGodinaVazenja();
        });

    if (minGodinaRacun != racuni.end()) {
        if (!(*minGodinaRacun)->sifraRacunaValidna()) {
            outputFajl << "Racun sa najmanjom godinom vazenja nema validan broj: " << (*minGodinaRacun)->getBrojRacuna() << "\n";
        }
        else {
            outputFajl << **minGodinaRacun << "\n";
        }
    }

    inputFajl.close();
    outputFajl.close();

    return 0;
}
