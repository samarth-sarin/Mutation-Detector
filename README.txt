//g++ -Wall -Werror -pedantic -std=c++1y -o gene gene.cpp
./gene

This program allows you to compare the single strand of DNA of a human and their respective transcripted strand. The program will transcribe their original strand and compare it to their actual transcripted strand in order to determine if any mistakes happened in the transcription process and will identify mutations. Not all mutations are harmful- which are identified in this program.


Here are some examples of strands to use.

Silent:
TACATGGGCATT
AUGUACCCUUAA


Nonsense:
TACATGGGCATT
AUGUAGCCGUAA


Missense:
TACATGGGCATT
AUUUACCCGUAA

Insertion:
TACATGGGCATT
AUGUACCGCGUAA


Deletion:
TACATGGGCATT
AUGUACCGUAA
