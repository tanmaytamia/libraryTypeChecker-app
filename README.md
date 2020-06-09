-------INTRODUCTION----------
Different computing devices use different CPU architectures, which in turn support different instruction sets. 
Application Binary Interface (ABI) is different for each combination of CPU and instruction set.
This program determines the total number of libraries present in a folder and its subfolders and architecture of each library.
Libraries can be of the following architecture type:
	armeabi: compiled code for all ARM based processors only
	armeabi-v7a: compiled code for all ARMv7 and above based processors only
	arm64-v8a: compiled code for all ARMv8 arm64 and above based processors only 
	x86: compiled code for x86 processors only
	x86_64: compiled code for x86_64 processors only 
	mips: compiled code for MIPS processors only.
	
-------ALGORITHM----------
Step1 Take directory path as an input.
Step2 For everyfile in this folder check elf header to find information about 'object file type' and its 'architecture' and store it in unordered_map.
	Step2.1 Struct to store only the required elf header information and this info is independent of 32 and 64 bit machine, therefore no need to check.
	Step2.2 elfdata to check byte ordering whether little endian or big endian and retreive values accordingly
	Step2.3 elfclass to check 32 or 64 bit object file type.
	Step2.4 emachine to check the architecture.
Step3 If directory, recursively follow step2.
Step4 Print the count of library files and their arcitecture type mentioned above.

-------COMPILATION AND INSTALLATION----------

g++ bluestack_assignmentv2.cpp -o bs
./bs

Input ::enter a path::

Output :: total count of all the library files inside the entered path.

-------REFERENCES----------
http://www.skyfree.org/linux/references/ELF_Format.pdf
https://code.woboq.org/userspace/glibc/elf/elf.h.html  