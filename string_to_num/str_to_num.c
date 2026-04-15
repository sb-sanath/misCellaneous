#include <stdio.h>
#include <stdint.h>

int main (void) {
	char *number_as_string = "526";
	int number_as_num = 0;
	uint8_t temp;

	while (*number_as_string != '\0') {
		temp = *number_as_string - '0';
		number_as_num = (number_as_num * 10) + temp;
		number_as_string ++;
	}

	printf("%d\n", number_as_num);

	return 0;

}

