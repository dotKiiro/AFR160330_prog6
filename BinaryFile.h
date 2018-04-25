/*
 * CS 3377 - C/C++ Programming in a Unix Environment
 *
 * File: BinaryFile.h
 * Author: Andres Ramirez
 * Email:  afr160330@utdallas.edu
 */

#ifndef BINARY_FILE_H
#define BINARY_FILE_H

// class for the header in the file
class BinaryFileHeader
{
   public:

   uint32_t magicNumber;
   uint32_t versionNumber;
   uint64_t numRecords;
};

const int maxRecordStringLength = 25;

// class for a single record in the file
class BinaryFileRecord
{
   public:
   
   uint8_t strLength;
   char stringBuffer[maxRecordStringLength];
};

#endif
