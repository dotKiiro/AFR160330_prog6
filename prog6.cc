/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include "BinaryFile.h"
#include "cdk.h"


#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 30
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

CDKMATRIX* setupCDK();

BinaryFileHeader* setupHeaders(ifstream&);

void displayHeader(CDKMATRIX*, BinaryFileHeader*);

void setupRecords(ifstream&, BinaryFileRecord**, int);

int main(void)
{
   // cdk matrix
   CDKMATRIX *myMatrix;

   myMatrix = setupCDK();
   
   // exits if matrix didn't setup properly
   if (myMatrix ==NULL)
   {
      printf("Error creating Matrix\n");
      _exit(1);
   }

   /* Display the Matrix */
   drawCDKMatrix(myMatrix, true);
   
   // bin file to read from
   ifstream binInfile ("cs3377.bin", ios::in | ios::binary);

   // reads the headers from the file and sets them up
   BinaryFileHeader *fileHeader = setupHeaders(binInfile);

   // adds the headers to the matrix
   displayHeader(myMatrix, fileHeader);

   int totalRecords = fileHeader->numRecords;

   BinaryFileRecord *fileRecords[totalRecords];

   setupRecords(binInfile, fileRecords, totalRecords);

   drawCDKMatrix(myMatrix, true);    /* required  */

   /* So we can see results, pause until a key is pressed. */
   unsigned char x;
   cin >> x;

   // Cleanup screen
   endCDK();
}

CDKMATRIX* setupCDK()
{
   WINDOW	*window;
   CDKSCREEN	*cdkscreen;
   CDKMATRIX     *myMatrix;           // CDK Screen Matrix

   // Remember that matrix starts out at 1,1.
   // Since arrays start out at 0, the first entries
   // below ("R0", and "C0") are just placeholders
   // 
   // Finally... make sure your arrays have enough entries given the
   // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
   // above.

   const char 		*rowTitles[] = {"a", "b", "c", "d", "e", "f"};
   const char 		*columnTitles[] = {"s", "b", "c", "d", "e", "f"};
   int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
   int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

   /*
    * Initialize the Cdk screen.
    *
    * Make sure the putty terminal is large enough
    */
   window = initscr();
   cdkscreen = initCDKScreen(window);

   /* Start CDK Colors */
   initCDKColor();

   /*
    * Create the matrix.  Need to manually cast (const char**) to (char **)
    */
   myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);
   return myMatrix;
}

BinaryFileHeader* setupHeaders(ifstream &binInfile)
{
  BinaryFileHeader *fileHeader = new BinaryFileHeader();

  binInfile.read((char*) fileHeader, sizeof(BinaryFileHeader));

  return fileHeader;
}

void displayHeader(CDKMATRIX *myMatrix, BinaryFileHeader *fileHeader)
{
    // stream to build text to display
   stringstream stream;
   // full text to display in each cell
   string cellText;

   // gets magic number ready
   stream << "0x" << hex << uppercase << fileHeader->magicNumber;
   stream >> cellText;
   cellText = "Magic: " + cellText;
   
   // displays magic number
   setCDKMatrixCell(myMatrix, 1, 1, cellText.c_str());

   cellText = "";
   stream.str("");
   stream.clear();

   stream << dec <<  fileHeader->versionNumber;
   stream >> cellText;
   cellText = "Version: " + cellText;

   setCDKMatrixCell(myMatrix, 1, 2, cellText.c_str());

   cellText = "";
   stream.str("");
   stream.clear();

   stream << fileHeader->numRecords;
   stream >> cellText;
   cellText = "numRecords: " + cellText;

   setCDKMatrixCell(myMatrix, 1, 3, cellText.c_str());
}

void setupRecords(ifstream &binInfile, BinaryFileRecord **fileRecords, int numRecords)
{
   for (int i = 0; i < numRecords; i++)
   {
      fileRecords[i] = new BinaryFileRecord();
      binInfile.read(fileRecords[i]->stringBuffer, sizeof(fileRecords[i]->stringBuffer));
      fileRecords[i]->strLength = strlen(fileRecords[i]->stringBuffer);
   }
}
