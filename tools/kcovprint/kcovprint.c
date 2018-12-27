#include <sys/ioctl.h> 
#include <sys/kcov.h> 
#include <sys/mman.h> 
 
#include <err.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

static void __attribute__ ((constructor)) lib_init(void);
static void __attribute__ ((destructor)) lib_finish(void);

int fd; 
unsigned long *cover; 
unsigned long size = 1024; 

static void lib_init(void) {
	fd = open("/dev/kcov", O_RDWR); 
	if (fd == -1) 
		err(1, "open"); 
 
	if (ioctl(fd, KIOSETBUFSIZE, &size) == -1) 
		err(1, "ioctl: KIOSETBUFSIZE"); 
	cover = mmap(NULL, size * sizeof(unsigned long), 
	    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
	if (cover == MAP_FAILED) 
		err(1, "mmap"); 
 
	if (ioctl(fd, KIOENABLE) == -1) 
		err(1, "ioctl: KIOENABLE"); 
	read(-1, NULL, 0); 

	return;
}

static void lib_finish(void) {
	unsigned long i; 
 
	if (ioctl(fd, KIODISABLE) == -1) 
		err(1, "ioctl: KIODISABLE"); 
 
	for (i = 0; i < cover[0]; i++) 
		printf("%p\n", (void *)cover[i + 1]); 
 
	if (munmap(cover, size * sizeof(unsigned long)) == -1) 
		err(1, "munmap"); 
	close(fd); 

	return;
}
