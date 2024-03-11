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
	
	return 0;
}

