# Game of Life - OpenMP

# How to Build
>make

# How to Run
>export OMP_NUM_THREADS=1,2,4  
./bin/game_of_life <input_file> <output_file>  

Warning: bin folder is been created by make automatically.Don't create it by hand.  


# How to Clean
> make clean

# Input File Format
5 5  
| |\*| | | |  
| | |\*| | |  
|\*|\*|\*| | |  
| | | | | |  
| | | | | |  

+ ' ' represents the DEAD cell
+ \* representes the ALIVE cell

# Iterations (Generations)
> Default is 1000  