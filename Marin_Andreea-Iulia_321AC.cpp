#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>
#include <sstream>
#include <cstdlib>
using namespace std;
ifstream inputFile;
std::ofstream outputFile;
int nr; //numar total de persoane
int nrAct; //numar total de actori+figuranti
class allCast{
    public:
        //creare fisier in limba engleza
        void printCast(){
            int i;
            inputFile.open("cast.csv");
            outputFile.open("castEng.csv");
                string line="";
                while(getline(inputFile,line)){
                    string name;
                    string role;

                    stringstream inputString(line);
                    getline(inputString, name, ',');
                    getline(inputString, role);
                    if(role=="actor")nrAct++;
                    line="";

                    outputFile<<name<<'\n';
                    nr++;
                }
            for(i=1;i<=132;i++)
                outputFile<<"Extra"<<i<<'\n';
            outputFile.close();
            inputFile.close();
            nr=nr+132;
            nrAct = nrAct+132;
        }

        //creare fisier in limba romana
        void afisareCast(){
            int i;
            inputFile.open("cast.csv");
            outputFile.open("castRo.csv");
                string line="";
                while(getline(inputFile,line)){
                    string name;
                    string role;

                    stringstream inputString(line);
                    getline(inputString, name, ',');
                    getline(inputString, role);
                    line="";
                    if(role=="actor")nrAct++;
                    nr++;
                    outputFile<<name<<'\n';
                }
            for(i=1;i<=132;i++)
                outputFile<<"Figurant"<<i<<'\n';
            outputFile.close();
            inputFile.close();
            nr=nr+132;
            nrAct = nrAct+132;
        }
        //afisarea bugetului
        void printBuget(int nrDays, float transport, long long machiaj, long long mancare, long long cazare, long long inchiriere, long long total){
            long long auxTot=total*0.21, auxCaz=cazare*0.21;
            outputFile<<nrDays<<','<<transport*0.21<<"$"<<','<<(unsigned long long)machiaj*0.21<<"$"<<','<<(unsigned long long)mancare*0.21<<"$"<<','<<(unsigned long long)auxCaz<<"$"<<','<<(unsigned long long)inchiriere*0.21<<"$"<<','<<(unsigned long long)auxTot<<"$"<<endl;
        }
};
//calculare buget pt transport
class transpBuget{
    private:
        int nrAutocare;
    public:
        float total;
        transpBuget(){
            nrAutocare = nr/50;
            if(nr % 50 != 0) nrAutocare++;
            total = nrAutocare * 5680;
            total = total * 2;
        }
        void add(long long *buget){
            *buget=*buget + this->total;
        }
        ~transpBuget();
};
transpBuget::~transpBuget(void){};

//clasa virtuala ce va fi folosita pentru calcularea urmatoarelor bugete, prin mostenire
class buget{
    protected:
        float oneDayTotal;
    public:
        virtual float total(int nrDays){
            return this->oneDayTotal * nrDays;
        }
        virtual void add(int nrDays, long long* buget){
            *buget=*buget + this->oneDayTotal * nrDays;
        }
        buget(){
            oneDayTotal=0;
        }
        ~buget();
};
buget::~buget(void){};

//clasa pentru calcularea bugetului pt machiaj
class machiajBuget:public buget{
    public:
        machiajBuget(){
            int i, aux;
            oneDayTotal=0;
            for(i=1; i<=nrAct; i++){
               aux= rand() % 5;
                switch(aux){
                    case 0:{
                        //vampir
                        this->oneDayTotal = this->oneDayTotal + 230;
                        break;
                    }   
                    case 1:{
                        //varcolac
                        this->oneDayTotal = this->oneDayTotal + 555;
                        break;
                    }
                    case 2:{
                        //sirena
                        this->oneDayTotal = this->oneDayTotal + 345;
                        break;
                    }
                    case 3:{
                        //clarvazator
                        this->oneDayTotal = this->oneDayTotal + 157;
                        break;
                    }
                    case 4:{
                        //om
                        this->oneDayTotal = this->oneDayTotal + 55;
                        break;
                    }
                }
            }
        }
        ~machiajBuget();
};
machiajBuget::~machiajBuget(void){};

//calculare buget mancare
class mancareBuget:public buget{
    public:
        mancareBuget(){
            oneDayTotal=0;
            int aux, i;
            for(i=1;i<=nr;i++){
                aux=rand()%3;
                switch(aux){
                    case 0:{
                        //mananca orice:
                        oneDayTotal=oneDayTotal+40;
                        break;
                    }
                    case 1:{
                        //vegetarian:
                        oneDayTotal=oneDayTotal+33;
                        break;
                    }
                    case 2:{
                        oneDayTotal=oneDayTotal+46;
                        break;
                    }
                }
            }
        }
        ~mancareBuget();
};
mancareBuget::~mancareBuget(void){};

//calculare buget bauturi
class bauturiBuget:public buget{
    public:
        bauturiBuget(){
            float apa, cafea, suc;
            //apa
            apa=nr/2*6 + nr%2;
            //cafea
            cafea=nr/2*30 + nr%2;
            //suc
            suc=nr*0.8/2*8;
            if(nr%3)suc++;
            oneDayTotal = nr;
        }
        ~bauturiBuget();
};
bauturiBuget::~bauturiBuget(void){};

