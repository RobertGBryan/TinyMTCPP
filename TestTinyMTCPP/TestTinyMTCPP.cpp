// TestTinyMTCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <sys/timeb.h>
//
#include "MTTiny32.h"
#include "MTTiny64.h"
// Pull in the original C code so it can be tested against the C++ version.
// These 4 original c files must either reside in the build dir or in the build dir path.
#include "tinymt32.h"
#include "tinymt32.c"
#include "tinymt64.h"
#include "tinymt64.c"

int _tmain(int argc, _TCHAR* argv[])
{
   BenchMarkTest();
   VerifyCAndCPPVersions();
	return 0;
}

void VerifyCAndCPPVersions()
{
   // This method tests out the original c version against the new c++ version to make sure that
   // no bugs were introduced.  A sequence of 1,000,000 ran numbers are generated and compared.
   // If there are no differnces, then the test passes.  This test is performed for both the 32 bit
   // and 64 bit versions of TinyMT.
   int Loop;
   int TestCount = 1000000;
   // Init the original c rng.
   uint32 RanNum1, RanNum2;
   uint64 Ran64Num1, Ran64Num2;
   uint32 Mat641 = 0xfa051f40;
   uint32 Mat642 = 0xffd0fff4;
   uint64 TMat64 = 0x58d02ffeffbfffbc;
   uint32 Mat1 = 0x837c106f;
   uint32 Mat2 = 0xfc18ff07;
   uint32 TMat = 0xeeb9bdff;
   tinymt32_t tinymt;
   tinymt.mat1 = Mat1;
   tinymt.mat2 = Mat2;
   tinymt.tmat = TMat;
   int Seed = 98765;
   int64 Seed64 = 98765;
   // Test the 32 bit version first.
   tinymt32_init(&tinymt, Seed);
   // Init the cpp rng version.
   TinyMT32 TMT32(Seed, Mat1, Mat2, TMat);
   printf("Executing VerifyCAndCPPVersions 32 bit version test\n");
   // Compare each rng value from the cpp version with the c version.
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RanNum1 = tinymt32_generate_uint32(&tinymt);
      RanNum2 = TMT32.GetRandInt();
      if (RanNum1 != RanNum2)
      {
         printf("   Test failed at Loop = %d\n", Loop);
         return;
      }
   }
   printf("   Test passed.\n");
   //
   // Test the 64 bit version.
   tinymt64_t tinymt64;
   tinymt64.mat1 = Mat641;
   tinymt64.mat2 = Mat642;
   tinymt64.tmat = TMat64;
   tinymt64_init(&tinymt64, Seed64);
   // Init the cpp rng version.
   TinyMT64 TMT64(Seed64, Mat641, Mat642, TMat64);
   printf("Executing VerifyCAndCPPVersions 64 bit version test\n");
   // Compare each rng value from the cpp version with the c version.
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      Ran64Num1 = tinymt64_generate_uint64(&tinymt64);
      Ran64Num2= TMT64.GetRandInt();
      if (Ran64Num1 != Ran64Num2)
      {
         printf("   Test failed at Loop = %d\n", Loop);
         return;
      }
   }
   printf("   Test passed.\n");
}

