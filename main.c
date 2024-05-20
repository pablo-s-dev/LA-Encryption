#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define HELLO_WORLD_EXE "..\\modules\\HelloWorld\\Debug\\HelloWorld.exe"
    #define CODER_EXE "..\\modules\\Coder\\Debug\\Coder.exe"
    #define DECODER_EXE "..\\modules\\Decoder\\Debug\\Decoder.exe"
    #define CONFIGURER_EXE "..\\modules\\Coder-configurer\\Debug\\Coder-configurer.exe"
    #define CLEAR_COMMAND "cls"
#else
    #define HELLO_WORLD_EXE "./modules/HelloWorld/HelloWorld"
    #define CODER_EXE "./modules/Coder/Coder"
    #define DECODER_EXE "./modules/Decoder/Decoder"
    #define CONFIGURER_EXE "./modules/Coder-configurer/Coder-configurer"
    #define CLEAR_COMMAND "clear"
#endif

#ifdef _WIN32
    #define CODED_DIR "..\\..\\coded"
    #define DECODED_DIR "..\\..\\decoded"
    #define CONFIG_DIR "..\\..\\config"

#else
    #define CODED_DIR "../coded"
    #define DECODED_DIR "../decoded"
    #define CONFIG_DIR "../config"

#endif



int main()
{

    char command[100];

    int option = -1;

    while(option != 5){



        printf("Welcome to Linear Algebra Cipher!\n");

        printf("1 - Configure the encryption\n");
        printf("2 - Encrypt a text file\n");
        printf("3 - Decrypt a text file\n");
        printf("4 - Receive a super cool message\n");
        printf("5 - Quit\n");

        printf("Choose an option: ");

        fflush(stdin);
        scanf("%d", &option);

        printf("\n");

        switch (option){



            case 1:
                sprintf(command, "%s \"%s\"", CONFIGURER_EXE, CONFIG_DIR);
                system(command);
                break;
            case 2:
                sprintf(command, "%s \"%s\" \"%s\"", CODER_EXE, CONFIG_DIR, CODED_DIR);
                system(command);
                break;
            case 3:
                sprintf(command, "%s \"%s\" \"%s\"", DECODER_EXE, CONFIG_DIR, DECODED_DIR);
                system(command);
                break;
            case 4:
                system(HELLO_WORLD_EXE);
                break;
            case 5:
                printf("\n\nGood bye!");
                break;
            default:
                printf("Invalid option!");
                break;
        }
        getchar();
        printf("\nPress any key to continue\n");
        getchar();

    }

    return 0;
}
