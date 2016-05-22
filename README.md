# File System

### What is it?
The <a href="https://en.wikipedia.org/wiki/File_system">File System</a> is the part of the operating System in charge of organizing and dealing with folders and files. In order to do so, it needs to manage not only the data, but also the metadata of every folder or file.

### What is in the repository?
The files contained with the repository are the following ones:

#### 1. create_disk
File that checks if the file <i>disk.dat</i> exist and can be open. This file will simulate a real hard disk.

#### 2. disk.dat
File that simulates a real hard disk. It is full of zeroes.

#### 3. filesystem
File where the file system is implemented. It has the following <b>characteristics</b>:

1. It can contain up to 50 files.
2. The maximum file name could contain up to 64 characters.
3. The maximum file size is 4KB.
4. The file system block size is 4KB.
5. There could be up to 30 tags into the file system.
6. The maximum tag name could contain up to 32 characters.
7. Each file could have up to 3 associated tags.

These characteristics give us an idea of the size of the file system. Although it is a small one, the concepts applied to its development should be applied into the development of largest file systems.

#### 4 include
Folder containning a pair of header files.

#### 5. Makefile
File to compile the other repository files. It can be also used to remove the compiled ones.
```shell
$ make
gcc -g -Wall -I./include -o create_disk create_disk.c
gcc -g -Wall -I./include   -c -o ufs.o ufs.c
gcc -g -Wall -I./include   -c -o filesystem.o filesystem.c
ar rcv libfs.a ufs.o filesystem.o
a - ufs.o
a - filesystem.o
gcc -g -Wall -I./include -o test test.c libfs.a
```

```shell
$ make clean 
rm -f libfs.a ufs.o filesystem.o test create_disk create_disk.o
```

#### 6. ufs
File that simulates the hard disk driver. It allow us to read and write entire blocks of data from the disk. Remember that the block size is 4KB.

### Possible operations

* <b>mkFS (maxNumFiles, deviceSize):</b> It will initialize all the metadata structures.<br>
<b>Parameters:</b> The maximum number of files and the total device size (between 320KB and 500KB).
* <b>mountFS:</b> It will simulate the mounting process into the Operating System current file system.
* <b>umountFS:</b> It will simulate the unmounting process into the Operating System current file system.
* <b>creatFS (fileName):</b> It will create a file.<br>
<b>Parameters:</b> The name of the created file.
* <b>openFS (fileName):</b> It will open a file in order to read or write on it.<br>
<b>Parameters:</b> The name of the file.
* <b>closeFS (fileDescriptor):</b> It will close the specified file.<br>
<b>Parameters:</b> The name of the file.
* <b>readFS (fileDescriptor, buffer, numBytes):</b> It will read the data of a given file from the reader/writer header up to the number of bytes specified, storing the result into the buffer.<br>
<b>Parameters:</b> The file descriptor, a buffer of at least 4KB, and the number of bytes to read.
* <b>writeFS (fileDescriptor, buffer, numBytes):</b> It will write as many bytes as specified into the file, from the buffer.<br>
<b>Parameters:</b> The file descriptor, a buffer containing the content to be written and the number of bytes to write.
* <b>lseekFS (fileDescriptor, offset, whence):</b> It will move the reading/writting header of a file to the specified position.
* <b>tagFS (fileName, tagName):</b> It will tag a file with the specified tag. If the tag does not exist, it will be created.<br>
<b>Parameters:</b> The file name and the tag name. 
* <b>untagFS (fileName, tagName):</b> It will remove the specified tag from the specified file. Returns and error if the tag was not previously associated to that file.<br>
<b>Parameters:</b> The file name and the tag name.
* <b>listFS (tagName, files):</b> It will store into the <i>files</i> array all the files containing the specified tag.<br>
<b>Parameters:</b> The tag name and an an array of strings <i>(char**)</i>.


### Testing
There is an additional file with the repository, called <i>test.c</i>. It is where all the different possible operations for the file system are tested, checking if they do what they are supossed to do. <b>Every operation has been checked</b>. Additional advance tests could be added if needed.


### Requirements
This File System is designed to work in a UNIX operating system. The reasons of this requirement are two: First of all a GCC (GNU Compiler-Compiler) is needed to execute the Makefile and compile the files; and secondly, because the System Calls could have a different name in other operating systems. Those which fullfill the requirements are:

<b>A) Mac OSX.</b><br>
<b>B) Any Linux distribution.</b>
