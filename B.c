#include <stdio.h>
#include <string.h>

union Data {
	unsigned char str[10];
    int i;
	float f;
};

int main() {
	union Data data;

	data.str[0] = 0x63;
	data.str[1] = 0x00;
	data.str[2] = 0x00;
	data.str[3] = 0x00;

	printf("data.i : %d\n", data.i);

	printf("data.f : %e\n", data.f);

	printf("data.str : %s\n", data.str);

	return 0;
}

