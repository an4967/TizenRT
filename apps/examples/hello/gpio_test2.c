#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>

#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>

// static struct timeval tm1;
// static struct timeval tm2;

iotbus_gpio_context_h gpio1;
iotbus_gpio_context_h gpio2;

static void gpio_cb(iotbus_gpio_context_h dev)
{
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_HIGH);
	return;
}

int gpio_test_main2(int argc, char *argv[])
{
	int ret;
	int i;

	printf("GPIO2 INT TEST\n");

	gpio1 = iotbus_gpio_open(9);
	gpio2 = iotbus_gpio_open(5);

	ret = iotbus_gpio_set_direction(gpio1, IOTBUS_GPIO_DIRECTION_IN);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Fail to set direction1\n");
		return 0;
	}
	ret = iotbus_gpio_set_direction(gpio2, IOTBUS_GPIO_DIRECTION_OUT);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Fail to set direction2\n");
		return 0;
	}
	
	int cnt = 5;
	// init;
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_HIGH);
	usleep(20 * 1000);
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_HIGH);
	iotbus_gpio_set_interrupt(gpio1, IOTBUS_GPIO_EDGE_FALLING, gpio_cb, 251);
	usleep(100 * 1000);
	for (i = 0; i < cnt; i++) {
		usleep(30 * 1000);
		iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
		usleep(30 * 1000);
	}
	usleep(20 * 1000);
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);

    iotbus_gpio_close(gpio1);
	iotbus_gpio_close(gpio2);

	printf("GPIO2 DONE\n");

	return 0;
}