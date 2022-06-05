#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "threads/synch.h"
#include "userprog/gdt.h"
#include "userprog/process.h"
#include "threads/flags.h"
#include "intrinsic.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include <console.h>

typedef int pid_t;

struct lock filesys_lock;

void syscall_entry (void);
void check_address (void *addr);
void syscall_handler (struct intr_frame *f UNUSED);
void halt(void);
void exit(int status);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
pid_t fork (const char *thread_name, struct intr_frame *f);
int exec (const char *file);
int wait (pid_t pid);
int open (const char *file);
int file_size (int fd);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);


/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);
	
	lock_init(&filesys_lock);
}

/* added */
void 
check_address (void *addr) {
	if (addr == NULL || is_kernel_vaddr(addr) || pml4_get_page(thread_current ()->pml4, addr) == NULL)
		exit(-1);
}

/* The main system call interface */
/* The x86-64 convention for function return values is to place them in the RAX register. 
 * System calls that return a value can do so by modifying the rax member of struct intr_frame. */
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	// printf ("system call!\n");
	switch (f->R.rax) {
		case 0:
			halt ();
			break;
		case 1:
			exit (f->R.rdi);
			break;
		case 2:
			f->R.rax = fork (f->R.rdi, f);
			break;
		case 3:
			f->R.rax = exec (f->R.rdi);
			break;
		case 4:
			f->R.rax = wait (f->R.rdi);
			break;
		case 5:
			f->R.rax = create (f->R.rdi, f->R.rsi);
			break;
		case 6:
			f->R.rax = remove (f->R.rdi);
			break;
		case 7:
			f->R.rax = open (f->R.rdi);
			break;
		case 8:
			f->R.rax = file_size (f->R.rdi);
			break;
		case 9:
			f->R.rax = read (f->R.rdi, f->R.rsi, f->R.rdx);
			break;
		case 10:
			f->R.rax = write (f->R.rdi, f->R.rsi, f->R.rdx);
			break;
		case 11:
			seek (f->R.rdi, f->R.rsi);
			break;
		case 12:
			f->R.rax = tell (f->R.rdi);
			break;
		case 13:
			close (f->R.rdi);
			break;
		default:
			thread_exit ();
			break;
	}
}


void halt(void) {
	power_off ();
}

void exit(int status){
	struct thread *current = thread_current ();
	current->exit_code = status;
	printf("%s: exit(%d)\n", current->name, current->exit_code);
	thread_exit ();
}

bool create (const char *file, unsigned initial_size) {
	check_address(file);
	return filesys_create (file, initial_size);
}

bool remove (const char *file){
	check_address(file);
	return filesys_remove (file);
}

int open (const char *file) {
	check_address(file);
	// struct thread *current = thread_current ();
	// int i;

	// if (current->next_fd < FILE_NUM) {
	// 	lock_acquire (&filesys_lock);
	// 	struct file *new_file = filesys_open (file);

	// 	if (new_file != NULL) {
	// 		current->fdt[current->next_fd] = new_file;
	// 		lock_release (&filesys_lock);
	// 		return current->next_fd++;
	// 	}
	// 	lock_release (&filesys_lock);
	// }

	int i;

	lock_acquire(&filesys_lock);
	struct file *ofile = filesys_open (file);
	struct thread *current = thread_current ();

	if (ofile == NULL) {
		lock_release(&filesys_lock);
		return -1;
	}

	for (i = 3; i < FILE_NUM; i++) {
		if (current->fdt[i] == NULL) {
			current->fdt[i] = ofile;
			lock_release(&filesys_lock);
			current->next_fd = i;
			return current->next_fd;
		}
	}
	lock_release(&filesys_lock);

	return -1;
}

void close (int fd) {
	struct file *ofile = thread_current ()->fdt[fd];
	if (ofile != NULL && fd >= 3) {
		file_close (ofile);
		thread_current ()->fdt[fd] = NULL;
	}
}

int file_size (int fd) {
	struct file *ofile = thread_current ()->fdt[fd];
	
	if (ofile == NULL)
		return 0;

	return file_length (ofile);
}

int read (int fd, void *buffer, unsigned length) {
	check_address(buffer);
	int bytes_read;

	if (fd == 0) {
		lock_acquire (&filesys_lock);
		bytes_read = input_getc ();
		lock_release (&filesys_lock);

		return bytes_read;
	}
	else if (fd >= 2) {
		lock_acquire (&filesys_lock);
		struct file *ofile = thread_current ()->fdt[fd];
		if (ofile != NULL) {
			bytes_read = file_read (ofile, buffer, length);
			lock_release (&filesys_lock);

			return bytes_read;
		}
		lock_release (&filesys_lock);
	}

	return -1;
}

int write (int fd, const void *buffer, unsigned length) {
	check_address(buffer);

	if (fd == 1){
		lock_acquire(&filesys_lock);
		putbuf (buffer, length);
		lock_release(&filesys_lock);

		return sizeof(buffer);
	} 
	else if (fd > 2) {
		lock_acquire(&filesys_lock);
		struct file *ofile = thread_current ()->fdt[fd];
		if (ofile != NULL) {
			int bytes_written = file_write (ofile, buffer, length);
			lock_release(&filesys_lock);

			return bytes_written;
		}

		lock_release(&filesys_lock);
	}

	return -1;
}


void seek (int fd, unsigned position) {
	struct file *ofile = thread_current ()->fdt[fd];
	if (ofile != NULL)
		return file_seek (ofile, position);
}


unsigned tell (int fd) {
	struct file *ofile = thread_current ()->fdt[fd];
	if (ofile != NULL)
		return file_tell (ofile);
}


pid_t fork (const char *thread_name, struct intr_frame *f) {
	check_address (thread_name);
	return process_fork (thread_name, f);
}


int exec (const char *file) {
	check_address (file);
	struct thread *curr = thread_current ();
	if (process_exec(file) == -1){
		return -1;
	}
	exit(curr->exit_code);
}

int wait (pid_t pid) {
	return process_wait (pid);
}