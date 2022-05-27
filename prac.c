#include <stdio.h>
#include <string.h>

char s[] = "$bin/ls –l foo bar";

int main(void) {
    char *token, *save_ptr;
    // char *f_copy;
    // strcpy(f_copy, s);
    // token = strtok_r (s, " ", &save_ptr);
    // printf("%s\n", token);
    // printf("%s\n", save_ptr);
    printf("%ld\n",sizeof(token));
    for (token = strtok_r (s, " ", &save_ptr); token != NULL; token = strtok_r (NULL, " ", &save_ptr)) {
        printf("'%s'\n", token);
        // printf("'%ld\n", strlen(token));
        // printf("%p\n", &(*token));
    }
    char *token2, *save_ptr2;
    
    // token = strtok_r (s, " ", &save_ptr);
    // printf("%s\n", token);
    // printf("%s\n", save_ptr);
    for (token2 = strtok_r (s, " ", &save_ptr2); token2 != NULL; token2 = strtok_r (NULL, " ", &save_ptr2)) {
        printf("'%s'\n", token2);
        // printf("'%ld\n", strlen(token2));
        // printf("%p\n", &(*token2));
    }
}