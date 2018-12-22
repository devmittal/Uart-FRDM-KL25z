/*****************************************************************************
​ ​* ​ ​ @file​ ​  buffer.h
​ * ​ ​ @brief​ ​ Contains the functions prototypes, structure and global
 *           variable declaration essential for buffer.c
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"

/* Structure for CIrcular Buffer */
typedef struct
{
	char *Buffer;
	int Length;
	int Ini;	//Rear
	int Outi;	//front
} ring_t;

/* Variable which holds the receiver circular buffer */
ring_t *ringrx;
/* Variable which holds the transmitter circular buffer */
ring_t *ringtx;

volatile unsigned int fib; //Stores next element of the fibonacci series
int flag;

/**
* @Brief  This function initializes the transmitter Circular buffer.
*
* @Param  void
*
* @Return void
**/
void inita();

/**
* @Brief  This function initializes the receiver Circular buffer.
*
* @Param  void
*
* @Return void
**/
void initb();
/**
* @Brief  This function inserts data into the selected FIFO Circular buffer.
*
* @Param  Reference of the circular buffer and, data to be inserted.
*
* @Return void
**/
void insert_data(ring_t *ring, char data);

/**
* @Brief  This function removes data from the selected FIFO Circular buffer.
*
* @Param  Reference of the circular buffer.
*
* @Return Data which is removed.
**/
int remove_data(ring_t *ring);

/**
* @Brief  This function checks if the buffer is empty.
*
* @Param  Reference of the circular buffer.
*
* @Return 1 or 0 depending on whether it is empty or not.
**/
int isempty(ring_t *ring);

/**
* @Brief  This function checks if the buffer is full.
*
* @Param  Reference of the circular buffer.
*
* @Return 1 or 0 depending on whether it is full or not.
**/
int isfull(ring_t *ring);

/**
* @Brief  This function calculates the occurences of each character input by user.
*
* @Param  void
*
* @Return void
**/
void report();
