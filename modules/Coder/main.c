#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CODER_FILENAME "coder.honovan"
#define DECODER_FILENAME "decoder.honovan"
#define MAPPINGS_FILENAME "mappings.honovan"
#define ASCII_LOWER_A 97
#define ASCII_LOWER_Z 122
#define DOT_POS 26
#define COMMA_POS 27
#define SPACE_POS 28


#include <errno.h>
#ifdef _WIN32
    #include <direct.h>
    #define makedir(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define makedir(dir) mkdir(dir, 0700)
#endif


char* read_line() {
    int bufsize = 128;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "read_line: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += 128;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "read_line: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void* load_matrix(char* dir, char* filename){

    int** matrix;
    void** data = malloc(sizeof(int*) * 2);
    char* file_path = (char* ) malloc(strlen(dir) + strlen(filename) + 1);
    FILE* f;
    int n, num_amount;
    int i;
    int* matrix_shape = (int*) malloc(sizeof(int) * 2);

    if(matrix_shape == NULL || data == NULL || file_path == NULL){
        printf("\nMemory allocation failed!\n");
        return NULL;
    }


    strcpy(file_path, dir);

    strcat(file_path, "/");

    strcat(file_path, filename);

    printf("\nMatrix path %s", file_path);

    f = fopen(file_path, "rb");

    if (f == NULL) {
        printf("\nError opening file!\n");
        printf("Maybe you are using '\' instead of '/'?");
        return NULL;
    }

    fseek(f, 0, SEEK_END);

    num_amount = ftell(f) / sizeof(int);
    rewind(f);

    n = sqrt(num_amount);

    matrix = (int**) malloc(sizeof(int*) * n);

    if(matrix == NULL){
        printf("\nMemory allocation failed!\n");
        return NULL;
    }


    for(i=0; i<n; i++){
        matrix[i] = (int*) malloc(sizeof(int) * n);

        if(matrix[i]  == NULL){
            printf("\nMemory allocation failed!\n");
            return NULL;
        }

        fread(matrix[i], sizeof(int), n, f);
    }

    free(file_path);

    matrix_shape[0] = n;
    matrix_shape[1] = n;

    data[0] = (void*) matrix;
    data[1] = (void*) matrix_shape;


    return data;

}

int* load_arr(char* dir, char* filename){

    int* arr;
    char* file_path = (char* ) malloc(strlen(dir) + strlen(filename) + 1);

    if (file_path == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    FILE* f;
    long num_amount;

    strcpy(file_path, dir);

    strcat(file_path, "/");

    strcat(file_path, filename);


    f = fopen(file_path, "rb");

    if (f == NULL) {
        printf("\nError opening file!\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);

    num_amount = ftell(f) / sizeof(int);
    rewind(f);


    arr = (int*) malloc(sizeof(int) * num_amount);

    if(arr == NULL){
        printf("\nMemory allocation failed!\n");
        return NULL;
    }

    fread(arr, sizeof(int), num_amount, f);

    fclose(f);

    free(file_path);

    return arr;

}

void* txt_to_num_matrix(char* plain_text_path, int* mapping_arr, int rows){


    FILE* f;
    f = fopen(plain_text_path, "r");


    if (f == NULL) {
        printf("\nError opening file!\n");
        return NULL;
    }

    int** num_matrix = (int**) malloc(sizeof(int*) * rows);
    void** data = malloc(sizeof(int*) * 2);
    long file_size;
    int ch_amount;
    int cols;
    int padding;
    char* row_text;
    int chars_read;
    int i = 0;
    int j;
    int num;
    int* msg_shape = (int*) malloc(sizeof(int) * 2);


    if(num_matrix == NULL || msg_shape == NULL || data == NULL || msg_shape == NULL){
        printf("\nMemory allocation failed!\n");
        return NULL;
    }



    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    ch_amount = file_size / sizeof(char);
    rewind(f);

    cols = ceil( (float) ch_amount / rows);

    padding = cols * rows - ch_amount;

    printf("\nPadding needed: %d", padding);


    while(!feof(f)){

        num_matrix[i] = (int*) malloc(sizeof(int) * rows);

        row_text = (char*) malloc(sizeof(char) * cols);



        fgets(row_text, cols + 1, f);

        chars_read = strlen(row_text);

        if(num_matrix[i] == NULL || row_text == NULL ){
            printf("\nMemory allocation failed!\n");
            return NULL;
        }

        strlwr(row_text);

        if(chars_read < cols){
            for(j=0; j<padding; j++){

                row_text[ chars_read + j ] = '#';

            }
        }

        for(j=0; j<cols; j++){


            if(row_text[j] >= ASCII_LOWER_A && row_text[j] <= ASCII_LOWER_Z){
                num = mapping_arr[row_text[j] - ASCII_LOWER_A ];
            }
            else{
                switch (row_text[j]){

                    case '.':
                        num = mapping_arr[ DOT_POS ];
                        break;
                    case ',':
                        num = mapping_arr [ COMMA_POS ];
                        break;
                    case ' ':
                        num = mapping_arr[ SPACE_POS ];
                        break;
                    case '#':
                        num = mapping_arr[ SPACE_POS ];
                        break;
                }

            }

            num_matrix[i][j] = num;

        }

        i++;

    }
    msg_shape[0] = rows;
    msg_shape[1] = cols;

    data[0] = (void*) num_matrix;
    data[1] = (void*) msg_shape;

    return data;
}


int matrix_to_txt(int** matrix, int* shape, char* dir, char* filename){

    if (makedir(dir) == -1) {

        if (errno != EEXIST) {
            perror("Unable to create directory");
            return -1;
        }
    }

    int numbers_written = 0;
    int i, j;

    // +1 for the \0 string terminator
    char* file_path = (char* ) malloc(strlen(dir) + strlen(filename) + 1);

    if(file_path == NULL){
        printf("\nMemory allocation failed!\n");
        return -1;
    }

    strcpy(file_path, dir);

    strcat(file_path, "/");

    strcat(file_path, filename);

    printf("\nMatrix path %s", file_path);

    FILE* f = fopen(file_path, "w");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }


    for(i = 0; i < shape[0]; i++){

        for(j = 0; j < shape[1]; j++){
            // Storing a line
            if (fprintf(f, "%d", matrix[i][j]) > 0) {
                numbers_written++;
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);

    free(file_path);

    if(numbers_written  == shape[0] * shape[1]){
        return numbers_written;
    }
    return -1;

}

int** mat_mul(int** A, int**B, int* A_shape, int* B_shape){

    int i, j, k;

    int** result = (int**) malloc(sizeof(int*) * A_shape[0]);

    if(result == NULL){
        printf("\nMemory allocation failed!\n");
        return NULL;
    }

    for ( i = 0; i < A_shape[0]; i++) {
        result[i] = (int*) malloc(sizeof(int) * B_shape[1]);

        if(result[i] == NULL){
            printf("\nMemory allocation failed!\n");
            return NULL;
        }
        for ( j = 0; j < B_shape[1]; j++) {
            result[i][j] = 0;

            // dot product between two vectors
            for ( k = 0; k < B_shape[0]; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }

        }

    }

    return result;

}

int encrypt_file(char* plain_text_path, char* output_dir, char* config_dir, char* enc_filename){

    int enc_matrix_shape[2];
    int** coder_matrix;
    int* mapping_arr;
    int** msg_matrix;
    int* msg_shape;
    int** enc_matrix;
    void** data;
    int* coder_matrix_shape;
    int saved = 0;

    data = load_matrix(config_dir, CODER_FILENAME);
    coder_matrix = (int**) ( data  [0]);
    coder_matrix_shape = (int*) (data [1]);


    mapping_arr = load_arr(config_dir, MAPPINGS_FILENAME);

    data = txt_to_num_matrix(plain_text_path, mapping_arr, coder_matrix_shape[1]);

    msg_matrix = (int**) data[0];

    msg_shape = (int*) data[1];


    enc_matrix = mat_mul(coder_matrix, msg_matrix, coder_matrix_shape, msg_shape);

    enc_matrix_shape[0] = coder_matrix_shape[0];
    enc_matrix_shape[1] = msg_shape[1];


    printf("\nEncripted text: \n\n");
    for(int i = 0; i< enc_matrix_shape[0]; i++){
        for(int j = 0; j< enc_matrix_shape[1]; j++){
            printf("%d ", enc_matrix[i][j]);
        }
        printf("\n");
    }


    saved = matrix_to_txt(enc_matrix, enc_matrix_shape, output_dir, enc_filename);


    if(saved == -1 ){
        printf("\nThere was an error when saving the encrypted file");
    }


    return saved;
}

int main(int argc, char* argv[])
{
    int encrypted = 0;
    char* config_dir;
    char* plain_text_path;
    char* enc_filename;
    char* output_dir = "./outputs";

    if(argc > 2){
        config_dir = argv[1];
        output_dir = argv[2];
    }
    else{
        printf("Please provide the path (using forward slashes)\nto the config directory with the files needed for the encryption: ");
        config_dir = read_line();
    }

    printf("Path to text file to be encrypted: ");
    plain_text_path = read_line();

    fflush(stdin);
    printf("Name(without file extension) you want\nfor the coded file: ");
    enc_filename = read_line();
    strcat(enc_filename, ".txt");

    encrypted = encrypt_file(plain_text_path, output_dir, config_dir, enc_filename);

    if(encrypted != -1){
        printf("\nThe file was encrypted with success.\n\n");
        return 0;
    }

    printf("\nAn error occurred, please try again.");

    return -1;
}
