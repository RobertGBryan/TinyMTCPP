using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestRandomNS
{
   class TestRandom
   {
      // This class tests out how fast the Random class is at generateing random numbers and also
      // performs a test in order to try the period to determine when the random number sequence repeats.
      //
      const int TestCount = 100000000;

      static void Main(string[] args)
      {
         // TestPeriod();
         BenchmarkTests();
      }

      private static void BenchmarkTests()
      {
         // This method benchmarks how fast the random class is so that it can be compared with the C++ TinyMT
         // class is.
         int Loop, RanNum;
         double dRanNum;
         DateTime EndTime;
         TimeSpan TS;
         String S;
         Console.WriteLine("Running bench mark tests:");
         Console.WriteLine("Running Random.Next test");
         DateTime StartTime = DateTime.Now;
         Random R = new Random(12345);
         const int BufSize = 4000000;
         byte[] Buf = new byte[BufSize];
         for (Loop = 0; Loop < TestCount; Loop++)
         {
            RanNum = R.Next();
         }
         EndTime = DateTime.Now;
         TS = EndTime - StartTime;
         S = "Time to obtain " + TestCount.ToString() + " 4-byte ints = " + TS.TotalSeconds.ToString();
         Console.WriteLine(S);
         //
         Console.WriteLine("Running Random.NextDouble test");
         StartTime = DateTime.Now;
         R = new Random(12345);
         for (Loop = 0; Loop < TestCount; Loop++)
         {
            dRanNum = R.NextDouble();
         }
         EndTime = DateTime.Now;
         TS = EndTime - StartTime;
         S = "Time to obtain " + TestCount.ToString() + " 8-byte doubles = " + TS.TotalSeconds.ToString();
         Console.WriteLine(S);
         //
         Console.WriteLine("Running Random.NextDouble test");
         StartTime = DateTime.Now;
         R = new Random(12345);
         for (Loop = 0; Loop < 100; Loop++)
         {
            R.NextBytes(Buf);
         }
         EndTime = DateTime.Now;
         TS = EndTime - StartTime;
         S = "Time to obtain " + " 100 4mb random bytes = " + TS.TotalSeconds.ToString();
         Console.WriteLine(S);
      }

      private static void TestPeriod()
      {
         // This method looks for when the period of the Random class repeats itself.  This is done by
         // storing the first 100 random numbers into a buffer and then checking each new random number
         // number sequence against the stored values.  No repeat of period found after calling the Random.next
         // 1 trillion times.
         // 
         int Loop1, Loop2, Loop3;
         int RemBytes = 0;
         byte[] Buf = new byte[100];
         byte[] RanBuf = new byte[1];
         byte[] RanSeq = new byte[1000000];
         long[] Stats = new long[256];
         Random R = new Random(12345);
         String S;
         //
         Console.WriteLine("Running TestPeriod:");
         // Get the first 100 bytes which are used to detect the start of the sequence.
         for (Loop1 = 0; Loop1 < 100; Loop1++)
         {
            R.NextBytes(RanBuf);
            Buf[Loop1] = RanBuf[0];
         }
         // Get the next 1M random numbers.
         for (Loop1 = 0; Loop1 < 1000000; Loop1++)
         {
            R.NextBytes(RanBuf);
            RanSeq[Loop1] = RanBuf[0];
         }
         // Look through 1,000,000,000,000 random numbers
         for (Loop1 = 0; Loop1 < 1000000; Loop1++)
         {
            for (Loop2 = 0; Loop2 < 1000000; Loop2++)
            {
               if (1000000 - Loop2 < 100)
               {
                  RemBytes = 1000000 - Loop2;
                  break;
               }
               // If this random number matches the first entry in the buffer, then check if the
               // next 99 also match.
               if (RanSeq[Loop2]== Buf[0])
               {
                  Stats[0]++;
                  for (Loop3 = 1; Loop3 < 100; Loop3++)
                  {
                     if (RanSeq[Loop2 + Loop3] != Buf[Loop3])
                        break;
                     Stats[Loop3]++;
                  }
                  if (Loop3 >= 99)
                  {
                     // The beginning of the sequence has been found at this point - let the user know.
                     Console.WriteLine("");
                     S = "   Repeat of sequence found at Loop1 = " + Loop1.ToString() + ", Loop2 = " + Loop2.ToString();
                     Console.WriteLine(S);
                     return;
                  }
               }
            }
            // Copy the last few bytes at the end of RanSeq that was not processed to the beg of the buf.
            for (Loop2 = 0; Loop2 < RemBytes; Loop2++)
            {
               RanSeq[Loop2] = RanSeq[1000000 - RemBytes + Loop2];
            }
            // Get the next set of random numbers to fill up the buffer.
            for (Loop2 = RemBytes; Loop2 < 1000000; Loop2++)
            {
               R.NextBytes(RanBuf);
               RanSeq[Loop2] = RanBuf[0];
            }
            if (Loop1 % 100 == 0)
               Console.Write(".");
         }
         Console.WriteLine("");
         S = "   Beg of sequence not found with Loop1 = 100000, Loop2 = 1000000";
         Console.WriteLine(S);
         Console.WriteLine();
         S = "   How often a given number of bytes were found:";
         Console.WriteLine(S);
         for (Loop1 = 0; Loop1 < 256; Loop1++)
         {
            if (Stats[Loop1] > 0)
            {
               S = "   Stats[" + Loop1.ToString() + "] = " + Stats[Loop1].ToString();
               Console.WriteLine(S);
            }
            else
               break;
         }
      }
   }
}
