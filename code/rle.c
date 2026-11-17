// Run-length encoding
// Each run is written as a one-digit count (1-9) followed by the symbol.
// Runs longer than 9 get split, so 12 A's -> 9A3A
//
// compile: gcc -Wall -o rle rle.c
// run:     ./rle

#include <stdio.h>

void encode(char text[]) {
    int count = 1;

    printf("%s -> ", text);

    for (int i = 0; text[i] != '\0'; i++) {
        // stop at 9 so the count is always one digit
        if (text[i] == text[i + 1] && count < 9) {
            count++;
        } else {
            printf("%d%c", count, text[i]);
            count = 1;
        }
    }

    printf("\n");
}

void decode(char code[]) {
    printf("%s -> ", code);

    for (int i = 0; code[i] != '\0'; i += 2) {
        int count = code[i] - '0';
        char letter = code[i + 1];

        // make sure the count is 1-9 and there's a letter after it
        if (count < 1 || count > 9 || letter == '\0') {
            printf("invalid input\n");
            return;
        }

        for (int j = 0; j < count; j++) {
            printf("%c", letter);
        }
    }

    printf("\n");
}

int main(void) {
    printf("Encoding:\n");
    encode("WWWWWBBBWWWW");   // output is half the size
    encode("ABCD");           // worst case, output doubles
    encode("AAAAAAAAAAAA");   // 12 A's get split into 9A3A
    encode("12");             // digits work because the count is one digit

    printf("\nDecoding:\n");
    decode("5W3B4W");
    decode("1A1B1C1D");
    decode("9A3A");
    decode("1112");           // gets back "12"
    decode("A5");             // bad input

    return 0;
}
