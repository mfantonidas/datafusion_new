#include "TestData.h"

TestData::TestData()
{
}

TestData::~TestData()
{
}

void TestData::get_randoms(void *buf, int numbers)
{
  unsigned int seed;
  struct timeval now;
  int i=0;
  
  gettimeofday(&now, NULL);
  seed = now.tv_sec ^ now.tv_usec;

  for(i = 0; i < numbers; i++)
  {
    *(int *)buf = (int)rand_r(&seed)/RAND_MAX*26.4+0.1;
  }
}

int TestData::get_random_data()
{
  unsigned int seed;
  struct timeval now;
  int data;

  gettimeofday(&now, NULL);
  seed = now.tv_sec ^ now.tv_usec;
  data = (int)rand_r(&seed)/RAND_MAX*26.4 + 0.1;

  return data;
}
