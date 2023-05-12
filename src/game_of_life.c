#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

char** read_input(FILE *input_file, int* rows, int* cols);
void   print_cosmos(char** cosmos, int rows, int cols);
void   print_cosmos_to_file(FILE *output_file, char** cosmos, int rows, int cols);
void   compute_N_generation(int rows, int cols, int N);
int    compute_neighbours(char** cosmos, int i, int j);
char** create_cosmos(int rows, int cols);
void   init_dead_cosmos(char** cosmos, int rows, int cols);
void   dealloc_cosmos(char** cosmos, int rows);

#define ALIVE '*'
#define DEAD  ' '
#define NUM_OF_GENERATIONS 1000

char** cosmos;
char** tmp_cosmos;

int main(int argc, char *argv[]) {
    
    if(argc != 3) {
        printf("Error: Wrong input! Usage: ./game_of_life input_file output_file\n");
        exit(EXIT_FAILURE);
    }

    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL) {
        printf("Error: Cannot open input file!\n");
        exit(EXIT_FAILURE);
    }

    int rows, cols;
    cosmos     = read_input(input_file, &rows, &cols);
    tmp_cosmos = create_cosmos(rows, cols);
    init_dead_cosmos(tmp_cosmos, rows, cols);

    struct timespec start_time, end_time;
    
    /*----<START MEASUREMENT>----*/
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    compute_N_generation(rows, cols, NUM_OF_GENERATIONS);
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    /*----<END MEASUREMENT>----*/
    
    double elapsed = (end_time.tv_sec - start_time.tv_sec);
    elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %.3f seconds\n", elapsed);

    FILE *output_file = fopen(argv[2], "w");
    if(output_file == NULL) {
        printf("Error: Cannot open output file!\n");
        exit(EXIT_FAILURE);
    }

    if(NUM_OF_GENERATIONS % 2 == 0) print_cosmos_to_file(output_file, tmp_cosmos, rows, cols);
    else print_cosmos_to_file(output_file, cosmos, rows, cols);

    dealloc_cosmos(cosmos, rows);
    dealloc_cosmos(tmp_cosmos, rows);

    fclose(input_file);
    fclose(output_file);      

    return 0;
}

void compute_N_generation(int rows, int cols, int N) {
    int i, j, k, l, gen, alive_neighbours;

    #pragma omp parallel shared(cosmos, tmp_cosmos) private(gen, i, j, k, l, alive_neighbours)
    for(gen = 0; gen < N; gen++) {
        #pragma omp for
        for(i = 1; i < rows-1; i++) {
            for(j = 1; j < cols-1; j++) {
                alive_neighbours = 0;
                /* with this way, we can reuse these arrays, with no need to re-initailize them */
                if(gen % 2 == 0) {
                        for(k = i - 1; k <= i + 1; k++) {
                        for(l = j - 1; l <= j + 1; l++) {
                            if(k == i && l == j) continue;
                            if(cosmos[k][l] == ALIVE) alive_neighbours++;
                        }
                    }
                    if(cosmos[i][j] == ALIVE) {
                        if(alive_neighbours < 2 || alive_neighbours > 3) tmp_cosmos[i][j] = DEAD;
                        else tmp_cosmos[i][j] = ALIVE;
                    }
                    else {       
                        if(alive_neighbours == 3) tmp_cosmos[i][j] = ALIVE;
                        else tmp_cosmos[i][j] = DEAD;
                    }
                }
                else {
                    for(k = i - 1; k <= i + 1; k++) {
                        for(l = j - 1; l <= j + 1; l++) {
                            if(k == i && l == j) continue;
                            if(tmp_cosmos[k][l] == ALIVE) alive_neighbours++;
                        }
                    }
                    if(tmp_cosmos[i][j] == ALIVE) {
                        if(alive_neighbours < 2 || alive_neighbours > 3) cosmos[i][j] = DEAD;
                        else cosmos[i][j] = ALIVE;
                    }
                    else {       
                        if(alive_neighbours == 3) cosmos[i][j] = ALIVE;
                        else cosmos[i][j] = DEAD;
                    }
                }
            }
        }    
    }
}

char** read_input(FILE *input_file, int* rows, int* cols) {
    int i, j;
    char c;

    fscanf(input_file, "%d %d", rows, cols);
    
    *rows = *rows + 2;
    *cols = *cols + 2;

    char** cosmos = (char**) malloc(*rows * sizeof(char*));
    for(int i = 0; i < *rows; i++) {
        cosmos[i] = (char*) malloc(*cols * sizeof(char));
    }

    for(int i = 0; i < *rows; i++) {
        for(int j = 0; j < *cols; j++) {
            cosmos[i][j] = DEAD;
        }
    }

    i = j = 1;
    while((c = fgetc(input_file)) != EOF) {
        if(c == '|' || c == '\n') continue;
        c == '*' ? (cosmos[i][j++] = ALIVE) : (cosmos[i][j++] = DEAD);
        
        if(j == *cols - 1) {
            i++;
            j = 1;
        }
    }

    return cosmos;
}

void print_cosmos_to_file(FILE *output_file, char** cosmos, int rows, int cols) {
    for(int i = 1; i < rows-1; i++) {
        for(int j = 1; j < cols-1; j++) {
            fprintf(output_file, "|%c", cosmos[i][j]);
        }
        fprintf(output_file, "|\n");
    }
    fprintf(output_file, "\n");
}

char** create_cosmos(int rows, int cols) {
    char** cosmos = (char**) malloc(rows * sizeof(char*));
    for(int i = 0; i < rows; i++) {
        cosmos[i] = (char*) malloc(cols * sizeof(char));
    }
    return cosmos;
}

void init_dead_cosmos(char** cosmos, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cosmos[i][j] = DEAD;
        }
    }
}

void dealloc_cosmos(char** cosmos, int rows) {
    for(int i = 0; i < rows; i++) {
        free(cosmos[i]);
    }
    free(cosmos);
}