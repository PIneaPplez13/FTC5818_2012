/*	DYNAMIC MEMORY MODULE
		Implements basic dynamic memory
		See stdbot docs for more info on how to use
*/

byte __dynMem[1024];	//	This is the dynamic memory array
											//	Do *NOT* interface directly with it

int __lastAvail = 0;

byte* malloc(void* data, int size);
byte* realloc(void* data, int size);
void free(void* data, int size);
byte* searchMem(int size);

byte* malloc(byte* data, int size)	{
	//	Returns a pointer to the memory location
	__lastAvail += size/8;
	byte *ptr = __dynMem[__lastAvail];
	return ptr;
}
