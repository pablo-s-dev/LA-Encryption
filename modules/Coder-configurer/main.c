#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#ifdef _WIN32
    #include <direct.h>
    #define makedir(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define makedir(dir) mkdir(dir, 0700)
#endif

#define CHARACTERS "abcdefghijklmnopqrstuvwxyz.,#"
#define CONFIG_DIR "./config"
#define MAPPING_SIZE 29

#define CODER_FILENAME "coder.encdata"
#define DECODER_FILENAME "decoder.encdata"
#define MAPPINGS_FILENAME "mappings.encdata"


int save_array(int* arr, int n, char* dir, char* filename){

    if (makedir(dir) == -1) {

        if (errno != EEXIST) {
            perror("Unable to create directory");
            return -1;
        }
    }
    int blocks_written = 0;

    // +1 for the \n string terminator
    char* file_path = (char* ) malloc(strlen(dir) + strlen(filename) + 2);

    strcpy(file_path, dir);

    strcat(file_path, "/");

    strcat(file_path, filename);


    FILE* f = fopen(file_path, "wb");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    blocks_written += fwrite(arr, sizeof(int), n, f);

    fclose(f);

    free(file_path);

    if(blocks_written  == n){
        return 1;
    }
    return -1;

}

int save_square_matrix(int** matrix, int n, char* dir, char* filename){

    if (makedir(dir) == -1) {

        if (errno != EEXIST) {
            perror("Unable to create directory");
            return -1;
        }
    }

    int blocks_written = 0;
    int i;

    char* file_path = (char* ) malloc(strlen(dir) + strlen(filename) + 2);

    if(file_path == NULL ){
        printf("\nMemory allocation failed!\n");
        return -1;
    }

    strcpy(file_path, dir);

    strcat(file_path, "/");

    strcat(file_path, filename);


    FILE* f = fopen(file_path, "wb");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }
    for(i = 0; i < n; i++){
        blocks_written += fwrite(matrix[i], sizeof(int), n, f);
    }

    fclose(f);

    free(file_path);

    if(blocks_written  == n * n){
        return 1;
    }
    return -1;

}

int main(int argc, char* argv[]){

    char* config_dir;
    int** coder_matrix;
    int** decoder_matrix;
    int ch_to_num_mapping[29] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    int custom_mapping = -1;
    int n;
    int i, j;
    int saved_files = 0;


    if(argc > 1){
        config_dir = argv[1];
    }
    else{
        config_dir = CONFIG_DIR;
    }

    printf("Defining the coder matrix with shape n x n. \n\n");

    printf("Define n: ");

    scanf("%d", &n);

    printf("\n");

    coder_matrix = (int**) malloc(n * sizeof(int*));

    if (coder_matrix == NULL) {
        printf("Memory allocation failed for coder_matrix!\n");
        return -1;
    }

    for(i=0; i<n; i++){

        coder_matrix[i]  = (int*) malloc(n * sizeof(int));

        for(j=0; j<n; j++){

            printf("Item [%d][%d]: ", i, j);
            scanf("%d", &(coder_matrix[i][j]));

        }

    }
    printf("\n\nDefining the decoder matrix. \n\n");

    decoder_matrix = (int**) malloc(n * sizeof(int*));

    for(i=0; i<n; i++){

        decoder_matrix[i]  = (int*) malloc(n * sizeof(int));

        for(j=0; j<n; j++){

            printf("Item [%d][%d]: ", i, j);
            scanf("%d", &(decoder_matrix[i][j]));

        }

    }

    printf("\nThe default custom mapping is: \n\n");

    for(i = 0; i<MAPPING_SIZE; i++){

        // Just for alignment purposes
        if( i > 8){
            printf("%c  ", CHARACTERS[i]);
        }
        else{
            printf("%c ", CHARACTERS[i]);
        }
    }

    printf("\n");

    for(i = 0; i<MAPPING_SIZE; i++){
        printf("%d ", ch_to_num_mapping[i]);
    }

    while(custom_mapping != 1 && custom_mapping != 0){

        printf("\n\nDo you wanna use a custom character to number mapping? \n");

        printf("1 - Yes\n");
        printf("0 - No\n");

        printf("\nAns: ");

        scanf("%d", &custom_mapping);


        if(custom_mapping != 1 && custom_mapping != 0){
            printf("\nInvalid choice, try again.\n\n");
        }
    }


    if(custom_mapping == 1){
        printf("\nNow, please provide the character to number mapping\n");

        for( i = 0; i < MAPPING_SIZE; i++ ){
            printf("Character [%c] maps to: ", CHARACTERS[i]);
            scanf("%d", &ch_to_num_mapping[i]);
        }
    }
    else{
        printf("\nOkay, we'll use the default char mappings then.\n");
    }

    printf("\nSaving the files...\n\n");

    // Saving the matrices
    saved_files += save_square_matrix(coder_matrix, n, config_dir, CODER_FILENAME);
    saved_files += save_square_matrix(decoder_matrix, n, config_dir, DECODER_FILENAME);

    // Saving the mapping file
    saved_files += save_array(ch_to_num_mapping, MAPPING_SIZE, config_dir, MAPPINGS_FILENAME);

    // Freeing stuff
    for(i=0; i<n; i++){
        free(coder_matrix[i]);
        free(decoder_matrix[i]);
    }

    free(coder_matrix);
    free(decoder_matrix);

    printf("Saved files: %d\n", saved_files);

    if(saved_files == 3){
        printf("The matrices and the character mapping files have been created in the directory %s", config_dir);
    }
    else{
        printf("An error occurred! Please restart the program and try again.");
    }
    return 0;
}
