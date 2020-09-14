/*
 * mm-implicit.c - an empty malloc package
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 *
 * @id : 201502071
 * @name : 심정우 
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define SIZE_PTR(p) ((size_t*)(((char*)(p)) - SIZE_T_SIZE))


/* $begin mallocmacros */
#define WSIZE 		4
#define DSIZE 		8
#define OVERHEAD	8
#define CHUNKSIZE	(1<<12)
#ifdef DEBUG
#define CHECKHEAP(verbvose) printf("%s\n", __func__); mm_checkheap(verbose);
#endif
#define MAX(x, y)	((x) > (y) ? (x) : (y))

#define PACK(size, alloc)	((size) | (alloc))

#define GET(p)		(*(unsigned int *) (p))
#define PUT(p, val)	(*(unsigned int *) (p) = (val))

#define GET_SIZE(p)		(GET(p) & ~0x7)
#define GET_ALLOC(p)	(GET(p) & 0x1)

#define HDRP(bp)	((char *)(bp) - WSIZE)
#define FTRP(bp)	((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)


#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))
    //implicit.pdf 파일을 참고하시오.
    //매크로 함수를 작성하시오.

/* $end mallocmacros */


/* function prototypes for internal helper routines */
static void *coalesce(void *bp);
static void place(void *bp, size_t asize);
static void *extend_heap(size_t words);
static void *find_fit(size_t asize);

/* Global variables */
static char *heap_listp = 0;
static char *next_heap;

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {

	if((heap_listp = mem_sbrk(4*WSIZE))== (void *) -1)//초기 empty heap 생성
				return -1;

	PUT(heap_listp,0);//Alignment Padding(정렬을 위해서 의미없는 값을 삽입)
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));//prologue header
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));//prologue footer
	PUT(heap_listp + (3*WSIZE), PACK(0, 1));	//epilogue header
	heap_listp += (2*WSIZE);
	next_heap = heap_listp;		//next-fit 구현용도

	if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;
    return 0;
}

/*
 * malloc
 */
void *malloc (size_t size) {

   	size_t asize; /* Adjusted block size */
	size_t extendsize; /* Amount to extend heap if no fit */
	char *bp;

	/* Ignore spurious requests */
	/* = 가짜 리퀘스트 무시 */
	if (size == 0)
		return NULL;
	
	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= DSIZE)
		asize = 2*DSIZE;
	else 
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	/* Search the free list for a fit */
	if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	/* No fit found. Get more memory and place the block */

	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;
	place(bp, asize);
	return bp;

}

/*
 * free
 */
void free (void *bp) {
	if (!bp) return;

	size_t size = GET_SIZE(HDRP(bp));

	//REMOVE_RATAG(HDRP(NEXT_BLKP(bp)));
	
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));

	//insert_node(bp, size);
	coalesce(bp);

	//return;
    //implicit.pdf 파일을 참고하시오.

}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
  size_t oldsize;
  void *newptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if(size == 0) {
    free(oldptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if(oldptr == NULL) {
    return malloc(size);
  }

  newptr = malloc(size);

  /* If realloc() fails the original block is left untouched  */
  if(!newptr) 
	{
    		return 0;
	}

	/* Copy the old data. */
	oldsize = *SIZE_PTR(oldptr);
	if(size < oldsize) 
		oldsize = size;
	memcpy(newptr, oldptr, oldsize);
										
	/* Free the old block. */
	free(oldptr);
											
	return newptr;
}
/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
    
	size_t bytes = nmemb * size;
	void *newptr;

	newptr = malloc(bytes);
	memset(newptr, 0, bytes);

	return newptr;
}
/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
    return p < mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int verbose) {
}


static void *coalesce(void *bp) 
{
    //implicit.pdf 파일을 참고하시오.
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if(prev_alloc && next_alloc){
	
		return bp;

	}
	else if (prev_alloc && !next_alloc) {
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));

	}
	else if (!prev_alloc  && next_alloc) {
		
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		
		PUT(FTRP(bp),PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	else {

		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
		
		
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	if(next_heap > (char*)bp && (next_heap < NEXT_BLKP(bp)))
		next_heap = bp;
	return bp;
}


static void *extend_heap(size_t words) 
{

	//implicit.pdf 파일을 참고하시오.
	char *bp;
	size_t size;

	size = (words%2) ? (words+1) * WSIZE : words * WSIZE;
	if((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	PUT(HDRP(bp), PACK(size,0));
	PUT(FTRP(bp), PACK(size,0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0,1));

	return coalesce(bp);

			
	
}


static void place(void *bp , size_t asize){
    //실습 자료를 참고하시오.    
	size_t csize = GET_SIZE(HDRP(bp));

	if((csize - asize) >= (2*DSIZE)){
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));
	}
	else{
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

static void *find_fit(size_t asize){
  	
	void *bp;
	//next-fit에 사용될 가장 최근에 접근한 블록
	char *old_heap = next_heap;
	
	//first-fit 
	/*
	for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
		if(!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
			return bp;
		}
	}
	
	*/
	//next-fit
	
	for( ; GET_SIZE(HDRP(next_heap)) > 0; next_heap = NEXT_BLKP(next_heap)){
		if(!GET_ALLOC(HDRP(next_heap)) && (asize <= GET_SIZE(HDRP(next_heap)))){
			return next_heap;
		}
	}

	for(next_heap = heap_listp; next_heap < old_heap; next_heap = NEXT_BLKP(next_heap)){
		 if(!GET_ALLOC(HDRP(next_heap)) && (asize <= GET_SIZE(HDRP(next_heap)))){
            		 return next_heap;
         	}
	}
	
	return NULL;
}
