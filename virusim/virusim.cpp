//  
// Simulação de propagação de vírus.
// Adaptada de um código proposto por David Joiner (Kean University).
//
// Uso: virusim <tamanho-da-populacao> <nro. experimentos> <probab. maxima> 

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include "Random.h"
#include "Population.h"

#if (defined(PARALLEL_PROBS) || defined(PARALLEL_TRIALS)) && defined(TASKS)
#error Can only use either TASKS, PARALLEL_PROBS, PARALLEL_TRIALS or combination of PARALLE_PROBS and PARALLEL_TRIALS
#endif

void
checkCommandLine(int argc, char **argv, int &size, int &trials, int &probs) {
    if (argc > 1) {
        size = (int) strtol(argv[1],nullptr,10);
    }
    if (argc > 2) {
        trials = (int) strtol(argv[2], nullptr,10);
    }
    if (argc > 3) {
        probs = (int) strtol(argv[3], nullptr,10);
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
#pragma ide diagnostic ignored "misc-throw-by-value-catch-by-reference"
#pragma ide diagnostic ignored "bugprone-infinite-loop"
int
main(int argc, char *argv[]) {

    // parâmetros dos experimentos
    int population_size = 30;
    int n_trials = 5000;
    int n_probs = 101;

    double *percent_infected; // percentuais de infectados (saída)
    double *prob_spread;      // probabilidades a serem testadas (entrada)
    double prob_min = 0.0;
    double prob_max = 1.0;
    double prob_step;
    int base_seed = 100;

    #if defined(TASKS)
    int last_ip = -1; // flow control for output on tasks
    #endif

    checkCommandLine(argc, argv, population_size, n_trials, n_probs);

    try {

        #if !defined(PARALLEL_PROBS) && !defined(PARALLEL_TRIALS) && !defined(TASKS)
        auto* population = new Population(population_size);
        #endif
        Random rand;

        prob_spread = new double[n_probs];
        percent_infected = new double[n_probs];

        prob_step = (prob_max - prob_min) / (double) (n_probs - 1);

        #if defined(PARALLEL_PROBS) || defined(PARALLEL_TRIALS) || defined(TASKS)
        auto start = std::chrono::steady_clock::now();
        if (!getenv("OUTPUT_ONLY_TIME")) {
        #endif
            printf("Probabilidade, Percentual Infectado\n");
        #if defined(PARALLEL_PROBS) || defined(PARALLEL_TRIALS) || defined(TASKS)
        }
        #endif

        // para cada probabilidade, calcula o percentual de pessoas infectadas
        #if defined(PARALLEL_PROBS)
        #pragma omp parallel for shared(prob_spread, percent_infected)
        #elif defined(TASKS)
        #pragma omp parallel
        #pragma omp single
        #pragma omp taskloop
        #endif
        for (int ip = 0; ip < n_probs; ip++) {

            #if defined(TASKS)
            #pragma omp taskgroup
            {
            #endif
            prob_spread[ip] = prob_min + (double) ip * prob_step;
            percent_infected[ip] = 0.0;
            rand.setSeed(base_seed + ip); // nova sequência de números aleatórios

            #if defined(PARALLEL_PROBS) && !defined(PARALLEL_TRIALS)
            auto *population = new Population(population_size);
            #endif
            // executa vários experimentos para esta probabilidade
            #if defined(PARALLEL_TRIALS)
            #pragma omp parallel for shared(percent_infected)
            #endif
            for (int it = 0; it < n_trials; it++) {
                #if defined(PARALLEL_TRIALS) || defined(TASKS)
                #if defined(TASKS)
                #pragma omp task shared(percent_infected) firstprivate(ip, it, prob_spread) depend(out: percent_infected[ip])
                {
                #endif
                auto *population = new Population(population_size);
                #endif
                // queima floresta até o fogo apagar
                population->propagateUntilOut(population->centralPerson(), prob_spread[ip], rand);
                double infected = population->getPercentInfected();
                #if defined(PARALLEL_TRIALS) || defined(TASKS)
                delete population;
                #pragma omp critical
                #endif
                percent_infected[ip] += infected;
                #if defined(TASKS)
                }
                #endif
            }

            #if defined(PARALLEL_PROBS) && !defined(PARALLEL_TRIALS)
            delete population;
            #endif

            #if defined(TASKS)
            #pragma omp task shared(last_ip) firstprivate(ip, percent_infected, prob_spread, n_trials) depend(in: percent_infected[ip])
            {
                    #pragma omp flush(last_ip)
                    #pragma clang diagnostic push
                    #pragma clang diagnostic ignored "-Wfor-loop-analysis"
                    while (last_ip + 1 != ip) {
                        #pragma omp taskyield
                        #pragma omp flush(last_ip)
                    }
                    #pragma clang diagnostic pop
                    #pragma omp critical
                    last_ip = ip;
            #endif
            // calcula média dos percentuais de árvores queimadas
            percent_infected[ip] /= n_trials;

            #if !defined(PARALLEL_PROBS)
            #if defined(PARALLEL_TRIALS) || defined(TASKS)
            if (!getenv("OUTPUT_ONLY_TIME")) {
            #endif
                #if defined(TASKS)
                #pragma omp critical
                #endif
                // mostra resultado para esta probabilidade
                printf("%lf, %lf\n", prob_spread[ip], percent_infected[ip]);
            #if defined(PARALLEL_TRIALS) || defined(TASKS)
            }
            #endif
            #endif
            #if defined(TASKS)
            }}
            #endif
        }

        #if defined(PARALLEL_PROBS) || defined(PARALLEL_TRIALS) || defined(TASKS)
        double elapsed_time = double(std::chrono::duration_cast <std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - start).count());

        if (getenv("OUTPUT_ONLY_TIME")) {
            printf("%.0lf", elapsed_time);
        } else {
            #if defined(PARALLEL_PROBS)
            for (int i = 0; i < n_probs; i++) {
                // mostra resultado para esta probabilidade
                printf("%lf, %lf\n", prob_spread[i], percent_infected[i]);
            }
            #endif
            printf("Time: %.0lf ms\n", elapsed_time);
        }
        #endif

        delete[] prob_spread;
        delete[] percent_infected;
    }
    catch (std::bad_alloc) {
        std::cerr << "Erro: alocacao de memoria" << std::endl;
        return 1;
    }

    return 0;
}
#pragma clang diagnostic pop

