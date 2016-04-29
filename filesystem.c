/* This file contains the definition of the functions that must be implemented
 * to allow the user access to the file system and the files.
 */

#include "include/filesystem.h"
#include <stdio.h>
#include <string.h>

/* Types of structs */
typedef struct tag {
	char name [32];
	long files;

} tag;

typedef struct inode {
	char name [64];
	short size;
	short position;
	char open;
	char tags [3];

} inode;

typedef struct sblock {
	struct tag tagsMap [30];
	unsigned char numberINodes;
	char firstDataBlock;
	char maximumFiles;
	char mounted;

} sblock;

/* Global variables */
sblock superBlock;
inode inodes[50];

int sblockSize = sizeof(superBlock);
int inodesSize = sizeof(inode) * 50;
int i, j, k;

/* PRIVATE FUNCTION TO BE ABLE OF WORKING WITH THE FILES BITMAP */
long power (int base, int exponent){

	long result = 1;
	for (k = 0 ; k < exponent ; k++){
		result = result * base;
	}

	return result;
}


/***************************/
/* File system management. */
/***************************/

/* Formats a device */
int mkFS (int maxNumFiles, long deviceSize) {

	// Checking if the number of maximum files is correct (1-50)
	if (maxNumFiles < 1 || maxNumFiles > 50){
		return -1;
	}

	// Checking if the size of the device is correct (320KB - 500KB)
	if (deviceSize < 327680 || deviceSize > 512000){
		return -1;
	}

	// Initializing all the superblock structures to their default values
	for (i = 0; i < 30 ; i++){
		strncpy(superBlock.tagsMap[i].name, "FREE", 32);
		superBlock.tagsMap[i].files = 0;
	}

	superBlock.numberINodes = 0;
	superBlock.firstDataBlock = 2;
	superBlock.maximumFiles = maxNumFiles;
	superBlock.mounted = 0;

	// Initializing all the inodes structures to their default values
	for (i = 0; i < 50 ; i++){
		strncpy(inodes[i].name, "", 64);
		inodes[i].size = 0;
		inodes[i].position = 0;
		inodes[i].open = 0;
		inodes[i].tags[0] = -1;
		inodes[i].tags[1] = -1;
		inodes[i].tags[2] = -1;
	}


	char block[BLOCK_SIZE];
	memset(block, '0', BLOCK_SIZE);
	memcpy(block, &superBlock, sblockSize);

	// Writting the superblock information into the first block of the disk
	if (bwrite(DEVICE_IMAGE, 0, block) == -1){
		return -1;
	}

	memset(block, '0', BLOCK_SIZE);
	memcpy(block, inodes, inodesSize);

	// Writting the inodes information into the second block of the disk
	if (bwrite(DEVICE_IMAGE, 1, block) == -1){
		return -1;
	}

	return 0;
}


/* Mounts a file system from the device deviceName */
int mountFS () {

	char block [BLOCK_SIZE];

	// Checking if reading the superblock produces any error
	if (bread(DEVICE_IMAGE, 0, block) == -1){
		return -1;
	}

	memcpy(&superBlock, block, sblockSize);

	// Checking if reading the i-nodes produces any error
	if (bread(DEVICE_IMAGE, 1, block) == -1){
		return -1;
	}

	memcpy(inodes, block, inodesSize);
	superBlock.mounted = 1;
	return 0;
}


/* Unmount file system */
int umountFS () {

	// Checking if there are open files
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (inodes[i].open == 1){
			return -1;
		}
	}

	char block[BLOCK_SIZE];
	memset(block, '0', BLOCK_SIZE);
	memcpy(block, &superBlock, sblockSize);

	// Writting the superBlock into the first block
	if (bwrite(DEVICE_IMAGE, 0, block) == -1){
		return -1;
	}

	memset(block, '0', BLOCK_SIZE);
	memcpy(block, &inodes, inodesSize);

	// Writting the array of i-nodes into the second block
	if (bwrite(DEVICE_IMAGE, 1, block) == -1){
		return -1;
	}

	superBlock.mounted = 0;
	return 0;
}


/*******************/
/* File read/write */
/*******************/

/* Creates a new file, if it doesn't exist */
int creatFS(char *fileName) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if there is enough space for another file
	if (superBlock.numberINodes == superBlock.maximumFiles){
		return -1;
	}

	// Checking if the file name has the proper length
	if (strlen(fileName) < 1 || strlen(fileName) > 64){
		return -1;
	}

	// Checking if there is another i-node with the same name
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){
			return 1;
		}
	}

	// Updating the information of the superBlock and the i-node
	strncpy(inodes[superBlock.numberINodes].name, fileName, 64);
	superBlock.numberINodes = superBlock.numberINodes + 1;
	return 0;
}