//calculare buget cazare
class cazareBuget:public buget{
    public:
        cazareBuget(){
            oneDayTotal=(nr-132)/2*350+(nr-132)%2*350 + 132/3*420;
        }
        ~cazareBuget();
};
cazareBuget::~cazareBuget(void){};

//calculare buget inchiriere
class inchiriereBuget{
    public:
        long long buget;
        inchiriereBuget(){
            buget=0;
        }
        inchiriereBuget(int nrDays){
            buget=10000*nrDays*(0.2*(nrDays/10));
        }
        ~inchiriereBuget();
};
inchiriereBuget::~inchiriereBuget(void){};

//CREARE MENIU
//template general, va fi folosit cu literele e,v,f reprezentand tipul de persoana(mananca orice, vegetarian, flexitarian) pentru care 
//realizam meniul si va genera meniu in limba engleza pe 3 zile
template <class T>
void meniu(T x){
    switch(x){
        //eats anything
        case 'e':{
            //soup
            outputFile<<"Chicken soup"<<','<<"Meatball soup"<<','<<"Stroganoff soup"<<endl;
            //main course
            outputFile<<"Lasagna"<<','<<"Salami pizza"<<','<<"Cheeseburger and fries"<<endl;
            //dessert
            outputFile<<"Tiramisu"<<','<<"Lava cake"<<','<<"Vanilla ice cream"<<endl;
            
            break;
        }
        //vegetarian
        case 'v':{
            //soup
            outputFile<<"Creamy tomato soup with croutons"<<','<<"Creamy mushroom soup"<<','<<"Lentil soup"<<endl;
            //main course
            outputFile<<"Eggs Florentine pizza"<<','<<"Quattro formaggi pizza"<<','<<"Quattro formaggi pasta"<<endl;
            //dessert
            outputFile<<"Chocolate ice cream"<<','<<"Lava cake"<<','<<"Chocolate cake"<<endl;
            break;
        }
        //flexitarian
        case 'f':{
            //soup
            outputFile<<"Fish soup"<<','<<"Seafood gumbo"<<','<<"Cioppino"<<endl;
            //main course
            outputFile<<"Salmon with creamy dill sauce"<<','<<"Tilapia Florentine"<<','<<"Cornmeal-crusted catfish"<<endl;
            //dessert
            outputFile<<"Strawberry cake"<<','<<"Strawberry ice-cream"<<','<<"Chocolate cookies and donuts"<<endl;
            break;
        }
    }
}

