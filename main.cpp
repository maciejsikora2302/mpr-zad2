#include <iostream>
#include <random>
#include <omp.h>
#include <string>
#include <fstream>

const int THREADS = 4;
const int SIZE = 1000000; //musi być większy niż THREADS*100
double array[SIZE];

const int TIMES_TO_REPEAT = 25;

const int STATIC = 1; //domyślnie liczba_iteracji / liczba_wątków, każdy wątek dostaje chunk do wykonania
const int DYNAMIC = 2; //domyślnie 1, każdy wątek bierze kolejny chunk
const int GUIDED = 3; //chunki skalują się z ilością nie przydzielonych jeszcze porcji. liczba_nie_przydzielonych_iteracji / liczba_wątków i zmierzają do 1
const int RUNTIME = 4; //bierze z zmiennej środowiskowej jedną z pozostałych opcji

const int DEFAULT = 1;
const int MODIFIED = 2;

void do_work(int threads, int schedule_style, int mode) {
    std::uniform_real_distribution<double> distribution(0, 1);
    double t0 = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {


        std::default_random_engine generator; 

        switch (schedule_style){
            case STATIC:
                if(mode == DEFAULT){
                    #pragma omp for schedule(static)
                    for (int i = 0; i < SIZE; i++) {
                        array[i] = distribution(generator);
                    }
                }else if(mode == MODIFIED){
                    #pragma omp for schedule(static, 1)
                    for (int i = 0; i < SIZE; i++) {
                        array[i] = distribution(generator);
                    }
                }
                break;
            case DYNAMIC:
                if(mode == DEFAULT){
                    #pragma omp for schedule(dynamic)
                    for (int i = 0; i < SIZE; i++) {
                        array[i] = distribution(generator);
                    }
                }else if(mode == MODIFIED){
                    #pragma omp for schedule(dynamic, SIZE/(omp_get_num_threads()*100))
                    for (int i = 0; i < SIZE; i++) {
                        array[i] = distribution(generator);
                    }
                }
                break;
            case GUIDED:
                #pragma omp for schedule(guided)
                for (int i = 0; i < SIZE; i++) {
                    array[i] = distribution(generator);
                }
                break;
        }
    }
    double time = omp_get_wtime() - t0;
    std::ofstream f;
    if(schedule_style == STATIC && mode == DEFAULT){
        f.open("f1.txt", std::ios_base::app);
    }else if(schedule_style == STATIC && mode == MODIFIED){
        f.open("f2.txt", std::ios_base::app);
    }else if(schedule_style == DYNAMIC && mode == DEFAULT){
        f.open("f3.txt", std::ios_base::app);
    }else if(schedule_style == DYNAMIC && mode == MODIFIED){
        f.open("f4.txt", std::ios_base::app);
    }else if(schedule_style == GUIDED && mode == DEFAULT){
        f.open("f5.txt", std::ios_base::app);
    }

    f << "," << time;
}

std::ofstream get_file(int style, int mode, std::ofstream f1, std::ofstream f2, std::ofstream f3, std::ofstream f4, std::ofstream f5){
    
}

int main(int argc, char* argv[]) {

    std::ofstream f1("f1.txt");
    std::ofstream f2("f2.txt");
    std::ofstream f3("f3.txt");
    std::ofstream f4("f4.txt");
    std::ofstream f5("f5.txt");

    f1 << "STATIC,default," << SIZE/THREADS;
    f2 << "STATIC,modified," << 1;
    f3 << "DYNAMIC,default," << 1;
    f4 << "DYNAMIC,modified," << SIZE/(THREADS*100);
    f5 << "GUIDED,default," << "to_allocate/threads";

    int schedule_style[] = {STATIC,DYNAMIC,GUIDED}; 
    int modes[] = {DEFAULT, MODIFIED};
    
    for (int i = 0; i < TIMES_TO_REPEAT; i++){
        for (int threads = 1; threads <= 4; threads++) {
            for (int ss = 0; ss < 3; ss++){
                int style = schedule_style[ss];
                for (int m = 0; m < 2; m++){
                    int mode = modes[m];
                    if (style == MODIFIED && mode == GUIDED) break;
                    do_work(threads, style, mode);
                }
            }
        }
    }

    std::ofstream f1_2("f1.txt", std::ios_base::app);
    std::ofstream f2_2("f2.txt", std::ios_base::app);
    std::ofstream f3_2("f3.txt", std::ios_base::app);
    std::ofstream f4_2("f4.txt", std::ios_base::app);
    std::ofstream f5_2("f5.txt", std::ios_base::app);

    f1_2 << "\n";
    f2_2 << "\n";
    f3_2 << "\n";
    f4_2 << "\n";
    f5_2 << "\n";
    return 0;
}