/* Opens an existing file */
int openFS (char *fileName) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -2;
	}

	// Checking if the file name has the proper length
	if (strlen(fileName) < 1 || strlen(fileName) > 64){
		return -2;
	}

	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){

			// If the file was already open, return its file descriptor without reseting the pointer
			if (inodes[i].open == 1){
				return (i + superBlock.firstDataBlock);
			}

			inodes[i].open = 1;
			inodes[i].position = 0;
			return (i + superBlock.firstDataBlock);
		}
	}

	return -1;
}


/* Closes a file */
int closeFS (int fileDescriptor) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file descriptor is a valid number
	if (fileDescriptor > superBlock.numberINodes + superBlock.firstDataBlock){
		return -1;
	}

	// Obtaining data block "offset"
	int index = fileDescriptor - superBlock.firstDataBlock;

	inodes[index].open = 0;
	return 0;
}


/* Reads a number of bytes from a file and stores them in a buffer */
int readFS (int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Obtaining data block "offset" and file offset
	int index = fileDescriptor - superBlock.firstDataBlock;
	int offset = inodes[index].position;

	// Checking if the number of bytes is valid
	if (numBytes < 0){
		return -1;
	}

	// Checking if the file is open
	if (inodes[index].open == 0){
		return -1;
	}

	// Checking if there is no byte left to be read
	if (offset == inodes[index].size){
		return 0;
	}

	// Reading the indicated block from the disk
	char block[BLOCK_SIZE];
	if (bread(DEVICE_IMAGE, fileDescriptor, block) == -1){
		return -1;
	}

	// If the number of bytes to read is higher than the number of remaining ones
	if (offset + numBytes > inodes[index].size){
			memcpy(buffer, &(block[offset]), inodes[index].size - offset);
			inodes[index].position = inodes[index].size;
			return inodes[index].size - offset;
	}

	// If it is the common behaviour
	memcpy(buffer, &(block[offset]), numBytes);
	inodes[index].position = offset + numBytes;
	return numBytes;
}


/* Reads number of bytes from a buffer and writes them in a file */
int writeFS (int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Obtaining data block "offset" and file offset
	int index = fileDescriptor - superBlock.firstDataBlock;
	int offset = inodes[index].position;

	// Checking if the number of bytes is valid
	if (numBytes < 0 || (offset + numBytes) > MAX_FILE_SIZE){
		return -1;
	}

	// Checking if the file is open
	if (inodes[index].open == 0){
		return -1;
	}

	// Checking if there is no space left to write
	if (offset == MAX_FILE_SIZE){
		return 0;
	}

	// Reading the indicated block from the disk
	char block[BLOCK_SIZE];
	if (bread(DEVICE_IMAGE, fileDescriptor, block) == -1){
		return -1;
	}

	// Writting data into the disk
	memcpy(&(block[offset]), buffer, numBytes);
	if (bwrite(DEVICE_IMAGE, fileDescriptor, block) == -1){
		return -1;
	}

	// Updating the pointer and the size of the file
	inodes[index].position = offset + numBytes;
	if (offset + numBytes > inodes[index].size){
		inodes[index].size = offset + numBytes;
	}

	return numBytes;
}


/* Repositions the pointer of a file */
int lseekFS (int fileDescriptor, long offset, int whence) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Obtaining data block "offset"
	int index = fileDescriptor - superBlock.firstDataBlock;

	// Checking if the file is open
	if (inodes[index].open == 0){
		return -1;
	}

	// Taking different actions depending on the "whence" value
	if (whence == FS_SEEK_SET && offset >= 0 && offset <= inodes[index].size){
		inodes[index].position = offset;
	}
	else if (whence == FS_SEEK_BEGIN){
		inodes[index].position = 0;
	}
	else if (whence == FS_SEEK_END){
		inodes[index].position = inodes[index].size;
	}
	else {
		return -1;
	}

	return inodes[index].position;
}


/**********************/
/* Version management */
/**********************/

