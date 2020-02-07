#include <tinyara/config.h>
#include <stdio.h>

#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>

static iotbus_gpio_context_h button[2];

static int gpio_num[8] = {16, 4, 5, 15, 14, 27, 12, 13};
static iotbus_gpio_context_h gpios[8];
static int digit_cnt = 8;
static int num = 0;

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

static int digit_init() {
	int i;
	int ret;

	for (i = 0; i < digit_cnt; i++) {
		gpios[i] = iotbus_gpio_open(gpio_num[i]);

		ret = iotbus_gpio_set_direction(gpios[i], IOTBUS_GPIO_DIRECTION_OUT);
		if (ret != IOTBUS_ERROR_NONE) {
			printf("Fail to set direction[%d] : %d\n", ret, gpio_num[i]);
		}

		iotbus_gpio_write(gpios[i], IOTBUS_GPIO_HIGH);
	}
	set_digit(num);

	return 0;
}

static void gpio_event_callback(void *user_data)
{
	if (++num > 9) {
		num = 0;
	}
	set_digit(num);

	return;
}

static int button_init() {
	int ret;

	button[0] = iotbus_gpio_open(18);
	ret = iotbus_gpio_set_direction(button[0], IOTBUS_GPIO_DIRECTION_IN);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Fail to set direction : %d\n", ret);
		return -1;
	}

	printf("Press Test Button!!!\n");

	ret = iotbus_gpio_register_cb(button[0], IOTBUS_GPIO_EDGE_RISING, gpio_event_callback, (void *)button[0]);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Registering BUBTTON Callback Error!\n");
		return -1;
	}

	return 0;
}

int gpio_test_main2()
{
	printf("GPIO2 TEST\n");

	iotapi_initialize();
	digit_init();
	button_init();

	return 0;

}