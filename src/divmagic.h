//#pragma once
// ! Credit to Hacker's Delight which signed division was taken from
// GCC Compiler's explanation: https://gmplib.org/~tege/divcnst-pldi94.pdf
// Unsigned division explanation: https://rubenvannieuwpoort.nl/posts/division-by-constant-unsigned-integers
#include <stdio.h>
#include <stdlib.h>

namespace divmag {
typedef struct ms {int M;          // Magic number
          int s;} magicst;         // and shift amount.

typedef struct msl {long M;          // Magic number
          long s;} magicslt;         // and shift amount.

typedef struct mu {unsigned M;     // Magic number,
          char d;          // round-down indicator,
          int p;           // pre-shift amount
          int s;} magicut;         // and shift amount.

typedef struct mul {unsigned long M;     // Magic number,
          char d;           // round-down indicator,
          int p;           // pre-shift amount
          int s;} magicult;         // and shift amount.

/* // ! for testing 
int main() {
   char testeru();
   struct ms magic(int);
   struct mul magicul(unsigned long);
   unsigned long divu128(unsigned, unsigned long);
   int failed[1000] = {};
   int count = 0;

   struct ms mags = magic(7);
   printf("mag = 0x%x, s = %d\n", mags.M, mags.s);

   struct mul mag = magicul(3l);
   printf("mag = %lx, d = %d, s = %d, p = %d\n", mag.M, mag.d, mag.s, mag.p);

   printf("%lx\n", divu128(45, 0x300000000000));
   return 0;

   return testeru();
}

char testeru() {
   struct mul magicul(unsigned long);
   char flip = 1;
   char succ = 1;
   unsigned long j = 0xFFFFFFFFFFFEFFFF;
   unsigned long end;
   printf("\033[0;32m");
   for (unsigned long i = 0xFFFFFFFFFFFEFFFF; i < 0xFFFFFFFFFFFFFFFF; i++) {
      if (!(i & (i-1))) continue;
      struct mul mag = magicul(i);
      unsigned long d = i;
      if (d > 18446744073709551615u/1000000016531u) {
         end = 0xFFFFFFFFFFFFFFFF;
      } else {
         end = 0xFFFFFFFFFFFFFFFF - d*1000000016531;
      }
      while (j <= end) {
         unsigned long n = (unsigned long) j;
         unsigned long _true = n/d;
         
         if (mag.p > 0) n >>= mag.p;
         if (mag.d) {
            n++;
            if (!n) n--;
         }

         unsigned long u0, u1, v0, v1, k, t;
         unsigned long w1, w2;

         u1 = n >> 32; u0 = n & 0xFFFFFFFF;
         v1 = mag.M >> 32; v0 = mag.M & 0xFFFFFFFF;

         t = u0*v0;
         k = t >> 32;

         t = u1*v0 + k;
         w1 = t & 0xFFFFFFFF;
         w2 = t >> 32;

         t = u0*v1 + w1;
         k = t >> 32;

         n = u1*v1 + w2 + k;

         n >>= (mag.s);
         printf("Testing %020lu, Divisor %lu, True: %020ld, Got: ", j, d, _true);
         if (n == _true) {
            printf("%020ld\n", n);
         } else {
            printf("\033[1;31m");
            printf("%020ld\n", n);
            printf("FAILED\n");
            printf("\033[0;32m");
            exit(1);
            succ = 0;         
         }
         if (!flip && j > 18446744073709551615u/1000000016531u) break;
         j = flip ? j+d*1000000016531-1 : j+1;
         if (d < 600000) { // low numbers are too slow
            unsigned long past = j;
            j *= 3;
            if (past / j != 3) break;
         }
         flip = !flip;
      }
      j=i+1;
      flip=1;
   }
   printf("\033[0;37m");
   
   return succ;
}

*/

// Computes the magic number for signed division.
struct ms magic(int d) {   // Must have 2 <= d <= 2**31-1
                           // or   -2**31 <= d <= -2.
   int p;
   unsigned ad, anc, delta, q1, r1, q2, r2, t;
   const unsigned two31 = 0x80000000;     // 2**31.
   struct ms mag;