/* Tags a file with the given tag name */
int tagFS (char *fileName, char *tagName) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file name has the proper length
	if (strlen(fileName) < 1 || strlen(fileName) > 64){
		return -1;
	}

	// Checking if the tag name has the proper length
	if (strlen(tagName) < 1 || strlen(tagName) > 32){
		return -1;
	}

	// Checking if the file is already in our file system
	int numINode = -1;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){

			// If the file exists but it is open: error
			if (inodes[i].open == 1){
				return -1;
			}

			numINode = i;
		}
	}

	if (numINode = -1){
		return -1;
	}

	// If there is already a tag with that name: its files are updated
	int found = 0, tagSpace = -1, tagged = 0;
	for (i = 0 ; i < 30 ; i++){

		if (strcmp(superBlock.tagsMap[i].name, tagName) == 0){
			found = 1;

			// Traversing the file tags looking for a space or a match between names
			for (j = 0 ; j < 3 ; j++){
				if (inodes[numINode].tags[j] == -1){
					tagSpace = j;
				}
				if (inodes[numINode].tags[j] == i){
					tagged = 1;
				}
			}

			if (tagged == 1){
				return 1;
			}

			else if (tagSpace == -1){
				return -1;
			}

			inodes[numINode].tags[tagSpace] = i;
			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files + power(2, numINode);

			// Break the loop, there will be only 1 tag with that name
			break;
		}
	}

	// If there is not any tag with that name: it is created
	if (found == 0){

		for (i = 0 ; i < 30 ; i++){
			if (strcmp(superBlock.tagsMap[i].name, "FREE") == 0 && superBlock.tagsMap[i].files == 0){

				// Traversing the file tags looking for a space or a match between names
				for (j = 0 ; j < 3 ; j++){
					if (inodes[numINode].tags[j] == -1){
						tagSpace = j;
					}
				}

				if (tagSpace == -1){
					return -1;
				}

				inodes[numINode].tags[tagSpace] = i;
				strncpy(superBlock.tagsMap[i].name, tagName, 32);
				superBlock.tagsMap[i].files = power(2, numINode);

				// Break the loop with one space is enough
				break;
			}
		}

		// If there is not space in the system to create the new tag: error
		if (tagSpace == -1){
			return -1;
		}
	}

	return 0;
}


/* Removes a tag from a file */
int untagFS (char *fileName, char *tagName) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the file name has the proper length
	if (strlen(fileName) < 1 || strlen(fileName) > 64){
		return -1;
	}

	// Checking if the tag name has the proper length
	if (strlen(tagName) < 1 || strlen(tagName) > 32){
		return -1;
	}

	// Checking if the file is already in our file system
	int numINode = -1;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){

			// If the file exists but it is open: error
			if (inodes[i].open == 1){
				return -1;
			}

			numINode = i;
		}
	}

	if (numINode == -1){
		return -1;
	}

	// If there is already a tag with that name: its files are updated
	int systemTag = 0, fileTag = 0;
	for (i = 0 ; i < 30 ; i++){
		if (strcmp(superBlock.tagsMap[i].name, tagName) == 0){
			systemTag = 1;

			// Traversing the file tags looking for the one passed as argument
			for (j = 0 ; j < 3 ; j++){
				if (inodes[numINode].tags[j] == i){
					inodes[numINode].tags[j] = -1;
					fileTag = 1;
				}
			}

			if (fileTag == 0){
				return 1;
			}

			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files - power(2, numINode);
			if (superBlock.tagsMap[i].files == 0){
				strncpy(superBlock.tagsMap[i].name, "FREE", 32);
			}

			// Break the loop, there will be only 1 tag with that name
			break;
		}
	}

	// If there is not a tag in the system with that name: error
	if (systemTag == 0){
		return -1;
	}

	return 0;
}


/* Looks for all files tagged with the tag tagName, and stores them in a list */
int listFS (char *tagName, char **files) {

	// Checking if the file system is mounted
	if (superBlock.mounted == 0){
		return -1;
	}

	// Checking if the tag name has the proper length
	if (strlen(tagName) < 1 || strlen(tagName) > 32){
		return -1;
	}

	// If there is already a tag with that name: take its "file" value
	int found = 0;
	long remain;
	for (i = 0 ; i < 30 ; i++){
		if (strcmp(superBlock.tagsMap[i].name, tagName) == 0){
			remain = superBlock.tagsMap[i].files;
			found = 1;
		}
	}

	if (found == 0){
		return -1;
	}

	// Storing into the "files" array the names of the found files
	int filesIndex = 0, foundFiles = 0;
	for (i = 0 ; remain > 0 ; i++){

		// If that file has the tag: its name is stored and both variables are updated
		if ((remain % 2) == 1){
			files[filesIndex] = inodes[i].name;
			foundFiles = foundFiles + 1;
			filesIndex = filesIndex + 1;
		}

		remain = (long) (remain / 2);
	}

	return foundFiles;
}
