#include "tabla.h"

NachosFilesTable::NachosFilesTable(){
  openFiles = NULL;
  openFilesMap = new BitMap(32);
  usage = 0;
}

NachosFilesTable::~NachosFilesTable(){
  delete openFilesMap;
}

int NachosFilesTable::Open(int UnixHandle){
  int pos = 0;

  while(openFiles[pos] != NULL){
    pos++;
  }
  openFiles[pos] = UnixHandle;
  openFilesMap->Mark(pos);
  return pos;
}

int NachosFilesTable::Close(int NachosHandle){
  int fdUnix = openFiles[NachosHandle];

  return 0;
}

bool NachosFilesTable::isOpen(int NachosHandle){
  return false;
}

int NachosFilesTable::getUnixHandle(int NachosHandle){
  return 0;
}

void NachosFilesTable::addThread(){

}
void NachosFilesTable::delThread(){

}

/*
 * NachosObjectsTable 
 */

NachosObjectsTable::NachosObjectsTable() {
  objectAddress = new int * [NumPhysPages];

  for(int i = 0; i < NumPhysPages; ++i) {
    objectAddress[i] = NULL;
  }
}

NachosObjectsTable::~NachosObjectsTable() {
  delete [] objectAddress;
}

int NachosObjectsTable::Create(int * ObjectPointer) {
  int pos = 0;

  while(objectAddress[pos] != NULL) {
    ++pos;
  }

  objectAddress[pos] = ObjectPointer;

  return pos;
}

void NachosObjectsTable::Delete(int ObjectHandle) {
  objectAddress[ObjectHandle] = NULL;
}

bool NachosObjectsTable::Exists(int ObjectHandle) {
  return objectAddress[ObjectHandle] != NULL;
}