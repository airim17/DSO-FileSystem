/* This file contains the definition of the functions that must be implemented
 * to allow the user access to the file system and the files.
 */

#include "include/filesystem.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Types of structs */
typedef struct tag {
	char name [32];
	unsigned char counter;
	unsigned long files;

} tag;

typedef struct inode {
	char name [64];
	unsigned short filePointer;
	unsigned short size;
	unsigned char open;
	char tags [3];

} inode;

typedef struct sblock {
	struct tag tagsMap [30];
	unsigned char numberINodes;
	unsigned char firstDataBlock;
	unsigned char maximumFiles;

} sblock;

/* Global variables */
sblock superBlock;
inode inodes[50];

int sblockSize = sizeof(superBlock);
int inodesSize = sizeof(inode) * 50;


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
	int i;
	for (i = 0; i < 30 ; i++){
		strncpy(superBlock.tagsMap[i].name, "FREE", 32);
		superBlock.tagsMap[i].counter = 0;
		superBlock.tagsMap[i].files = 0;
	}

	superBlock.numberINodes = 0;
	superBlock.firstDataBlock = ceil(inodesSize / 4096) + 1;
	superBlock.maximumFiles = (unsigned char) maxNumFiles;

	// Initializing all the inodes structures to their default values
	for (i = 0; i < 50 ; i++){
		strncpy(inodes[i].name, "", 64);
		inodes[i].filePointer = 0;
		inodes[i].size = 0;
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
	return 0;
}


/* Unmount file system */
int umountFS () {

	// Checking if there are open files
	int i;
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

	return 0;
}


/*******************/
/* File read/write */
/*******************/

/* Creates a new file, if it doesn't exist */
int creatFS(char *fileName) {

	// Checking if there is enough space for another file
	if (superBlock.numberINodes == superBlock.maximumFiles){
		return -1;
	}

	// Checking if the file name has the proper length
	if (sizeof(fileName) < 1 || sizeof(fileName) > 64){
		return -1;
	}

	// Checking if there is another i-node with the same name
	int i;
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

	// Checking if the file name has the proper length
	if (sizeof(fileName) < 1 || sizeof(fileName) > 64){
		return -1;
	}

	int i;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){
			inodes[i].open = 1;
			inodes[i].filePointer = 0;
			return (i + superBlock.firstDataBlock);
		}
	}

	return -1;
}


/* Closes a file */
int closeFS (int fileDescriptor) {

	// Checking if the file descriptor is a valid number
	if (fileDescriptor > superBlock.numberINodes + superBlock.firstDataBlock){
		return -1;
	}

	if (inodes[fileDescriptor-superBlock.firstDataBlock].open == 1){
		inodes[fileDescriptor-superBlock.firstDataBlock].open = 0;
		return 0;
	}

	return -1;
}


/* Reads a number of bytes from a file and stores them in a buffer */
int readFS (int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Checking if the file is open
	if (inodes[fileDescriptor-superBlock.firstDataBlock].open == 0){
		return -1;
	}

	// Obtaining data block "offset" and file offset
	int index = fileDescriptor - superBlock.firstDataBlock;
	int offset = inodes[index].filePointer;

	// Checking if the number of bytes is valid
	if (numBytes < 0 || numBytes > inodes[index].size){
		return -1;
	}

	// Checking if there is no byte left to be read
	if (inodes[index].filePointer == inodes[index].size){
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
			inodes[index].filePointer = inodes[index].size;
			return inodes[index].size - offset;
	}

	// If it is the common behaviour
	memcpy(buffer, &(block[offset]), numBytes);
	inodes[index].filePointer = offset + numBytes;
	return numBytes;
}


/* Reads number of bytes from a buffer and writes them in a file */
int writeFS (int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Checking if the file is open
	if (inodes[fileDescriptor-1].open == 0){
		return -1;
	}

	// Obtaining data block "offset" and file offset
	int index = fileDescriptor - superBlock.firstDataBlock;
	int offset = inodes[index].filePointer;

	// Checking if the number of bytes is valid
	if (numBytes < 0 || (offset + numBytes) > MAX_FILE_SIZE){
		return -1;
	}

	// Checking if there is no byte left to be read
	if (inodes[index].filePointer == MAX_FILE_SIZE){
		return 0;
	}

	// Writting data into the disk
	char block[BLOCK_SIZE];
	if (bwrite(DEVICE_IMAGE, fileDescriptor, buffer) == -1){
		return -1;
	}

	// Updating the pointer and the size of the file
	memcpy(&(buffer[offset]), block, numBytes);
	inodes[index].filePointer = offset + numBytes;
	if (offset + numBytes > inodes[index].size){
		inodes[index].size = offset + numBytes;
	}

	return numBytes;
}


