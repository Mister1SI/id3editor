#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef enum {
	TITLE,
	ARTIST,
	ALBUM,
	YEAR,
	COMMENT,
	UNKNOWN
} EditType;

int edit_v1(char* pdata_start, EditType et, char* new_field);
void strip_newlines(char* text, int bytes);

int main(int argc, char** argv) {
	char* filename;
	char field;
	int edit;
	if(argc == 1) {
		help();
		return 0;
	} else if(argc == 2) {
		filename = argv[1];
		edit = 0;
	} else if(argc == 3) {
		edit = 1;
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
	printf("Filesize: %ldB\n", filesize);

	// Open file
	int fd = open(filename, O_RDWR);
	if(fd == -1) {
		perror("Failed to open file");
		return 3;
	}
	
	// Map memory
	char* pfile = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pfile == MAP_FAILED) {
		perror("Failed to open file");
		return 4;
	}
	
	// Get the pointer to the start of the v1 data tag
	char* pv1_start = pfile + (filesize - 128);

	// Check if there's a v1 tag
	if(0 == strncmp(pv1_start, "TAG", 3)) {
		puts("ID3v1 Tag info:");
		write(1, "\tTitle:\t\t", 10);
		write(1, pv1_start + 3, 30);
		write(1, "\n\tArtist:\t\t", 11);
		write(1, pv1_start + 33, 30);
		write(1, "\n\tAlbum:\t\t", 10);
		write(1, pv1_start + 63, 30);
		write(1, "\n\tYear:\t\t", 9);
		write(1, pv1_start + 93, 4);
		write(1, "\n\tComment:\t", 12);
		write(1, pv1_start + 97, 30);
		puts("");

		// If edit mode is enabled, process that
		if(edit) {
			puts("Editing field");
			char* buffer = malloc(256);
			memset(buffer, 0, 256);
			switch(field) {
				case 't':
					printf("Enter new title: ");
					fflush(stdout);
					read(0, buffer, 256);
					strip_newlines(buffer, 30);
					edit_v1(pv1_start, TITLE, buffer);
					break;
				case 'a':
					printf("Enter new artist: ");
					fflush(stdout);
					read(0, buffer, 256);
					strip_newlines(buffer, 30);
					edit_v1(pv1_start, ARTIST, buffer);
					break;
				case 'l':
					printf("Enter new album: ");
					fflush(stdout);
					read(0, buffer, 256);
					strip_newlines(buffer, 30);
					edit_v1(pv1_start, ALBUM, buffer);
					break;
				case 'y':
					printf("Enter new year: ");
					fflush(stdout);
					read(0, buffer, 256);
					strip_newlines(buffer, 4);
					edit_v1(pv1_start, YEAR, buffer);
					break;
				case 'c':
					printf("Enter new comment: ");
					fflush(stdout);
					read(0, buffer, 256);
					strip_newlines(buffer, 30);
					edit_v1(pv1_start, COMMENT, buffer);
					break;
			}

		}

	} else {
		puts("Did not find a v1 tag.");
	}
	
	munmap(pfile, filesize);
	return 0;
}

int edit_v1(char* pdata_start, EditType et, char* new_field) {
	switch(et) {
		case TITLE:
			memcpy(pdata_start + 3, new_field, 30);
			return 0;
		case ARTIST:
			memcpy(pdata_start + 33, new_field, 30);
			return 0;
		case ALBUM:
			memcpy(pdata_start + 63, new_field, 30);
			return 0;
		case YEAR:
			memcpy(pdata_start + 93, new_field, 4);
			return 0;
		case COMMENT:
			memcpy(pdata_start + 97, new_field, 30);
			return 0;
		case UNKNOWN:
			return -1;
	}
}


void strip_newlines(char* text, int bytes) {
	for(int i = 0; i < bytes; i++) {
		if(text[i] == '\n')
			text[i] = 0;
	}
}

void help() {
	puts("id3editor - ID3v1 Reader & Editor\n"
	"usage:\n"
	"id3editor				- Display help menu. \n"
	"id3editor <filename>			- Display ID3v1 data.\n"
	"id3editor <filename> [t a l y c]	- Edit title, artist, album, year, or comment, respectively.");
}

