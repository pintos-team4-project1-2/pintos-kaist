#include <stdio.h>
#include <string.h>

char s[] = "$bin/ls -l foo bar";

int main(void) {
    // char *token, *save_ptr;
    // // char *f_copy;
    // // strcpy(f_copy, s);
    // // token = strtok_r (s, " ", &save_ptr);
    // // printf("%s\n", token);
    // // printf("%s\n", save_ptr);
    // char *arr[128];
    // int i = 0, argc = 0;
    // for (token = strtok_r (s, " ", &save_ptr); token != NULL; token = strtok_r (NULL, " ", &save_ptr)) {
    //     arr[argc++] = token;
    //     printf("'%s'\n", token);
    //     // printf("'%ld\n", strlen(token));
    //     // printf("%p\n", token);
    // }

    // for (i = argc-1; i >= 0; i--) {
    //     // printf("%s\n", arr[i]);
    //     printf("1 %s\n", arr[i]);
    //     printf("%li\n", strlen(arr[i])+1);
    //     // printf("2 %p\n", arr[i]-sizeof(void *));
    // }

	char *arg[128];
	int count = 0;
    char *token, *save_ptr;
    int i = 0, argc = 0;
    for (token = strtok_r (s, " ", &save_ptr); token != NULL; token = strtok_r (NULL, " ", &save_ptr)) {
        arg[count++] = token;
        printf("'%s'\n", token);
        printf("'%p'\n", *token);
        printf("'%p'\n", token);
    }


}