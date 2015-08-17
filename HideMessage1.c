#include <stdio.h>
#include <stdlib.h>

unsigned int GetBit(unsigned char*, unsigned long);
void Encode(unsigned char*, unsigned char*, unsigned long, unsigned long);

int main (int argc, char *argv[])
{
	FILE *fin1, *fin2, *fout;
	unsigned char *inputBuf, *hiddenBuf;
	unsigned int inputfilesize, hiddenfilesize;
	unsigned int startingPos, index;

	// Getting the file names from the command line arguments
	if (argc < 4) {
		fprintf (stderr, "Usage: <exe-name> <InputVideo> <HiddenVideo> <OutputVideo>\n");
		return -1;
	}

	// Opening the input file and checking whether it exists
	if ((fin1 = fopen (argv[1], "rb")) == NULL) {
		fprintf (stderr, "Error: Opening Input video file...\n");
		return -1;
	}
	// Opening the hidden video file and checking whether it exists in the disk
	if ((fin2 = fopen (argv[2], "rb")) == NULL) {
		fprintf (stderr, "Error: Opening hidden video file.\n");
		return -1;
	}
	// Opening the output file
	if ((fout = fopen (argv[3], "wb")) == NULL) {
		fprintf (stderr, "Error: Opening output video file...\n");
		return -1;
	}

	printf ("Enter the starting position : ");
	scanf ("%d", &startingPos);

	// Get the Input video file size
	fseek (fin1, 0, SEEK_END);
	inputfilesize = ftell (fin1);
	rewind (fin1);
	// Get the Hidden video file size
	fseek (fin2, 0, SEEK_END);
	hiddenfilesize = ftell (fin2);
	rewind (fin2);

	if (inputfilesize < hiddenfilesize * 8) {
		fprintf (stderr, "Filesize is more. It is impossible to hide with this input video. Select other video.\n");
		return -1;
	}

	// Allocating memory for both the Input and Hidden file video datas
	// inputBuf = (unsigned char*) malloc (inputfilesize);
	inputBuf = (unsigned char*) malloc(8);
	// hiddenBuf = (unsigned char*) malloc (hiddenfilesize);
	hiddenBuf = (unsigned char*) malloc(1);

	// Reading the Input and Hidden video data
	// printf ("Reading both the input data's\n");
	// fread (inputBuf, sizeof (unsigned char), inputfilesize, fin1);
	// fread (hiddenBuf, sizeof (unsigned char), hiddenfilesize, fin2);

	// Embed the hidden video into the Input video using LSB Algorithm
	printf ("Embedding started . . . \n");
	// Encode (inputBuf + startingPos - 1, hiddenBuf, inputfilesize, hiddenfilesize);

	// Move the file pointer
	for(index = 0; index < startingPos - 1; index++)
	{
		fread(hiddenBuf, sizeof(unsigned char), 1, fin1);
		fwrite(hiddenBuf, sizeof(unsigned char), 1, fout);
	}

	for(index = 0; index < hiddenfilesize; index++)
	{
		// Read the input video
		fread(inputBuf, sizeof(unsigned char), 8, fin1);
		// Read the stego
		fread(hiddenBuf, sizeof(unsigned char), 1, fin2);
		// Hide the stego video
		Encode(inputBuf, hiddenBuf, 8, 1);
		// Write the embedded data to the disk
		fwrite(inputBuf, sizeof(unsigned char), 8, fout);
	}
	// Store the remaining data
	for(index = ftell(fin1); index < inputfilesize; index++)
	{
		fread(hiddenBuf, sizeof(unsigned char), 1, fin1);
		fwrite(hiddenBuf, sizeof(unsigned char), 1, fout);
	}

	// Write the embedded video onto the disk
	// fwrite (inputBuf, sizeof (unsigned char), inputfilesize, fout);
	printf ("Number of bytes hidden = %d\n", hiddenfilesize);
	printf ("Embedding Done  . . . \n\n\n");

	fclose (fin1);
	fclose (fin2);
	fclose (fout);

	return 0;
}

void Encode(dest, src, fs1, fs2)
unsigned char *dest, *src;
unsigned long fs1, fs2;
{
	unsigned long index;
	
	for(index = 0; index < (fs2 * 8); index++) {
		if(GetBit(src, index))
			dest[index] = dest[index] | 0x1;
		else
			dest[index] = dest[index] & 0xfe;
	}

	return;
}

unsigned int GetBit(unsigned char *array, unsigned long bitno)
{
	return (array[bitno / 8] & (1 << (7 - (bitno % 8)))) > 0 ? 1 : 0;
}
