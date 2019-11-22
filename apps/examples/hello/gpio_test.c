#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>

#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>

static struct timeval tm1;
static struct timeval tm2;

static void gpio_cb(iotbus_gpio_context_h dev)
{
	(void)gettimeofday(&tm2, NULL);
	printf("%d pin Interrupt Occured!\n", iotbus_gpio_get_pin(dev));
	printf("tm1 : %d.%d\n", tm1.tv_sec, tm1.tv_usec);
	printf("tm2 : %d.%d\n", tm2.tv_sec, tm2.tv_usec);
	return;
}

int gpio_test_main(int argc, char *argv[])
{
	int ret;
	int i;

	printf("GPIO INT TEST\n");

	iotbus_gpio_context_h gpio1 = iotbus_gpio_open(15);
	iotbus_gpio_context_h gpio2 = iotbus_gpio_open(19);

	ret = iotbus_gpio_set_direction(gpio1, IOTBUS_GPIO_DIRECTION_IN);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Fail to set direction1\n");
		return 0;
	}
	ret = iotbus_gpio_set_direction(gpio2, IOTBUS_GPIO_DIRECTION_OUT);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Fail to set direction2\n");
		return 0 ;
	}
	
	iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
	usleep(100 * 1000);
	iotbus_gpio_set_interrupt(gpio1, IOTBUS_GPIO_EDGE_FALLING, gpio_cb, 224);

	for (i = 0; i < 10; i++) {
		iotbus_gpio_write(gpio2, IOTBUS_GPIO_HIGH);
		usleep(10 * 1000);
		ret = iotbus_gpio_read(gpio1);
		dbg("val1 : %d\n", ret);
		(void)gettimeofday(&tm1, NULL);
		iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
		usleep(10 * 1000);
		ret = iotbus_gpio_read(gpio1);
		dbg("val2 : %d\n", ret);
	}

	// iotbus_gpio_unset_interrupt(gpio1, IOTBUS_GPIO_FALLING);

	// for (i = 0; i < 10; i++) {
	// 	iotbus_gpio_write(gpio2, IOTBUS_GPIO_HIGH);
	// 	usleep(100 * 1000);
	// 	iotbus_gpio_write(gpio2, IOTBUS_GPIO_LOW);
	// 	usleep(100 * 1000);
	// }

    iotbus_gpio_close(gpio1);
	iotbus_gpio_close(gpio2);

	return 0;
}