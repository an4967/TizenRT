#include <tinyara/config.h>
#include <stdio.h>

#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>

static int gpio_num[8] = {16, 4, 5, 15, 14, 27, 12, 13};
static iotbus_gpio_context_h gpios[8];

static int digit[10][8] = {
	{0, 1, 1, 1, 0, 1, 1, 1}, // 0
	{0, 0, 0, 1, 0, 0, 0, 1}, // 1
	{0, 1, 1, 0, 1, 0, 1, 1}, // 2
	{0, 0, 1, 1, 1, 0, 1, 1}, // 3
	{0, 0, 0, 1, 1, 1, 0, 1}, // 4
	{0, 0, 1, 1, 1, 1, 1, 0}, // 5
	{0, 1, 1, 1, 1, 1, 1, 0}, // 6
	{0, 0, 0, 1, 0, 1, 1, 1}, // 7
	{0, 1, 1, 1, 1, 1, 1, 1}, // 8
	{0, 0, 1, 1, 1, 1, 1, 1}, // 9
};

static int set_digit(int num)
{
	int i;

	if (num < 0 || num > 9) {
		return -1;
	}

	for (i = 1; i < 8; i++) {
		if (digit[num][i] == 1) {
			iotbus_gpio_write(gpios[i], IOTBUS_GPIO_LOW);
		} else {
			iotbus_gpio_write(gpios[i], IOTBUS_GPIO_HIGH);
		}
	}

	return 0;
}

int gpio_test_main(int num)
{

	int i;
	int cnt = 8;
	int ret;
	int flag = 0;

	printf("GPIO TEST\n");

	for (i = 0; i < cnt; i++) {
		gpios[i] = iotbus_gpio_open(gpio_num[i]);

		ret = iotbus_gpio_set_direction(gpios[i], IOTBUS_GPIO_DIRECTION_OUT);
		if (ret != IOTBUS_ERROR_NONE) {
			printf("Fail to set direction[%d] : %d\n", ret, gpio_num[i]);
			flag = 1;
		}

		iotbus_gpio_write(gpios[i], IOTBUS_GPIO_HIGH);
		usleep(10 * 1000);
	}

	if (flag == 1) {
		return -1;
	}

	if (num == -1) {
		for (i = 0; i < cnt; i++) {
			iotbus_gpio_write(gpios[i], IOTBUS_GPIO_LOW);
			usleep(500 * 1000);
			iotbus_gpio_write(gpios[i], IOTBUS_GPIO_HIGH);
			usleep(10 * 1000);
		}

		for (i = 0; i < 10; i++) {
			set_digit(i);
			if (i == 9) {
				i = -1;
			}
			usleep(500 * 1000);
		}
	} else {
		set_digit(num);
	}

	for (i = 0; i < cnt; i++) {
		iotbus_gpio_close(gpios[i]);
	}

	printf("GPIO DONE\n");

	return 0;
}