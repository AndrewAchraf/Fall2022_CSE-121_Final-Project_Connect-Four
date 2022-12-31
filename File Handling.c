#include <stdio.h>
#include <string.h>

int main(){
    FILE* fptr;

    /*fptr = fopen("Test File.txt", "w");

    fputs("C is garbage\n", fptr);
    fputs("I hate programming in C", fptr);

    fclose(fptr);*/

    /*fptr = fopen("Test File.txt", "r");

    char text[100];

    while( fgets(text, 100, fptr) ){
        printf("%s", text);
    }

    fclose(fptr);*/

    char string[100];
    char string2[100];
    strcpy(string, "Heolloo file !");

    fptr = fopen("Test File.bin", "wb");

    fwrite(string, sizeof(string), 1, fptr);

    fclose(fptr);

    fptr = fopen("Test File.bin", "rb");

    fread(string2, sizeof(string2), 1, fptr);

    fclose(fptr);

    printf("%s", string2);

    return 0;
}
