#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <iterator>

using namespace std;

class Movie {
public:
    string index;
    string original_language;
    string original_title;
    string release_date;
    string revenue;
    string runtime;

    Movie() {

    }

    Movie(string l, string t, string date, string rev, string time) {
        original_language = l;
        original_title = index = t;
        release_date = date;
        revenue = rev;
        runtime = time;
    }
};

class AddressFunction{
protected:
    vector<Movie> movies;
public:
    virtual int getAddress(string key, int address, int attempt, int size) const = 0;
};

class LinearHashing : AddressFunction{
public:
    LinearHashing() {
    }
    LinearHashing(vector<Movie> m) {
        movies = m;
    }
    int getAddress(string key, int address, int attempt, int sizeM) const {
        int returnIndex,s = 0;
        returnIndex = (address + s*attempt)%sizeM;
        while(movies[returnIndex].original_title.compare("0")!=0) {
            s++;
            returnIndex = (address + s*attempt)%sizeM;
        }
        return returnIndex;
    }

};

class HashTable{
public:
    int maxSize;

    int cntAll=0,cntYes=0,cntFail=0;
    vector<Movie> movies;
    int attempt = 1;
    LinearHashing lh;

    HashTable() {}

    HashTable(string tableName) {
        maxSize = 5000;
        Movie memp("0","0","0","0","0");
        movies = vector<Movie>();
        lh = LinearHashing(movies);
        ifstream classFile(tableName.c_str());
        bool firstLine = true;

        if (!classFile.is_open())
            cout << "Nije ucitan fajl." << endl;

        string lang;
        string title;
        string date;
        string rev;
        string time;

        while(classFile.good()) {

            getline(classFile,lang,',');
            getline(classFile,title,',');
            getline(classFile,date,',');
            getline(classFile,rev,',');
            getline(classFile,time,'\n');
            if (firstLine) {
                firstLine = false;
            }
            else {
                Movie m(lang,title,date,rev,time);
                lh = LinearHashing(movies);
                int num = rand()%maxSize;
                int a = getAddress(title,num,attempt,maxSize);
                movies.insert(movies.begin()+a, m);

            }
        }
    }

    string findKey(string k) {
        int ii=-1;
        cntAll++;
        for(int i=0; i<maxSize; i++)
            if (movies[i].index.compare(k)==0) {
                ii=i;
                break;
            }
        if (ii==-1) {
            cntFail++;
            return "0";
        }
        Movie m = movies[ii];
        cntYes++;
        return m.original_language + " " + m.original_title + " " + m.release_date +
                " " + m.revenue + " " + m.runtime + "\n";
    }

    double avgAccessSuccess() {
        if (cntYes == 0)
            return 0;
        return (double)cntAll/cntYes;
    }

    double avgAccessUnsuccess() {
        if (cntFail == 0)
            return 0;
        return (double)cntAll/cntFail;
    }

    bool insertKey(string k, string info) {
        if (findKey(k).compare("0")!=0)
            return false;

        char str[500], *token;
        char lang[2],name[100],date[10],rev[50],time[4];

        strcpy(str,info.c_str());
        token = strtok(str,",");
        strcpy(lang,token);
        string l(lang);
        token = strtok(NULL,",");
        strcpy(name,token);
        string n(name);
        token = strtok(NULL,",");
        strcpy(date,token);
        string d(date);
        token = strtok(NULL,",");
        strcpy(rev,token);
        string r(rev);
        token = strtok(NULL,",");
        strcpy(time,token);
        string t(time);

        Movie m(l,k,d,r,t);
        int num = rand()%maxSize;
        int a = getAddress(n,num,attempt,maxSize);
        movies.insert(movies.begin()+a, m);
        return true;
    }

    bool deleteKey(string k){
        cntAll++;
        if (findKey(k).compare("0")==0) {
            cntFail++;
            return false;
        }
        cntYes++;
        for(int i=0; i<maxSize; i++) {
            if (movies[i].index.compare(k)==0) {
                movies.erase(movies.begin()+i);
                break;
            }
        }
        return true;
    }

    int tableSize(){
        return maxSize;
    }

    int keyCount() {
        return movies.size();
    }

    void resetStatistics() {
        cntAll = 0;
        cntFail = 0;
        cntYes = 0;
    }

    void clearTable(){
        movies.clear();
    }

