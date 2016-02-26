#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Header to bit-pack
#include "icon.h"

int main(int argc, char **argv) {
	uint8_t data;
	char *tmp;
	int i, j;
	
	printf("{\n\t");
	for(i = 0; i < height; i++) {
		tmp = header_data + i*width + width;
		data = 0;
		for(j = 0; j < width; j++) {
			data <<= 1;
			data |= *--tmp;
		}
		printf("%hhu,", data);
		if(!((i + 1) % 8))
			printf("\n\t");
		else
			printf(" ");
	}
	printf("}\n");
	
	return 0;
}
