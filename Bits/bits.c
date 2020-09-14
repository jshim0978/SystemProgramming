/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * Name : 심정우
 * Number : 201502071
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each fun:ction with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */

	 
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
	
	int result = ~(~x|~y);
	//대우
	
	
	return result;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
	//8배후 나머지비트 0으로

	return  (x>>(n<<3)) & 0xFF;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  
	x = x >> n;

	n = 32 + ~n;

	int AllOnes = ((~0x0) << n) <<1;
	// 4바이트를 잡아 모두 1로 만든뒤 왼쪽으로 n+1만큼 이동
	x = (x|AllOnes) ^ AllOnes;
	return x;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {

	int a = 0x01;
	int count = 0;
	count = count + (x&a);//x에 대해 이동시키면서 1이 있는지 계속 확인하여 카운트함
	count = count + (x >> 1 & a);
	count = count + (x >> 2 & a);
	count = count + (x >> 3 & a);
	count = count + (x >>4&a);
	count = count + (x >> 5 &a);
	count = count + (x >> 6 & a);
	count = count + (x >> 7& a);
	count = count + (x >> 8 & a);
	count = count + (x >> 9 & a);
	count = count + (x >> 10 & a);
	count = count + (x >> 11 & a);
	count = count + (x >> 12 & a);
	count = count + (x >> 13 & a);
	count = count + (x >> 14 & a);
	count = count + (x >> 15 & a);
	count = count + (x >> 16 & a);
	count = count + (x >> 17 & a);
	count = count + (x >> 18 & a);
	count = count + (x >> 19 & a);
	count = count + (x >> 20 & a);
	count = count + (x >> 21 & a);
	count = count + (x >> 22 & a);
	count = count + (x >> 23 & a);
	count = count + (x >> 24 & a);
	count = count + (x >> 25 & a);
	count = count + (x >> 26 & a);
	count = count + (x >> 27 & a);
	count = count + (x >> 28 & a);
	count = count + (x >> 29 & a);
	count = count + (x >> 30 & a);
	count = count + (x >> 31 & a);
	
	return count ;
}
/*
 * isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
  	int		result =(!x) ;
	//0 일때만 1이나오게
	return  result;
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {

	//xor를활용해 같은 비트를 체크 다같다면 모든 비트가 0이 다나올거고 거기에 not을 붙여1이 나오게한다
	int result  = !(x^y);
	return result;
}

/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  
	int shifter = 33 + (~n);
	//몇비트 이동할지 결정
	int result = x<<shifter;
	//비트를 이동했다가
	result = result >>shifter;
	//되돌림
	return !(result^x);//isEqual을 활용해 같은지 확인. 같다면 n비트 표현가능 ;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  

	int xsign = (x >> 31) & 1;//x의 사인비트를 확인하여 x가 양수인지 음수인지 판단
	int ysign= (y >> 31) & 1;//같은 맥락
	int isXNegative = ((!ysign)) & (xsign);//사인비트가 같은지 확인하여 같은 양수인지음수인지판단
	int isLess = (!(ysign^xsign)) & (((x + ~y) >> 31) & 1); 
	//x와 y가 둘다 양수인지 둘다 음수인지를 판단하는 부분이 앞부분
	//뒷부분은 x<y인지 판단 
	//뒷부분은 스스로 해결이 힘들어 찾아보았습니다
	//그러고 앞부분이 1일때 1을 isLess가 1을 가지게하여 
	return isXNegative | isLess;
	//&를 활용해 x만 음수이면 무조건 1 이고 둘다 같은 사인비트를 가지고 x<=y이면 1을 리턴 
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
	
	int AllOnesExceptSign = (0x1 << 31 ) + (~0);


  	return x << n | (x >> (32 + (~n+1)) & AllOnesExceptSign >> (31 + (~n+1)))	;
	//x << n : 이동한 비트는 생략한 회전된 비트
	//32 + (~n + 1 ) : 보수 +1 &를 활용해 1만있는 변수를 활용해 이동한 x를 찾아서
	//둘을 | 로 합친다.
}
