#include <iostream>
#include <time.h>
#include <math.h>
#include <chrono>
#include <ctime>
#include <fstream>

#define L 24

using namespace std;
using namespace chrono;

// zmienne
int mcs0 = 30000;
int mcs = 230000;
int divider = 100;
int spiny[L][L][L] = {1};
float temp[56] = {};
int sg[L] = {};
int sd[L] = {};
int kroki = 0;

int MAG_ARRAY_SIZE = 2000;

float tablica_magnetyzacji[2000] = {};
float wariancja = 0.0;
float mag_pom = 0.0;

void initialize(){
    for(int x=0; x<L; x++){
        for(int y=0; y<L; y++){
            for(int z=0; z<L; z++){
                spiny[x][y][z] = 1;
            }
        }
    }
    float tmp_temp = 1.5;
    for(int i=0; i<56; i++){
        temp[i] = tmp_temp;
        tmp_temp = tmp_temp + 0.1;
    }

    for(int i=0; i<L-1; i++){
        sg[i] = i+1;
        sd[i+1] = i;
    }
    sg[L-1] = 0;
    sd[0] = L-1;
}

void print_arrays(){
    cout<<"temp: [";
    for(int i=0; i<56; i++){
        cout<<temp[i]<<", ";
    }
    cout<<"] \n sg: [";
    for(int i=0; i<L; i++){
        cout<<sg[i]<<", ";
    }
    cout<<"] \n sd: [";
    for(int i=0; i<L; i++){
        cout<<sd[i]<<", ";
    }
    cout<<"]"<<endl;
}

double average0 (float values[], int size) {
        double sum = 0;

        for (int i = 0; i < size; i++) {
            sum += values[i];
        }

        return sum / size;
    }

double variance (float values[], int size) {
    double avg = average0(values, size);
    double var = 0;

    for (int i = 0; i < size; i++) {
        var += (values[i] - avg) * (values[i] - avg);
    }

    return var / size;
}

int main() {
    clock_t full_start = clock();

    srand( (unsigned)time( NULL ) );

    fstream plik;
    plik.open("test.txt", ios::out);

    initialize();
    print_arrays();

    // wyliczenie kroków
    kroki = 0;
    for(int i=0; i<mcs; i++){
        if((i >= mcs0) && (i%100 == 99)){
            kroki = kroki+1;
        }
    }
    cout<<"Kroki: "<<kroki<<endl;

    // glowna petla
    double mag = 0.0;
    float T_red = 0.0;
    int delta_U = 0;
    double suma = 0;
    float temperatura_Zredukowana = 0.1;
    while (temperatura_Zredukowana <= 10){
    //for(int t=0; t<56; t++){
        clock_t temp_start = clock();
        //T_red = temp[t];
        mag = 0.0;
        mag_pom = 0.0;
        int indeks = 0;
        for(int i=0; i<mcs; i++){
            for(int x=0; x<L; x++){
                for(int y=0; y<L; y++){
                    for(int z=0; z<L; z++){
                        delta_U = (2 * spiny[x][y][z]) * (spiny[x][sg[y]][z] + spiny[x][sd[y]][z] + spiny[sg[x]][y][z] + spiny[sd[x]][y][z] +
                        spiny[x][y][sg[z]] + spiny[x][y][sd[z]] );
                        if(delta_U < 0){
                            spiny[x][y][z] = (-1 * spiny[x][y][z]);
                        }
                        else if( ((double) rand()/RAND_MAX) <= exp((double)((-1*delta_U)/temperatura_Zredukowana)) ){
                            spiny[x][y][z] = (-1 * spiny[x][y][z]);
                        }
                    } //for z
                } // for y
            } // for x
            if (i>=mcs0 && i%divider==0){
                suma = 0;
                for(int x=0; x<L; x++){
                    for(int y=0; y<L; y++){
                        for(int z=0; z<L; z++){
                            suma = suma +  spiny[x][y][z];
                        }
                    }
                }
                mag_pom += suma;
                mag_pom = mag_pom / (L*L*L);
                mag = mag + (double)abs(suma/(double)(L*L*L));
                tablica_magnetyzacji[indeks] = abs(mag_pom);
                indeks += 1;
            }
        }// for i

        float variance_val = variance(tablica_magnetyzacji, 2000);
        wariancja = ((L*L*L) / (temperatura_Zredukowana)) * (variance_val);

        clock_t temp_stop = clock();
        plik<<temperatura_Zredukowana<<" "<<mag/kroki<<" "<<wariancja<<endl;
        cout<<temperatura_Zredukowana<<" "<<mag/kroki<<" "<<wariancja<<" t:"<<double(temp_stop - temp_start) / CLOCKS_PER_SEC<<" [s]"<<endl;
    //} // for t
    if (temperatura_Zredukowana >= 3.0 && temperatura_Zredukowana < 6.0)
    {
        temperatura_Zredukowana += 0.01;
    }
    else
    {
        temperatura_Zredukowana += 0.1;
    }

    }
    clock_t full_stop = clock();
    double full_duration = double(full_stop - full_start) / CLOCKS_PER_SEC;
    cout <<"Full Duration: " << full_duration<< endl;

    plik.close();

    cout << "exit";
    return 0;
}
