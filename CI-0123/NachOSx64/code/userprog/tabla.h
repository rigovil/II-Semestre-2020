#include "bitmap.h"
#include "machine.h"

class NachosFilesTable {

public:
  NachosFilesTable();
  ~NachosFilesTable();

  int Open(int UnixHandle);
  int Close(int NachosHandle);
  bool isOpen(int NachosHandle);
  int getUnixHandle(int NachosHandle);
  void addThread();
  void delThread();

void Print();

private:
  int * openFiles;
  BitMap *openFilesMap;
  int usage;
};


#define NULL_PTR 0

class NachosObjectsTable {

  public:
    NachosObjectsTable();
    ~NachosObjectsTable();

    int Create(int * ObjectPointer);
    int * getObject(int ObjectHandle) { return objectAddress[ObjectHandle]; }
    bool Exists(int ObjectHandle);
    void Delete(int ObjectHandle);
    void addThread();
    void delThread();

  private:
    int ** objectAddress;
};