#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

void help();

// Usage:
// id3editor <filename> [field: t a l y c]
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
	
	struct stat st;
	if(stat(filename) == -1) {
		perror("Failed to process file stats for given file");
		return 1;
	}
	long filesize = st.st_size;
	printf("Filesize: %ldB", filesize);

	return 0;
}

