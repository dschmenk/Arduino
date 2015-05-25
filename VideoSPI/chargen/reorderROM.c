#include <stdio.h>

int main(int argc, char **argv)
{
    unsigned char charROM[1024];
    int charIndex, charRow;
    
    for (charIndex = 0; charIndex < 1024; charIndex += 8)
    {
        if (scanf("%hhi,%hhi,%hhi,%hhi,%hhi,%hhi,%hhi,%hhi,\n", &charROM[charIndex], &charROM[charIndex+1], &charROM[charIndex+2], &charROM[charIndex+3], &charROM[charIndex+4], &charROM[charIndex+5], &charROM[charIndex+6], &charROM[charIndex+7]) != 8)
        {
            fprintf(stderr, "Error reading character ROM at row %i\n", charIndex / 8);
            return(1);
        }
    }
    for (charRow = 0; charRow < 8; charRow++)
    {
        for (charIndex = 0; charIndex < 128; charIndex++)
        {
            printf("0x%02X,", charROM[charIndex * 8 + charRow]);
        }
        printf("\n");
    }
}
        
