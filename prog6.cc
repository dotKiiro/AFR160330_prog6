/*
 * CS 3377 - C/C++ Programming in a Unix Environment
 *
 * File:   prog6.cc
 * Author: Andres Ramirez
 * Email:  afr160330@utdallas.edu
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

// prototypes
CDKMATRIX* setupCDK();
BinaryFileHeader* setupHeaders(ifstream&);
void displayHeader(CDKMATRIX*, BinaryFileHeader*);
void setupRecords(ifstream&, BinaryFileRecord**, int);
void displayRecords(CDKMATRIX*, BinaryFileRecord**, int);
void cleanupRecords(BinaryFileRecord**);

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

   // gets the number of records in int form
   int totalRecords = fileHeader->numRecords;

   // array of record instances to use
   BinaryFileRecord *fileRecords[totalRecords];

   // populates the array
   setupRecords(binInfile, fileRecords, totalRecords);
   
   // displays the rest of the records
   displayRecords(myMatrix, fileRecords, totalRecords);
   
   // shows the matrix
   drawCDKMatrix(myMatrix, true);

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

   // names the rows and columns

   const char 		*rowTitles[] = {"a", "b", "c", "d", "e", "f"};
   const char 		*columnTitles[] = {"s", "b", "c", "d", "e", "f"};
   int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
   int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

   /*
    * Initialize the Cdk screen.
    *
    * Make sure the terminal is large enough
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

// gets the header from the binary file and saves it to the header class
BinaryFileHeader* setupHeaders(ifstream &binInfile)
{
  BinaryFileHeader *fileHeader = new BinaryFileHeader();
  
  // saves it all in one go
  binInfile.read((char*) fileHeader, sizeof(BinaryFileHeader));

  return fileHeader;
}

// displays the header gotten from the binary file
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

   // resets stream and cell text for next cell
   cellText = "";
   stream.str("");
   stream.clear();
   
   // gets version number ready
   stream << dec <<  fileHeader->versionNumber;
   stream >> cellText;
   cellText = "Version: " + cellText;
   
   // displays version number
   setCDKMatrixCell(myMatrix, 1, 2, cellText.c_str());

   cellText = "";
   stream.str("");
   stream.clear();
   
   // gets num records ready
   stream << fileHeader->numRecords;
   stream >> cellText;
   cellText = "numRecords: " + cellText;

   // displays num records
   setCDKMatrixCell(myMatrix, 1, 3, cellText.c_str());
}

// saves records in record class array
void setupRecords(ifstream &binInfile, BinaryFileRecord **fileRecords, int numRecords)
{
   // char to toss leading character 
   char trash[1];
   // goes through to the number of records the header said
   for (int i = 0; i < numRecords; i++)
   {
      // tosses leading character
      binInfile.read(trash, sizeof(char));
      
      // makes a new instance in the array for every record
      fileRecords[i] = new BinaryFileRecord();
      
      // reads a single record and saves it
      binInfile.read(fileRecords[i]->stringBuffer, sizeof(fileRecords[i]->stringBuffer));
      
      // calculates the length and stores it
      int len = strlen(fileRecords[i]->stringBuffer);
      (fileRecords[i]->strLength) = (uint8_t) len;
   }
}

// displays the records gotten from the file in the matriz
void displayRecords(CDKMATRIX *myMatrix, BinaryFileRecord **fileRecords, int totalRecords)
{
   // stream to build text to display
   stringstream stream;
   // full text to display in each cell
   string cellText = "";

   // prints out each record by row 
   for (int i = 0; i < totalRecords; i++)
   {
      // preps the string length to be printed
      stream << (int) fileRecords[i]->strLength;
      stream >> cellText;
      // full text for string length to be printed
      cellText = "strlen: " + cellText;
       
      // sets both cells in the matrix to the required text 
      setCDKMatrixCell(myMatrix, i+2, 1, cellText.c_str());
      setCDKMatrixCell(myMatrix, i+2, 2, fileRecords[i]->stringBuffer);

      // clears the strlen text for the next go around
      cellText = "";
      stream.str("");
      stream.clear();
   }
}
