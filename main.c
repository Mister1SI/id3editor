#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

void help();

// Usage:
// id3editor <filename> [field: t a l y c]
//
// Error codes:
// 0:	Success
// 1:	Incorrect argument amount
// 2:	stat() failed
// 3:	open() failed
// 4:	mmap() failed
//

int main(int argc, char** argv) {
	puts("Hello");
	char* filename;
	char field;
	if(argc == 1) {
		help();
		return 0;
	} else if(argc == 3) {
		filename = argv[1];
		field = argv[2][0];
	} else {
		puts("Incorrect argument amount");
		help();
		return 1;
	}
	
	// Get file size
	struct stat st;
	if(stat(filename, &st) == -1) {
		perror("Failed to process file stats for given file");
		return 2;
	}
	long filesize = st.st_size;
	printf("Filesize: %ldB", filesize);

	// Open file
	int fd = open(filename, O_RDWR);
	if(fd == -1) {
		perror("Failed to open file");
		return 3;
	}
	
	// Map memory
	char* pfile;
	pfile = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pfile == MAP_FAILED) {
		perror("Failed to open file");
		return 4;
	}

	return 0;
}

void help() {
	puts("Help");
}

