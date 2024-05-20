#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define HELLO_WORLD_EXE ".\\modules\\HelloWorld\\bin\\debug\\HelloWorld.exe"
    #define CODER_EXE ".\\modules\\Coder\\bin\\debug\\Coder.exe"
    #define DECODER_EXE ".\\modules\\Decoder\\bin\\debug\\Decoder.exe"
    #define CONFIGURER_EXE ".\\modules\\Coder-configurer\\bin\\Debug\\Coder-configurer.exe"
#else
    #define HELLO_WORLD_EXE "./modules/HelloWorld/bin/debug/HelloWorld"
    #define CODER_EXE "./modules/Coder/bin/debug/Coder"
    #define DECODER_EXE "./modules/Decoder/bin/debug/Decoder"
    #define CONFIGURER_EXE "./modules/Coder-configurer/bin/Debug/Coder-configurer"
#endif




#define CODED_DIR "./coded"
#define DECODED_DIR "./decoded"
#define CONFIG_DIR "./config"


int main()
{

    char command[100];

    int option;

    while(option != 5){

        system("cls");

        printf("Welcome to Linear Algebra Cipher!\n");

        printf("1 - Configure the encryption\n");
        printf("2 - Encrypt a text file\n");
        printf("3 - Decrypt a text file\n");
        printf("4 - Receive a super cool message\n");
        printf("5 - Quit\n");

        printf("Choose an option: ");

        fflush(stdin);
        scanf("%d", &option);

        switch (option){



            case 1:
                sprintf(command, "\"%s %s\" ", CONFIGURER_EXE, CONFIG_DIR);
                system(command);
                break;
            case 2:
                sprintf(command, "\"%s %s %s\" ", CODER_EXE, CONFIG_DIR, CODED_DIR);
                system(command);
                break;
            case 3:
                sprintf(command, "\"%s %s %s\" ", DECODER_EXE, CONFIG_DIR, DECODED_DIR);
                system(command);
                break;
            case 4:
                system(HELLO_WORLD_EXE);
                break;
            default:
                printf("Invalid option!");
                break;
        }

        fflush(stdin);
        printf("\nPress any key to continue\n");
        getchar();

    }

    return 0;
}