void BenchMarkTest()
{
   // This method benchmarks how fast 100 million random numbers can be produced by tinyMT32.
   // Tests show that TinyMT32 is roughly 5 times faster than the C++ Rand method.
   // Since the Rand method only returns 15 bit ints, to get the same number of bytes as TinyMT,
   // at least twice as many calls must be made.  Thus, the TinyMT32 rng is in reality at least 10 times faster.
   // The cpp version is about the same speed as the original C version.  The cpp version can produce at least
   // 200M 32 bit random numbers in under a second on a single threaded i7 950 cpu (3.07 ghz).
   // This test also benchmarks the 64 bit version.  Tests show that it is almost twice as slow as the 32 bit
   // version.  This is probably due to it using larger 64-bit arithmetic .vs. 32 bit.  However, the 64 bit
   // version of GetRandBytes appears to be at least 5% faster than the 32 bit version.
   //
   int Loop;
   int TestCount = 100000000;
   const int BufSize = 4000000;
   unsigned int RanNum;
   unsigned int HighBits = 65535;
   uint32 Mat1, Mat2, TMat;
   tinymt32_t tinymt;
   Mat1 = tinymt.mat1 = 0x837c106f;
   Mat2 = tinymt.mat2 = 0xfc18ff07;
   TMat = tinymt.tmat = 0xeeb9bdff;
   int Seed = 98765;
   struct _timeb TimeBufStart;
   struct _timeb TimeBufEnd;
   time_t StartTimeInSecs, EndTimeInSecs, TimeInSecs;
   //
   // Get the time for executing 1M calls of the original c version of tineMT32 rng.
   //
   _ftime64_s(&TimeBufStart);
   tinymt32_init(&tinymt, Seed);
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RanNum = tinymt32_generate_uint32(&tinymt);
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100M TinyMT32 C   rng numbers = %I64d milliseconds\n", TimeInSecs);
   //
   // Get the time for executing 100M calls of the cpp version of tineMT32 rng.
   //
   _ftime64_s(&TimeBufStart);
   TinyMT32 TMT32(Seed, Mat1, Mat2, TMat);
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RanNum = TMT32.GetRandInt();
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100M TinyMT32 CPP rng numbers = %I64d milliseconds\n", TimeInSecs);
   //
   // Get the time for executing 100M calls of the C version of tineMT64 rng.
   //
   _ftime64_s(&TimeBufStart);
   TINYMT64_T tiny64MT;
   uint64_t Mat164, Mat264, TMat64, RanNum64;
   uint64_t Seed64 = 98765;
   tinymt64_init(&tiny64MT, Seed64);
   Mat164 = tiny64MT.mat1 = 0x14e9029c;
   Mat264 = tiny64MT.mat2 = 0xe8b0fa2c;
   TMat64 = tiny64MT.tmat = 0xf5f77effffbffffe;
   tinymt64_init(&tiny64MT, Seed64);
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RanNum64 = tinymt64_generate_uint64(&tiny64MT);
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100M TinyMT64 C   rng numbers = %I64d milliseconds\n", TimeInSecs);
   //
   // Get the time for executing 100M calls of the cpp version of tineMT64 rng.
   //
   _ftime64_s(&TimeBufStart);
   TinyMT64 TMT64(Seed, Mat1, Mat2, TMat);
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RanNum64 = TMT64.GetRandInt();
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100M TinyMT64 CPP rng numbers = %I64d milliseconds\n", TimeInSecs);
   //
   // Get the time for executing 100 calls to the TinyMT32.GetRandBytes method at 4M bytes each.
   //
   _ftime64_s(&TimeBufStart);
   TinyMT32 TM32(Seed, Mat1, Mat2, TMat);
   unsigned char *pBuf = new unsigned char[BufSize];
   for (Loop = 0; Loop < 100; Loop++)
   {
      TM32.GetRandBytes(pBuf, BufSize);
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100 TinyMT32.GetRandBytes of 4M random bytes = %I64d milliseconds\n", TimeInSecs);
   //
   // Get the time for executing 100 calls to the TinyMT64.GetRandBytes method at 4M bytes each.
   //
   _ftime64_s(&TimeBufStart);
   TinyMT64 TM64(Seed, Mat1, Mat2, TMat);
   for (Loop = 0; Loop < 100; Loop++)
   {
      TM64.GetRandBytes(pBuf, BufSize);
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100 TinyMT32.GetRandBytes of 4M random bytes = %I64d milliseconds\n", TimeInSecs);
   delete[] pBuf;
   //
   // Get the time for executing Rand().
   //
   // int HighNum = 0;
   // int LowNum = 99999;
   int RNum;
   _ftime64_s(&TimeBufStart);
   srand(123456);
   for (Loop = 0; Loop < TestCount; Loop++)
   {
      RNum = rand();
      /* The following code proves that range of rand() is 0 - 32767 inclusive.
      if (RNum > HighNum)
         HighNum = RNum;
      else if (RNum < LowNum)
         LowNum = RNum;
      */
   }
   _ftime64_s(&TimeBufEnd);
   StartTimeInSecs = (TimeBufStart.time * 1000) + TimeBufStart.millitm;
   EndTimeInSecs = (TimeBufEnd.time * 1000) + TimeBufEnd.millitm;
   TimeInSecs = EndTimeInSecs - StartTimeInSecs;
   printf("Time to execute 100M C++ Rand() rng numbers = %I64d milliseconds\n", TimeInSecs);
   // printf("HighNum = %d,  LowNum = %d\n", HighNum, LowNum);
}