   ad = abs(d);
   t = two31 + ((unsigned)d >> 31);
   anc = t - 1 - t%ad;     // Absolute value of nc.
   p = 31;                 // Init. p.
   q1 = two31/anc;         // Init. q1 = 2**p/|nc|.
   r1 = two31 - q1*anc;    // Init. r1 = rem(2**p, |nc|).
   q2 = two31/ad;          // Init. q2 = 2**p/|d|.
   r2 = two31 - q2*ad;     // Init. r2 = rem(2**p, |d|).
   do {
      p = p + 1;
      q1 = 2*q1;           // Update q1 = 2**p/|nc|.
      r1 = 2*r1;           // Update r1 = rem(2**p, |nc|).
      if (r1 >= anc) {     // (Must be an unsigned
         q1 = q1 + 1;      // comparison here).
         r1 = r1 - anc;}
      q2 = 2*q2;           // Update q2 = 2**p/|d|.
      r2 = 2*r2;           // Update r2 = rem(2**p, |d|).
      if (r2 >= ad) {      // (Must be an unsigned
         q2 = q2 + 1;      // comparison here).
         r2 = r2 - ad;}
      delta = ad - r2;
   } while (q1 < delta || (q1 == delta && r1 == 0));

   mag.M = q2 + 1;
   if (d < 0) mag.M = -mag.M; // Magic number and
   mag.s = p - 32;            // shift amount to return.
   return mag;
}

// Computes the magic number for signed division.
struct msl magicl(long d) {   // Must have 2 <= d <= 2**31-1
                           // or   -2**31 <= d <= -2.
   long p;
   unsigned long ad, anc, delta, q1, r1, q2, r2, t;
   const unsigned long two63 = 0x8000000000000000;     // 2**63.
   struct msl mag;

   ad = abs(d);
   t = two63 + ((unsigned long)d >> 63);
   anc = t - 1 - t%ad;     // Absolute value of nc.
   p = 63;                 // Init. p.
   q1 = two63/anc;         // Init. q1 = 2**p/|nc|.
   r1 = two63 - q1*anc;    // Init. r1 = rem(2**p, |nc|).
   q2 = two63/ad;          // Init. q2 = 2**p/|d|.
   r2 = two63 - q2*ad;     // Init. r2 = rem(2**p, |d|).
   do {
      p = p + 1;
      q1 = 2*q1;           // Update q1 = 2**p/|nc|.
      r1 = 2*r1;           // Update r1 = rem(2**p, |nc|).
      if (r1 >= anc) {     // (Must be an unsigned
         q1 = q1 + 1;      // comparison here).
         r1 = r1 - anc;}
      q2 = 2*q2;           // Update q2 = 2**p/|d|.
      r2 = 2*r2;           // Update r2 = rem(2**p, |d|).
      if (r2 >= ad) {      // (Must be an unsigned
         q2 = q2 + 1;      // comparison here).
         r2 = r2 - ad;}
      delta = ad - r2;
   } while (q1 < delta || (q1 == delta && r1 == 0));

   mag.M = q2 + 1;
   if (d < 0) mag.M = -mag.M; // Magic number and
   mag.s = p - 64;            // shift amount to return.
   return mag;
}

// Returns magic number for unsigned integer division by constant, input may not be 0 and must not be a power of 2
struct mu magicu(unsigned d) {
   struct mu mag;
   unsigned divu64(unsigned, unsigned);
   mag.d = 0;

   char l2 = 31;
   unsigned p = 0;
   l2 -= __builtin_clz(d);
   
