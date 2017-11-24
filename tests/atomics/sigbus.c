#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main (int argc, char** argv) {
       	int fd = open("test", O_WRONLY | O_CREAT, 0644);
       	write(fd, "abcd\n", 5);
       	close(fd);

       	fd = open("test", O_RDONLY);
       	char* ptr = (char*)mmap(0, 102400, PROT_READ, MAP_SHARED, fd, 0);
       	char z = ptr[4097];
       	write(1, &z, 1);
       	return 0;
}
