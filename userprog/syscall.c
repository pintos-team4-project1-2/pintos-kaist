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
void syscall_handler (struct intr_frame *);
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
	if (is_kernel_vaddr(addr))
		exit(-1);
	
	if (pml4_get_page(NULL, addr) == NULL)
		exit(-1);
}

/* The main system call interface */
/* The x86-64 convention for function return values is to place them in the RAX register. 
 * System calls that return a value can do so by modifying the rax member of struct intr_frame. */
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	
	if (!is_user_vaddr(f->rsp))
		return;

	// uint64_t sys_num = f->R.rax;
	// uint64_t arg1 = f->R.rdi;
	// uint64_t arg2 = f->R.rsi;
	// uint64_t arg3 = f->R.rdx;
	// uint64_t arg4 = f->R.r10;
	// uint64_t arg5 = f->R.r8;
	// uint64_t arg6 = f->R.r9;

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
	struct thread *curr = thread_current ();
	curr->exit_code = status;
	printf("%s: exit(%d)\n", curr->name, curr->exit_code);
	thread_exit ();
}

bool create (const char *file, unsigned initial_size) {
	// if (file == NULL)
	// 	exit(-1);
	check_address(file);
	return filesys_create (file, initial_size);
}

bool remove (const char *file){
	return filesys_remove (file);
}

int open (const char *file) {
	lock_acquire (&filesys_lock);
	struct thread *curr = thread_current ();
	int i;

	if (file != NULL && curr->next_fd < FILE_NUM) {
		struct file *new_file = filesys_open (file);
		if (new_file != NULL) {
			curr->fdt[curr->next_fd] = new_file;
			lock_release (&filesys_lock);
			return curr->next_fd++;	
		}
	}

	lock_release (&filesys_lock);
	return -1;
}

void close (int fd) {
	process_close_file(fd);
}

int file_size (int fd) {
	struct thread *curr = thread_current ();
	off_t file_size = file_length (curr->fdt[fd]);

	if (file_size == 0 || file_size == NULL) {
		return -1;
	}

	return file_size;
}

int read (int fd, void *buffer, unsigned length) {
	struct thread *curr = thread_current ();
	int bytes_read;

	if (fd == 0) {
		bytes_read = input_getc ();
		return bytes_read;
	}
	else if (fd >= 2) {
		lock_acquire (&filesys_lock);
		bytes_read = file_read (curr->fdt[fd], buffer, length);
		lock_release (&filesys_lock);

		return bytes_read;
	}
	return -1;
}

int write (int fd, const void *buffer, unsigned length) {
	// if (!(is_user_vaddr))
	if (fd == 1){
		// lock_acquire(&filesys_lock);
		putbuf (buffer, length);
		// lock_release(&filesys_lock);
		return sizeof(buffer);
		// return sizeof(buffer) > length ? sizeof(buffer) : length;
	} 
	else if (fd > 1) {
		lock_acquire(&filesys_lock);
		int bytes_written = file_write (thread_current ()->fdt[fd], buffer, length);
		lock_release(&filesys_lock);
		return bytes_written;
		// return bytes_written > length ? bytes_written : length;
	}
	return -1;
}


void seek (int fd, unsigned position) {
	return file_seek (thread_current ()->fdt[fd], position);
}


unsigned tell (int fd) {
	return file_tell (thread_current ()->fdt[fd]);
}


pid_t fork (const char *thread_name, struct intr_frame *f) {
	int pid = process_fork (thread_name, f);
	return pid;
}


int exec (const char *file) {
	struct thread *curr = thread_current ();

	if (process_exec(file) == -1){
		return -1;
	}
	exit(curr->exit_code);
}

int wait (pid_t pid) {
	// printf("pid %d %p\n", pid, &pid);
	return process_wait (pid);
}