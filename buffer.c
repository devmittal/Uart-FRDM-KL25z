/*****************************************************************************
​ ​* ​ ​ @file​ ​  buffer.c
​ * ​ ​ @brief​ ​ This function carries out different operations on the circular
 *		     buffer like insert, remove and check if it is empty or full.
 *		     It also generates the report.
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "buffer.h"

/* Initializes ring buffer */
void inita()
{
	/* Allocate memory for the structure and character array (buffer) dynamically and process if successful */
	if((ringtx = malloc(1000 + sizeof(*ringtx))) != NULL)
	{
		/* Initialize all structure parameters */
		ringtx->Length = 1000;
		/* Set Rear (insertion - Ini) and Front (Deletion - Outi) Markers  to default */
		ringtx->Ini = 0;
		ringtx->Outi = 0;
		/* Point to space reserved for buffer */
		ringtx->Buffer = (char *)(ringtx + 1);
	}
}

/* Initializes ring buffer */
void initb()
{
	/* Allocate memory for the structure and character array (buffer) dynamically and process if successful */
	if((ringrx = malloc(10000 + sizeof(*ringrx))) != NULL)
	{
		/* Initialize all structure parameters */
		ringrx->Length = 10000;
		/* Set Rear (insertion - Ini) and Front (Deletion - Outi) Markers  to default */
		ringrx->Ini = 0;
		ringrx->Outi = 0;
		/* Point to space reserved for buffer */
		ringrx->Buffer = (char *)(ringrx + 1);
	}
}

/* To insert data in the circular buffer */
void insert_data(ring_t *ring, char data)
{
	/* Insert data in buffer and conform index within buffer length */
	ring->Buffer[ring->Ini++ % ring->Length] = data;
}

/* To remove data from the FIFO Circular buffer */
int remove_data(ring_t *ring)
{
	/* Remove single data and conform the out marker within buffer length */
	return ring->Buffer[ring->Outi++ % ring->Length];
}

/* Check if buffer is empty */
int isempty(ring_t *ring)
{
	return ring->Outi == ring->Ini;
}

/* Check if buffer is full */
int isfull(ring_t *ring)
{
	return ring->Ini-ring->Outi == ring->Length;
}

void report()
{
	char str[1000];
	unsigned int character_parser = 0, buffer_parser = 0, occurance_counter = 0;

	sprintf(str, "\r\n%u",fib);
	uart_write(str); //print next element of the fibonacci series
	uart_write("\n\rStats:");

	/* Loop for each of the 256 characters */
	for(character_parser = 0 ; character_parser < 256 ; character_parser++)
	{
		/* Execute the following statements if overlap has not occurred */
	if((ringrx->Ini % ringrx->Length) > (ringrx->Outi % ringrx->Length))
	{
		    /* Traverse the entire contents of the buffer and check if the character is equal to any data within the buffer */
			for(buffer_parser = ringrx->Outi; buffer_parser < ringrx->Ini; buffer_parser++)
			{
				if((unsigned int)ringrx->Buffer[buffer_parser % ringrx->Length] == character_parser)
				{
					occurance_counter++; //If that character is contained in the buffer, increase its count
				}
			}
	}
	    /* Execute the following statements if overlap has occurred */
		else
		{
			/* Traverse buffer from Initial Position (Zero index) to In Marker */
			for(buffer_parser = 0; buffer_parser < (ringrx->Ini % ringrx->Length); buffer_parser++)
			{
				if((unsigned int)ringrx->Buffer[buffer_parser % ringrx->Length] == character_parser)
				{
					occurance_counter++; //If that character is contained in the buffer, increase its count
				}
			}
			/* Traverse buffer from Out Marker until the penultimate index of selected buffer (Index of Length - 1) */
			for(buffer_parser = (ringrx->Outi % ringrx->Length); buffer_parser < ringrx->Length; buffer_parser++)
			{
				if((unsigned int)ringrx->Buffer[buffer_parser % ringrx->Length] == character_parser)
				{
					occurance_counter++; //If that character is contained in the buffer, increase its count
				}
			}
		}
		buffer_parser = 0;

		/* If occurrence of the character is greater than 0, print it */
	if(occurance_counter != 0)
	{
		sprintf(str, "\r\n%c - %u",(char)character_parser , occurance_counter);
		uart_write(str);
		occurance_counter = 0; //Reset counter back to 0
	}
	}
}
