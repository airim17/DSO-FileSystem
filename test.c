#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/filesystem.h"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

int main() {
	int ret, i, fd;
	char buffer1[BLOCK_SIZE];
	char buffer2[BLOCK_SIZE];
	char buffer3[BLOCK_SIZE];

	fprintf(stdout, "%s", "TEST mkFS\n");

	ret = mkFS(50, 400800);
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST mountFS\n");

	ret = mountFS();
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST creatFS\n");

	ret = creatFS("test.txt");
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST openFS + closeFS\n");

	fd = openFS("test.txt");
	if(fd < 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at openFS\n");
		return -1;
	}
	ret = closeFS(fd);
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at closeFS\n");
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST openFS + closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS\n");

	fd = openFS("test.txt");
	if(fd < 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at openFS\n");
		return -1;
	}

	memset(buffer2, 't', BLOCK_SIZE);
	ret = writeFS(fd, buffer2, BLOCK_SIZE);
	if(ret != BLOCK_SIZE) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at writeFS\n");
		return -1;
	}

	ret = readFS(fd, buffer1, BLOCK_SIZE);
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at readFS\n");
		return -1;
	}
	ret = lseekFS(fd, 0, FS_SEEK_SET);
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at lseekFS\n");
		return -1;
	}

	ret = readFS(fd, buffer3, BLOCK_SIZE);
	if(ret != BLOCK_SIZE) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at readFS #2\n");
		return -1;
	}
	for(i = 0; i < BLOCK_SIZE; ++i) {
		if(buffer3[i] != 't') {
			fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at readFS #2\n");
			return -1;
		}
	}
	ret = closeFS(fd);
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at closeFS\n");
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST openFS + readFS + writeFS + lseekFS + closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST tagFS + listFS + untagFS\n");

	ret = tagFS("test.txt", "sample");
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at tagFS\n");
		return -1;
	}
	char **files = (char**) malloc(50*sizeof(char*));
	for(i = 0; i < 50; ++i) {
		files[i] = (char*) malloc(64*sizeof(char));
	}
	ret = listFS("sample", files);
	if(ret != 1 || strcmp(files[0], "test.txt") != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at listFS\n");
		return -1;
	}
	ret = untagFS("test.txt", "sample");
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at untagFS\n");
		return -1;
	}
	ret = listFS("sample", files);
	if(ret != -1) {
		fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at listFS #2\n");
		return -1;
	}
	fprintf(stdout, "%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	fprintf(stdout, "%s", "TEST umountFS\n");


	ret = umountFS();
	if(ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST umountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s", "TEST umountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	return 0;
}