    int avgAccessUnsuccessEst(){
        double avg = avgAccessUnsuccess();
        double fr = fillRatio();
        return avg/(avg-fr);
    }

    double fillRatio(){
        int sizeTable = movies.size();
        int countTable = 0;
        for(int i = 0; i<sizeTable; i++){
            if(movies[i].original_title.find("?") != string::npos) {
                countTable++;
                continue;
            }
            if(movies[i].release_date.empty()){
                countTable++;
                continue;
            }
            if(movies[i].revenue.compare("0") == 0){
                countTable++;
                continue;
            }
            if(movies[i].runtime.compare("0") == 0){
                countTable++;
                continue;
            }
        }
        return 1-(double)countTable / sizeTable;
    }

    int getAddress(string key, int address, int attempt, int size) {
        return 0;
    }
    void printEl() {
        for(int i=0; i<movies.size(); i++) {
            cout << movies[i].original_title << endl;
        }
    }
};

ostream & operator << (ostream &out, const vector<Movie> &mm) {
        for(int i=0; i<mm.size(); i++) {
            out << mm[i].original_language + " ";
            out << mm[i].original_title + " ";
            out << mm[i].release_date + " ";
            out << mm[i].revenue + " ";
            out << mm[i].runtime << endl;
        }
        return out;
    }

    void meniChooser(){
        cout << "--------------------MENI-----------------------" << endl;
        cout << "(0)izlaz" << endl;
        cout << "(1)pronadji kljuc" << endl;
        cout << "(2)unesi kljuc" << endl;
        cout << "(3)obrisi kljuc" << endl;
        cout << "(4)prosecan broj pristupa tabeli prilikom uspesnog trazenja kljuca" << endl;
        cout << "(5)prosecan broj pristupa tabeli prilikom neuspesnog trazenja kljuca" << endl;
        cout << "(6)prosecan broj pristupa tabeli prilikom neuspenog trazenja kljuca" << endl;
        cout << "dobijen na osnovu prozene bazirane na popunjenosti tabele" << endl;
        cout << "(7)postavljanje svih podataka portebnih za prosecan pristup tabeli na pocetnu vrednost" << endl;
        cout << "(8)brisanje svih kljuceva" << endl;
        cout << "(9)broj umetnutih kljuceva" << endl;
        cout << "(10)velicina tabele" << endl;
        cout << "(11)ispis sadrzaja tabele" << endl;
        cout << "(12)stepen popunjenosti tabele" << endl;
    }

int main()
{
    int st;
    string filename;
    cout << "Unesi naziv fajla: ";
    cin >> filename;
    HashTable h(filename);
    meniChooser();
    srand((unsigned)time(0));
    int meniOption;
    cin >> meniOption;
    do{
        if(meniOption == 0){
            return 0;
        }
        if(meniOption == 1){
            cout << "Unesite kljuc:" << endl;
            char key[50];
            scanf("%s", key);
            string key1(key);
            cout << h.findKey(key1) << endl;
        }
        if(meniOption == 2){
            cout << "Unesite kljuc:" << endl;
            char key[50];
            scanf("%s", key);
            string key1(key);
            cout << "Unesite informacije o filmu:" << endl;
            char info[500];
            scanf("%s", info);
            string info1(info);
            h.insertKey(key1, info1);
        }
        if(meniOption == 3){
            cout << "Unesite kljuc koji zelite da obrisete:" << endl;
            char key[50];
            scanf("%s", key);
            string key1(key);
            h.deleteKey(key1);
        }
        if(meniOption == 4){
            cout << h.avgAccessSuccess() << endl;
        }
        if(meniOption == 5){
            cout << h.avgAccessUnsuccess() << endl;
        }
        if(meniOption == 6){
            cout << h.avgAccessUnsuccessEst() << endl;
        }
        if(meniOption == 7){
            h.resetStatistics();
        }
        if(meniOption == 8){
            h.clearTable();
        }
        if(meniOption == 9){
            cout << h.keyCount() << endl;
        }
        if(meniOption == 10){
            cout << h.tableSize() << endl;
        }
        if(meniOption == 11){
            cout << h.movies << endl;
        }
        if(meniOption == 12){
            cout << h.fillRatio() << endl;
        }
        meniChooser();
        scanf("%d", &meniOption);
    }while(meniOption > 0 && meniOption < 13);

    return 0;
}
