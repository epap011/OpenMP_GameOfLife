#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

char** read_input(FILE *input_file, int* rows, int* cols);
void   print_cosmos(char** cosmos, int rows, int cols);
void   print_cosmos_to_file(FILE *output_file, char** cosmos, int rows, int cols);
char** compute_next_generation(char** cosmos, int rows, int cols);
int    compute_neighbours(char** cosmos, int i, int j);
void   dealloc_cosmos(char** cosmos, int rows);

#define ALIVE '*'
#define DEAD  ' '
#define NUM_OF_GENERATIONS 100

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
    char** cosmos = read_input(input_file, &rows, &cols);

    struct timespec start_time, end_time;
    
    /*----<START MEASUREMENT>----*/
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    for(int i = 0; i < NUM_OF_GENERATIONS; i++) {
        printf("Generation %d\n", i+1);
        cosmos = compute_next_generation(cosmos, rows, cols);
    }
    
    /*----<END MEASUREMENT>----*/
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed = (end_time.tv_sec - start_time.tv_sec);
    elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %.3f seconds\n", elapsed);

    FILE *output_file = fopen(argv[2], "w");
    if(output_file == NULL) {
        printf("Error: Cannot open output file!\n");
        exit(EXIT_FAILURE);
    }

    print_cosmos_to_file(output_file, cosmos, rows, cols);

    fclose(input_file);
    fclose(output_file);      

    return 0;
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

void print_cosmos(char** cosmos, int rows, int cols) {

    for(int i = 1; i < rows-1; i++) {
        for(int j = 1; j < cols-1; j++) {
            printf("|%c", cosmos[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
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

char** compute_next_generation(char** cosmos, int rows, int cols) {
    int alive_neighbours;
    
    char** next_gen_cosmos = (char**) malloc(rows * sizeof(char*));

    for(int i = 0; i < rows; i++) {
        next_gen_cosmos[i] = (char*) malloc(cols * sizeof(char));
    }

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            next_gen_cosmos[i][j] = DEAD;
        }
    }

    int i, j;
    for(i = 1; i < rows - 1; i++) {
        for(j = 1; j < cols - 1; j++) {
            alive_neighbours = compute_neighbours(cosmos, i, j);
            if(cosmos[i][j] == ALIVE) {
                if(alive_neighbours < 2 || alive_neighbours > 3) next_gen_cosmos[i][j] = DEAD;
                else next_gen_cosmos[i][j] = ALIVE;
            }
            
            if(cosmos[i][j] == DEAD) {
                if(alive_neighbours == 3) next_gen_cosmos[i][j] = ALIVE;
            }
        }
    }
    

    dealloc_cosmos(cosmos, rows);
    return next_gen_cosmos;
}

int compute_neighbours(char** cosmos, int i, int j) {
    int alive_neighbours = 0;
    for(int k = i - 1; k <= i + 1; k++) {
        for(int l = j - 1; l <= j + 1; l++) {
            if(k == i && l == j) continue;
            if(cosmos[k][l] == ALIVE) alive_neighbours++;
        }
    }
    return alive_neighbours;
}

void dealloc_cosmos(char** cosmos, int rows) {
    for(int i = 0; i < rows; i++) {
        free(cosmos[i]);
    }
    free(cosmos);
}