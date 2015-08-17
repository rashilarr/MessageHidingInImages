#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Decode(unsigned char*, unsigned char*, unsigned long, unsigned long);
unsigned int GETBIT(unsigned char *array, unsigned long bitno);

int main (int argc, char *argv[])
{
	FILE *fin, *fout;
	unsigned char *stegoBuf, *outputBuf;
	unsigned int inputfilesize, startingPos, hiddensize, index;

	if (argc < 3) {
		fprintf (stderr, "Usage: <%s> <stegovideo> <extractedvideo>\n", argv[0]);
		return -1;
	}

	// Open both the input and output file
	if ((fin = fopen (argv[1], "rb")) == NULL) {
		fprintf (stderr, "Error: Opening stego video file . . .\n");
		return -1;
	}
	if ((fout = fopen (argv[2], "wb")) == NULL) {
		fprintf (stderr, "Error: Opening output video file . . .\n");
		return -1;
	}

	// Find the input filesize
	fseek (fin, 0, SEEK_END);
	inputfilesize = ftell (fin);
	rewind (fin);

	printf ("Enter the starting position : ");
	scanf ("%d", &startingPos);
	printf ("Enter the hidden bytes : ");
	scanf ("%d", &hiddensize);

	// Check whether the entered data is proper
	if (hiddensize * 8 > inputfilesize - startingPos) {
		fprintf (stderr, "Entered data is improper . . .\n");
		return -1;
	}

	// Allocate memory for both the input and output contents
	// stegoBuf = (unsigned char*) malloc (inputfilesize);
	stegoBuf = (unsigned char*) malloc(8);
	// outputBuf = (unsigned char*) malloc (hiddensize);
	outputBuf = (unsigned char*) malloc(1);

	// Reading the input video data
	// fread (stegoBuf, sizeof (unsigned char), inputfilesize, fin);
	// Move the file pointer to the desired location
	fseek(fin, startingPos - 1, SEEK_SET);

	// Extract the data
	printf ("Extracting the hidden video . . .\n");
	for(index = 0; index < hiddensize; index++)
	{
		fread(stegoBuf, sizeof(unsigned char), 8, fin);
		*outputBuf = 0;
		Decode(outputBuf, stegoBuf, 8, 1);
		fwrite(outputBuf, sizeof(unsigned char), 1, fout);
	}
	// memset (outputBuf, 0, hiddensize);
	// Decode (outputBuf, stegoBuf + startingPos - 1, inputfilesize, hiddensize);

	// Store the extracted data on the disk
	// fwrite (outputBuf, sizeof (unsigned char), hiddensize, fout);
	printf ("Extraction Done . . .\n\n\n");

	free (stegoBuf);
	free (outputBuf);

	fclose (fin);
	fclose (fout);

	return 0;
}

void Decode(unsigned char *dest, unsigned char *src, unsigned long fs1, unsigned long fs2)
{
	unsigned long index;
	for(index = 0; index < (fs2 * 8); index++) {
		dest[index / 8] = dest[index / 8] | (GETBIT(src, index) << (7 - (index % 8)));
	}
	
	return;
}

unsigned int GETBIT(unsigned char *array, unsigned long bitno)
{
	return array[bitno] & 0x1;
}
