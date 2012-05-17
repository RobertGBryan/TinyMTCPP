TinyMTCPP
=========

Port of the TinyMT project which is written in C to VC++.

# TinyMT - Tiny Mersenne Twister

A 2^127-1 period PRNG

## What is Tiny Mersenne Twister

(as described in <http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html>)

TinyMT is a new small-sized pseudo random number generator variant of Mersenne Twister (MT) introduced by
Mutsuo Saito and Makoto Matsumoto in 2011.  There are two types of TinyMT, tinymt32 and tinymt64. tinymt32 outputs 32-bit
unsigned integers and single precision floating point numbers.  On the other hand, tinymt64 outputs 64-bit unsigned integers
and double precision floating point numbers.

The purpose of TinyMT is not to replace Mersenne Twister. TinyMT has a far
shorter period than Mersenne Twister. The merit of TinyMT is in its small size
of the internal state of 127 bits, far smaller than 19937 bits used by its larger cousin Mersenne Twister.
The purpose of TinyMT may be used in a situation where a small lightweight object is preferred.
According to statistical tests (BigCrush), the quality of the outputs of TinyMT seems pretty good, taking the
small size of the internal state into consideration.

## TinyMT features

* The periods of generated sequences are 2^127-1
* The size of internal state space is 127 bits
* The state transition function is F2-linear
* The output function is not F2-linear
* TinyMTDC generates distinct parameter sets for TinyMT
* TinyMTDC can generate a large number of parameter sets (over 232 x 216)
* Parameter generation of TinyMTDC is fast

## TinyMT changes to the original C code:

* Made the class retain state .vs. passing in the state each time a ran number is needed.
* Added a method to fill an array with random bytes - see GetRandBytes().
* Replaced globals with class vars.
* Renamed methods, constants and types to something easier to use and remember.
* Moved the private methods to the private section.
* Cleaned up some comments.
* Made the MT32State and MT64State structs private.  Constructor now inits the object, but the Init method can still be called to re-init the object to a different seed so that constant creating / deleting this object won't fragment memory.
* Removed some methods:
  1. PeriodCertification - was not doing anything useful.
  2. Redundant floating point methods.
  3. Conversion functions that the above methods call.
  4, Init1 & Init2 - not being used in the 64 bit initialization methods.
