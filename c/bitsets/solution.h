#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t Bitset;

int Task() {
    Bitset deaf_consonants_mask = 0;
    uint64_t one = 1;
    char deaf_consonants[] = "ptkfsh";
    char all_consonants[] = "bcdfghjklmnpqrstvwxyz";
    Bitset all_consonants_mask = 0;
    for (int i = 0; all_consonants[i] != '\0'; ++i) {
        all_consonants_mask |= (one << (all_consonants[i] - 'a'));
    }
    for (int i = 0; deaf_consonants[i] != '\0'; ++i) {
        deaf_consonants_mask |= (one << (deaf_consonants[i] - 'a'));
    }
    int input_char = 0;
    Bitset current_word_consonants = 0;
    bool in_word = false;
    bool word_found = false;
    while ((input_char = getchar()) != EOF) {
        if ((input_char >= 'a' && input_char <= 'z') || (input_char >= 'A' && input_char <= 'Z')) {
            if (!in_word) {
                in_word = true;
                current_word_consonants = 0;
            }
            int lower_char = input_char;
            if (input_char >= 'A' && input_char <= 'Z') {
                lower_char = input_char + ('a' - 'A');
            }
            if ((all_consonants_mask >> (lower_char - 'a')) & one) {
                current_word_consonants |= (one << (lower_char - 'a'));
            }
        } else {
            if (in_word) {
                Bitset non_deaf_consonants = current_word_consonants & (~deaf_consonants_mask);
                if (non_deaf_consonants == 0 && current_word_consonants != 0) {
                    word_found = true;
                }
                in_word = false;
                current_word_consonants = 0;
            }
        }
    }
    if (in_word) {
        Bitset non_deaf_consonants = current_word_consonants & (~deaf_consonants_mask);
        if (non_deaf_consonants == 0 && current_word_consonants != 0) {
            word_found = true;
        }
    }
    if (word_found) {
        printf("True\n");
    } else {
        printf("False\n");
    }
    return 0;
}
