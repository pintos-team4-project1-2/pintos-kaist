#include <stdio.h>
#include <stdbool.h>

#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

typedef int pid_t;

struct lock filesys_lock;

void syscall_init (void);

void check_address (void *addr);

void halt(void);
void exit(int status);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
// pid_t fork (const char *thread_name, struct intr_frame *f);
int exec (const char *file);
int wait (pid_t pid);
int open (const char *file);
int file_size (int fd);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);


#endif /* userprog/syscall.h */
