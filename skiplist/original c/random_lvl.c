#include <stdlib.h>
#include <stdio.h>



#define MAX_LVL_COUNT 16
#define MAX_LEVEL (MAX_LVL_COUNT - 1)
#define BITS_IN_RANDOM 31

static int random_bits = 10;
static int randoms_left = BITS_IN_RANDOM / 2;

int random_lvl(void)
{
	register int level = 0;
	register int b;

	do
	{
		b = random_bits & 3;
		if(!b)
		{
			level++;
		}
		random_bits >>= 2;

		if(--randoms_left == 0)
		{
			random_bits = rand();
			randoms_left = BITS_IN_RANDOM / 2;
		}
	} while(!b);

	return (level > MAX_LEVEL ? MAX_LEVEL : level);
};


int main()
{
	int first_call = random_lvl();

	printf("first_call:\t%d\n", first_call);
	printf("random_bits:\t%d\n", random_bits);
	printf("randoms_left:\t%d\n", randoms_left);

	return 0;
}
