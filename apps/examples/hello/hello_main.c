/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include <fcntl.h>
#include <tinyara/i2c.h>
#include <tinyara/gpio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/

static int gpio_read(int port)
{
	char buf[4];
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("fd open fail\n");
		return -1;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
	if (read(fd, buf, sizeof(buf)) < 0) {
		printf("read error\n");
		close(fd);
		return -1;
	}
	close(fd);

	return buf[0] == '1';
}

static void gpio_write(int port, int value)
{
	char buf[4];
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("fd open fail\n");
		return;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	if (write(fd, buf, snprintf(buf, sizeof(buf), "%d", !!value)) < 0) {
		printf("write error\n");
	}
	close(fd);
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	int i;

	int gpio_out = 21;
	int gpio_in = 15;
	
	printf("GPIO TEST!\n");

	int val = 1;
	for (i = 0;i < 5; i++) {
		if (val == 0) val = 1;
		else val = 0;

		gpio_write(gpio_out, val);
		sleep(1);

		int r_read = gpio_read(gpio_in);

		printf("I(%d)->%d : %d\n", i, val, r_read);
	}

	return 0;
}