//template specializat pentru tipul int, va fi folosit cu numerele 1,2,3 semnificand cele 3 tipuri de persoane(mananca orice,
//vegetarian si flexitarian) si va genera fisier in limba romana cu meniul pentru cele 3 zile
template <>
void meniu(int x){
    switch(x){
        //mananca orice
        case '1':{
            //soup
            outputFile<<"Supa de pui"<<','<<"Ciorba de perisoare"<<','<<"Supa Stroganoff"<<endl;
            //main course
            outputFile<<"Lasagna"<<','<<"Pizza salami"<<','<<"Cheeseburger si cartofi prajiti"<<endl;
            //dessert
            outputFile<<"Tiramisu"<<','<<"Lava cake"<<','<<"Inghetata de vanilie"<<endl;
            
            break;
        }
        //vegetarian
        case '2':{
            //soup
            outputFile<<"Supa crema de rosii cu crutoane"<<','<<"Supa crema de ciuperci"<<','<<"Supa de linte"<<endl;
            //main course
            outputFile<<"Pizza Florentine cu oua"<<','<<"Pizza quattro formaggi"<<','<<"Paste quattro formaggi"<<endl;
            //dessert
            outputFile<<"Inghetata de ciocolata"<<','<<"Lava cake"<<','<<"Tort de ciocolata"<<endl;
            break;
        }
        //flexitarian
        case '3':{
            //soup
            outputFile<<"Supa de peste"<<','<<"Gumbo cu fructe de mare"<<','<<"Cioppino"<<endl;
            //main course
            outputFile<<"Somon cu crema de marar"<<','<<"Tilapia Florentine"<<','<<"File de somn in crusta de porumb"<<endl;
            //dessert
            outputFile<<"Tort de capsune"<<','<<"Inghetata de capsune"<<','<<"Biscuiti si gogosi cu ciocolata"<<endl;
            break;
        }
    }
}
int main(){
    //creare obiect pentru scrierea castului in csv
    allCast obj1;
    //input de la tastatura pentru alegerea limbii in care se vor returna fisierele csv
    cout<<"'e' for english /// 'r' pentru romana:\n";
    char c;
    cin>>c;
    long long buget; //bugetul total
    int nrDays; //numarul de zile pentru care calculam
    try{
        switch(c){
            //in limba engleza
            case 'e':{
                obj1.printCast(); //afisare cast in limba engleza
                outputFile.open("menuEng.csv", std::ios::app); //deschidere fisier pentru meniu in engleza
                outputFile<<"Day One"<<','<<"Day Two"<<','<<"Day Three"<<endl; //organizarea meniului pe zile
                //eats anything
                outputFile<<" "<<','<<"Eats anything"<<endl;
                meniu<char>('e'); //folosire template pt cei care mananca orice
                //vegetarian
                outputFile<<" "<<','<<"Vegetarian"<<endl;
                meniu<char>('v'); //folosire template pt vegetarieni
                //flexitarian
                outputFile<<" "<<','<<"Flexitarian"<<endl;
                meniu<char>('f'); //folosire template pt flexitarieni
                outputFile.close(); //inchidere fisier in engleza
                outputFile.open("budgetEng.csv", std::ios::app); //deschidere fisier pentru buget in limba engleza
                    outputFile<<"Nr days/Type"<<','<<"Transport"<<','<<"Makeup"<<','<<"Food& Drinks"<<','
                    <<"Accomodation"<<','<<"Rent"<<','<<"Total"<<endl;
                    break;
                }
            //in limba romana
            case 'r':{
                obj1.afisareCast(); //afisare cast in limba romana
                outputFile.open("meniuRo.csv", std::ios::app); //deschidere fisier pentru meniu in romana
                outputFile<<"Ziua 1"<<','<<"Ziua 2"<<','<<"Ziua 3"<<endl; //organizarea meniului pe zile
                //mananca orice
                outputFile<<" "<<','<<"Mananca orice"<<endl;
                meniu<int>('1'); //folosire template pt cei care mananca orice
                //vegetarian
                outputFile<<" "<<','<<"Vegetarian"<<endl;
                meniu<int>('2'); //folosire template pt vegetarieni
                //flexitarian
                outputFile<<" "<<','<<"Flexitarian"<<endl;
                meniu<int>('3');//folosire template pt flexitarieni
                outputFile.close(); //inchidere fisier in romana
                outputFile.open("bugetRo.csv", std::ios::app); //deschidere fisier pentru buget in limba romana
                outputFile<<"Nr zile/ Tip"<<','<<"Transport"<<','<<"Machiaj"<<','<<"Mancare si apa"<<','
                <<"Cazare"<<','<<"Chirie"<<','<<"Total"<<endl;
                break;
            }
            //caz in care litera introdusa este diferita de e si r
            default:{
                throw "wrong letter";
            }
        }
    }catch(const char* msg){
        cout<<msg;
        return 0; //print eroare, incheierea executiei
    }
    //creare obiecte, calcul pentru o zi
    transpBuget transp; //creare obiect pt buget transport
    machiajBuget machiaj; //creare obiect pt buget machiaj
    mancareBuget mancare; //creare obiect pt buget mancare
    bauturiBuget bauturi; //creare obiect pt buget bauturi
    cazareBuget cazare; //creare obiect pt buget cazare

    //pentru 30 de zile
    buget=0;
    nrDays=30;
    //adaugam fiecare buget la suma totala
    transp.add(&buget);
    machiaj.add(nrDays, &buget);
    mancare.add(nrDays, &buget);
    bauturi.add(nrDays, &buget);
    cazare.add(nrDays, &buget);
    inchiriereBuget inchiriere30(nrDays); //obiect pentru inchiriere pe 30 de zile
    obj1.printBuget(nrDays, transp.total, machiaj.total(nrDays), mancare.total(nrDays)+bauturi.total(nrDays), 
    cazare.total(nrDays), inchiriere30.buget,buget); //afisare in fisier
    
    //pentru 45 de zile
    buget=0;
    nrDays=45;
    transp.add(&buget);
    machiaj.add(nrDays, &buget);
    mancare.add(nrDays, &buget);
    bauturi.add(nrDays, &buget);
    cazare.add(nrDays, &buget);
    inchiriereBuget inchiriere45(nrDays);
    obj1.printBuget(nrDays, transp.total, machiaj.total(nrDays), mancare.total(nrDays)+bauturi.total(nrDays), cazare.total(nrDays), inchiriere45.buget,buget);

    //pentru 60 de zile
    buget=0;
    nrDays=60;
    transp.add(&buget);
    machiaj.add(nrDays, &buget);
    mancare.add(nrDays, &buget);
    bauturi.add(nrDays, &buget);
    cazare.add(nrDays, &buget);
    inchiriereBuget inchiriere60(nrDays);
    obj1.printBuget(nrDays, transp.total, machiaj.total(nrDays), mancare.total(nrDays)+bauturi.total(nrDays), cazare.total(nrDays), inchiriere60.buget,buget);

    //pentru 100 de zile
    buget=0;
    nrDays=100;
    transp.add(&buget);
    machiaj.add(nrDays, &buget);
    mancare.add(nrDays, &buget);
    bauturi.add(nrDays, &buget);
    cazare.add(nrDays, &buget);
    inchiriereBuget inchiriere100(nrDays);
    obj1.printBuget(nrDays, transp.total, machiaj.total(nrDays), mancare.total(nrDays)+bauturi.total(nrDays), cazare.total(nrDays), inchiriere100.buget,buget);

    outputFile.close(); //inchiderea fisierului in romana sau engleza

return 0;
}