#ifndef TESTDATA_H
#define TESTDATA_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int tempdata[256];

class TestData{
public:
   TestData();
   ~TestData();
   void get_randoms(void *buf, int nbytes);
   int get_random_data();
};

#endif //TESTDATA_H