/* Repositions the pointer of a file */
int lseekFS (int fileDescriptor, long offset, int whence) {

	// Checking if the file descriptor is a valid number
	if (fileDescriptor < superBlock.firstDataBlock || fileDescriptor > (superBlock.numberINodes + superBlock.firstDataBlock)){
		return -1;
	}

	// Obtaining data block "offset"
	int index = fileDescriptor - superBlock.firstDataBlock;

	// Checking if the offset in a valid number
	if (offset < 0 || offset > inodes[index].size){
		return -1;
	}

	// Taking different actions depending on the "whence" value
	if (whence == FS_SEEK_SET){
		inodes[index].filePointer = offset;
	}
	else if (whence == FS_SEEK_BEGIN){
		inodes[index].filePointer = 0;
	}
	else if (whence == FS_SEEK_END){
		inodes[index].filePointer = inodes[index].size;
	}
	else {
		return -1;
	}

	return inodes[index].filePointer;
}


/**********************/
/* Version management */
/**********************/

/* Tags a file with the given tag name */
int tagFS (char *fileName, char *tagName) {

	// Checking if the file name has the proper length
	if (sizeof(fileName) < 1 || sizeof(fileName) > 64){
		return -1;
	}

	// Checking if the tag name has the proper length
	if (sizeof(tagName) < 1 || sizeof(tagName) > 32){
		return -1;
	}

	// Checking if the file is already in our file system
	int i, numINode, fileExist = 0;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){

			// If the file exists but it is open: error
			if (inodes[i].open == 1){
				return -1;
			}

			fileExist = 1;
			numINode = i;
		}
	}

	if (fileExist == 0){
		return -1;
	}

	// If there is already a tag with that name: its counter and files are updated
	int j, found = 0, tagSpace = -1, tagged = 0;
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
			superBlock.tagsMap[i].counter = superBlock.tagsMap[i].counter + 1;
			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files + pow(2, numINode);

			// Break the loop, there will be only 1 tag with that name
			break;
		}
	}

	// If there is not any tag with that name: it is created
	if (found == 0){

		for (i = 0 ; i < 30 ; i++){
			if (strcmp(superBlock.tagsMap[i].name, "FREE") == 0 && superBlock.tagsMap[i].counter == 0){

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
				strncpy(superBlock.tagsMap[i].name, tagName, 32);
				superBlock.tagsMap[i].counter = 1;
				superBlock.tagsMap[i].files = pow(2, numINode);

				// Break the loop with one space is enough
				break;
			}
		}
	}

	return 0;
}


/* Removes a tag from a file */
int untagFS (char *fileName, char *tagName) {

	// Checking if the file name has the proper length
	if (sizeof(fileName) < 1 || sizeof(fileName) > 64){
		return -1;
	}

	// Checking if the tag name has the proper length
	if (sizeof(tagName) < 1 || sizeof(tagName) > 32){
		return -1;
	}

	// Checking if the file is already in our file system
	int i, numINode, fileExist = 0;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (strcmp(inodes[i].name, fileName) == 0){

			// If the file exists but it is open: error
			if (inodes[i].open == 1){
				return -1;
			}

			fileExist = 1;
			numINode = i;
		}
	}

	if (fileExist == 0){
		return -1;
	}

	// If there is already a tag with that name: its counter is decrease
	int j, systemTag = 0, fileTag = 0;
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

			superBlock.tagsMap[i].counter = superBlock.tagsMap[i].counter - 1;
			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files - pow(2, numINode);
			if (superBlock.tagsMap[i].counter == 0){
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

	// Checking if the tag name has the proper length
	if (sizeof(tagName) < 1 || sizeof(tagName) > 32){
		return -1;
	}

	// If there is already a tag with that name: take its "file" value
	int i, remain, found = 0;
	for (i = 0 ; i < 30 ; i++){
		if (strcmp(superBlock.tagsMap[i].name, tagName) == 0){
			remain = superBlock.tagsMap[i].files;
			found = 1;
		}
	}

	if (found == 0){
		return -1;
	}

	// Creating the elements to operate with the files bit map
	char filesMap [superBlock.numberINodes];
	int index = superBlock.numberINodes - 1;

	// Storing in the files bit map which files contain the tag
	for (i = 0 ; remain > 0 ; i++){
		filesMap[index] = remain % 2;
		remain = floor(remain / 2);
		index--;
	}

	// Storing into the "files" array the names of the found files
	int foundFiles = 0;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (filesMap[i] == 1){
			foundFiles = foundFiles + 1;
			files[i] = inodes[i].name;
		}
	}

	return foundFiles;
}
