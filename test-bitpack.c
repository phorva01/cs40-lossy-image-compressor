

#include <stdlib.h>
#include <stdio.h>
#include <bitpack.h>

int main()
{
	uint64_t mama = 0;

	printf("Packing: %lu\n", (uint64_t)27);
	mama = Bitpack_newu(mama, 9, 23, (uint64_t)27);
	printf("Retrieved: %lu\n", Bitpack_getu(mama, 5, 23));

	printf("Packing %ld with width 5 at lsb 0\n", (int64_t)-4);
	mama = Bitpack_news(mama, 5, 0, (int64_t)-4);
	printf("Retrieved: %ld\n", Bitpack_gets(mama, 5, 0));


	// printf("Packing: %lu\n", (int64_t)8);
	// mama = Bitpack_news(mama, 5, 8, (int64_t)8);
	// printf("Retrieved: %lu\n", Bitpack_gets(mama, 5, 8));

	// printf("Packing: %lu\n", (int64_t)8);
	// mama = Bitpack_news(mama, 5, 8, (int64_t)8);
	// printf("Retrieved: %lu\n", Bitpack_gets(mama, 5, 8));

	// printf("Packing: %lu\n", (int64_t)8);
	// mama = Bitpack_news(mama, 5, 8, (int64_t)8);
	// printf("Retrieved: %lu\n", Bitpack_gets(mama, 5, 8));

	// printf("Packing: %lu\n", (int64_t)8);
	// mama = Bitpack_news(mama, 5, 8, (int64_t)8);
	// printf("Retrieved: %lu\n", Bitpack_gets(mama, 5, 8));

	// printf("Packing: %ld\n", (int64_t)-5);
	// mama = Bitpack_news(mama, 4, 30, (int64_t)-5);
	// printf("Retrieved: %ld\n", Bitpack_gets(mama, 4, 30));

	// printf("Packing: %ld\n", (int64_t)-4);
	// mama = Bitpack_news(mama, 4, 30, (int64_t)-4);
	// printf("Retrieved: %ld\n", Bitpack_gets(mama, 4, 30));

	// if (Bitpack_gets(Bitpack_news(mama, 6, 50, (int64_t)-8), 6, 50) == (int64_t)-8) {
	// 	printf("ya did it\n");
	// } else {
	// 	printf("nice try, rascals!\n");
	// }

	// if (Bitpack_gets(Bitpack_news(mama, 6, 50, (int64_t)-8), 6, 57) == 
	// 		Bitpack_gets(mama, 6, 57)) {
	// 	printf("ya did it again\n");
	// } else {
	// 	printf("nice try, rascals! twice\n");
	// }

	return EXIT_SUCCESS;
}