   unsigned pow = 1 << l2;    // 2**l2
   unsigned m; 
   if (d < 0x10000) {
      // no overflow in 32 bits 2**(32+l2)//d + 1 if d is less than 2**16
      const unsigned two31 = 0x80000000; // 2**31
      unsigned f = two31/d;
      m = f * (pow << 1) + ((two31 - (f*d)) * (pow << 1))/d + 1;
   } else {
      m = divu64(l2, d) + 1;
   }
   if ((m * d) > pow) {      // m*d % 2**N < 2**l2
      if (!(d & 1)) {
         l2++;
         do {
            d >>= 1;
            p++;
            l2 -= 2;
         } while (!(d & 1) && l2 > 0);
         m = (m + (1 << (p - 1)) - 1) >> (p - 1); // m = ceil(m/2**p-1)
         if (l2 < 0) {
            m <<= 1;
            l2++;
         }
      } else {
         m--;
         mag.d = 1;
      }
   }
   while (!(m & 1) && l2) {
      m >>= 1;
      l2--;
   }
   mag.M = m;
   mag.p = p;
   mag.s = l2;
   return mag;
}

// Divides unsigned 2**u * (2**32) by v without overflowing in 32 bits, Knuth's Algorithm D
unsigned divu64(unsigned u, unsigned v) {
   // We work in base 2**16.
   // A uint32 holds a single digit. A uint64 holds two digits.
   // Our numerator is conceptually [num3, num2, num1, num0].
   // Our denominator is [den1, den0].
   const unsigned b = (1u << 16);

   // The high 32 bits of the dividend
   unsigned numhi = (1u << u);

   // The high and low digits of our computed quotient.
   unsigned short q1;
   unsigned short q0;

   // The normalization shift factor.
   int shift;

   // The high and low digits of our denominator (after normalizing).
   unsigned short den1;
   unsigned short den0;

   // A partial remainder.
   unsigned rem;

   // The estimated quotient, and its corresponding remainder (unrelated to true remainder).
   unsigned qhat;
   unsigned rhat;

   // Variables used to correct the estimated quotient.
   unsigned c1;
   unsigned c2;

   // Check for overflow and divide by 0, should be unreachable in this use case
   if (numhi >= v) {
      return ~0u;
   }

   // Determine the normalization factor. We multiply den by this, so that its leading digit is at
   // least half b. In binary this means just shifting left by the number of leading zeros, so that
   // there's a 1 in the MSB.
   // We also shift numer by the same amount. This cannot overflow because numhi < den.
   // The expression (-shift & 63) is the same as (64 - shift), except it avoids the UB of shifting
   // by 64. The funny bitwise 'and' ensures that numlo does not get shifted into numhi if shift is 0.
   // clang 11 has an x86 codegen bug here: see LLVM bug 50118. The sequence below avoids it.
   shift = __builtin_clz(v);
   v <<= shift;
   numhi <<= shift;

   // Extract the low digits of the numerator and both digits of the denominator.
   den1 = (unsigned short) (v >> 16);
   den0 = (unsigned short) (v & 0xFFFFu);

   // We wish to compute q1 = [n3 n2 n1] / [d1 d0].
   // Estimate q1 as [n3 n2] / [d1], and then correct it.
   // Note while qhat may be 2 digits, q1 is always 1 digit.
   qhat = numhi / den1;
   rhat = numhi - den1 * qhat;
   c1 = qhat * den0;
   c2 = rhat * b;
   if (c1 > c2)
      qhat -= (c1 - c2 > v) ? 2 : 1;
   q1 = qhat;

   // Compute the true (partial) remainder.
   rem = (numhi << 16) - q1 * v;

   // We wish to compute q0 = [rem1 rem0 n0] / [d1 d0].
   // Estimate q0 as [rem1 rem0] / [d1] and correct it.
   qhat = rem / den1;
   rhat = rem - den1 * qhat;
   c1 = qhat * den0;
   c2 = rhat * b;
   if (c1 > c2)
      qhat -= (c1 - c2 > v) ? 2 : 1;
   q0 = qhat;

   return ((unsigned) q1 << 16) | q0;
}

// * alternative to __builtin_clz
unsigned nlzu(unsigned x) {
   unsigned n;

   n = 0;
   if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
   if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
   if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
   if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
   if (x <= 0x7FFFFFFF) {n = n + 1;}
   return n;
}

// Returns magic number for unsigned integer division by constant, input may not be 0 and must not be a power of 2
struct mul magicul(unsigned long d) {
   struct mul mag;
   unsigned long divu128(unsigned, unsigned long);
   mag.d = 0;

   char l2 = 63;
   unsigned p = 0;
   l2 -= __builtin_clzll(d);
   
