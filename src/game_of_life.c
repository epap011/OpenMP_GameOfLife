#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

char** read_input(FILE *input_file, int* rows, int* cols);
void   print_cosmos(char** cosmos, int rows, int cols);
char** compute_next_generation(char** cosmos, int rows, int cols);
void   dealloc_cosmos(char** cosmos, int rows);

#define ALIVE '*'
#define DEAD  ' '

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
    
    print_cosmos(cosmos, rows, cols);
    cosmos = compute_next_generation(cosmos, rows, cols);
    print_cosmos(cosmos, rows, cols);

    

    FILE *output_file = fopen(argv[2], "w");
    if(output_file == NULL) {
        printf("Error: Cannot open output file!\n");
        exit(EXIT_FAILURE);
    }

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

    for(int i = 1; i < rows - 1; i++) {
        for(int j = 1; j < cols - 1; j++) {
            alive_neighbours = 0;
            for(int k = i - 1; k <= i + 1; k++) {
                for(int l = j - 1; l <= j + 1; l++) {
                    if(k == i && l == j) continue;
                    if(cosmos[k][l] == ALIVE) alive_neighbours++;
                }
            }

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

void dealloc_cosmos(char** cosmos, int rows) {
    for(int i = 0; i < rows; i++) {
        free(cosmos[i]);
    }
    free(cosmos);
}