   unsigned long pow = 1 << l2;    // 2**l2
   unsigned long m; 
   if (d < 0x100000000) {
      // no overflow in 64 bits 2**(64+l2)//d + 1 if d is less than 2**32
      const unsigned long two63 = 0x8000000000000000; // 2**63
      unsigned long f = two63/d;
      m = f * (pow << 1) + ((two63 - (f*d)) * (pow << 1))/d + 1;
   } else {
      m = divu128(l2, d) + 1;
   }
   if ((m * d) >= pow) {      // m*d % 2**N >= 2**l2
      if (!(d & 1)) {
         l2++;
         do {
            d >>= 1;
            p++;
            l2 -= 2;
         } while (!(d & 1) && l2 > 0);
         m = (m + (1 << (p - 1)) - 1) >> (p - 1); // m = ceil(m/2**(p-1))
         if (l2 < 0) {
            m <<= 1;
            l2++;
         }
      } else {
         m--;
         mag.d = 1;
      }
   }
   while (!(m & 1) && l2) {
      m >>= 1;
      l2--;
   }
   mag.M = m;
   mag.p = p;
   mag.s = l2;
   return mag;
}

// Divides unsigned 2**u * (2**64) by v without overflowing in 64 bits, Knuth's Algorithm D
unsigned long divu128(unsigned u, unsigned long v) {
   // We work in base 2**32.
   // A uint32 holds a single digit. A uint64 holds two digits.
   // Our numerator is conceptually [num3, num2, num1, num0].
   // Our denominator is [den1, den0].
   const unsigned long b = (1ull << 32);

   // The high 64 bits of the dividend
   unsigned long numhi = (1ull << u);

   // The high and low digits of our computed quotient.
   unsigned q1;
   unsigned q0;

   // The normalization shift factor.
   int shift;

   // The high and low digits of our denominator (after normalizing).
   unsigned den1;
   unsigned den0;

   // A partial remainder.
   unsigned long rem;

   // The estimated quotient, and its corresponding remainder (unrelated to true remainder).
   unsigned long qhat;
   unsigned long rhat;

   // Variables used to correct the estimated quotient.
   unsigned long c1;
   unsigned long c2;

   // Check for overflow and divide by 0, should be unreachable in this use case
   if (numhi >= v) {
      return ~0ull;
   }

   // Determine the normalization factor. We multiply den by this, so that its leading digit is at
   // least half b. In binary this means just shifting left by the number of leading zeros, so that
   // there's a 1 in the MSB.
   // We also shift numer by the same amount. This cannot overflow because numhi < den.
   // The expression (-shift & 63) is the same as (64 - shift), except it avoids the UB of shifting
   // by 64. The funny bitwise 'and' ensures that numlo does not get shifted into numhi if shift is 0.
   // clang 11 has an x86 codegen bug here: see LLVM bug 50118. The sequence below avoids it.
   shift = __builtin_clzll(v);
   v <<= shift;
   numhi <<= shift;

   // Extract the low digits of the numerator and both digits of the denominator.
   den1 = (unsigned) (v >> 32);
   den0 = (unsigned) (v & 0xFFFFFFFFu);

   // We wish to compute q1 = [n3 n2 n1] / [d1 d0].
   // Estimate q1 as [n3 n2] / [d1], and then correct it.
   // Note while qhat may be 2 digits, q1 is always 1 digit.
   qhat = numhi / den1;
   rhat = numhi - den1 * qhat;
   c1 = qhat * den0;
   c2 = rhat * b;
   if (c1 > c2)
      qhat -= (c1 - c2 > v) ? 2 : 1;
   q1 = qhat;

   // Compute the true (partial) remainder.
   rem = (numhi << 32) - q1 * v;

   // We wish to compute q0 = [rem1 rem0 n0] / [d1 d0].
   // Estimate q0 as [rem1 rem0] / [d1] and correct it.
   qhat = rem / den1;
   rhat = rem - den1 * qhat;
   c1 = qhat * den0;
   c2 = rhat * b;
   if (c1 > c2)
      qhat -= (c1 - c2 > v) ? 2 : 1;
   q0 = qhat;

   return ((unsigned long) q1 << 32) | q0;
}

// * alternative to __builtin_clzll
unsigned long nlzl(unsigned long x) {
   unsigned long n;

   n = 0;
   if (x <= 0x00000000FFFFFFFF) {n = n +32; x = x <<32;}
   if (x <= 0x0000FFFFFFFFFFFF) {n = n +16; x = x <<16;}
   if (x <= 0x00FFFFFFFFFFFFFF) {n = n + 8; x = x << 8;}
   if (x <= 0x0FFFFFFFFFFFFFFF) {n = n + 4; x = x << 4;}
   if (x <= 0x3FFFFFFFFFFFFFFF) {n = n + 2; x = x << 2;}
   if (x <= 0x7FFFFFFFFFFFFFFF) {n = n + 1;}
   return n;
}
}