#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iotbus/iotbus_gpio.h>

struct timeval tv1;
struct timeval tv2;

static void test_cb1(iotbus_gpio_context_h hnd) {
    printf("Callback1!! [%p]\n", hnd);
}

static void test_cb2(iotbus_gpio_context_h hnd) {
    gettimeofday(&tv2, NULL);
    printf("tv1 : %ld.%ld\n", tv1.tv_sec, tv1.tv_usec);
    printf("tv2 : %ld.%ld\n", tv2.tv_sec, tv2.tv_usec);
    printf("Workqueue\n");
}

struct _iotbus_gpio_s {
	int pin;
	iotbus_gpio_drive_e drive;
	iotbus_gpio_direction_e dir;
	iotbus_gpio_edge_e edge;
	int fd;
	gpio_isr_cb isr_cb;
	void *ud;
};

struct _iotbus_gpio_wrapper_s {
	struct _iotbus_gpio_s *handle;
};

int int_test_main(void) {
    printf("INTR Test\n");

    iotbus_gpio_context_h m_gpio1 = iotbus_gpio_open(5);
	iotbus_gpio_context_h m_gpio2 = iotbus_gpio_open(9);

    iotbus_gpio_set_direction(m_gpio1, IOTBUS_GPIO_DIRECTION_IN);
	iotbus_gpio_set_direction(m_gpio2, IOTBUS_GPIO_DIRECTION_OUT);

    struct iotbus_int_info_s info1 = { m_gpio1, IOTBUS_GPIO, test_cb1, IOTBUS_GPIO_FALLING };
    struct iotbus_int_info_s info2 = { m_gpio2, IOTBUS_GPIO, test_cb1, IOTBUS_GPIO_RISING };
    struct iotbus_int_info_s info3 = { m_gpio1, IOTBUS_GPIO, test_cb1, IOTBUS_GPIO_FALLING };
    struct iotbus_int_info_s info4 = { m_gpio1, IOTBUS_GPIO, test_cb2, IOTBUS_GPIO_FALLING };
    struct iotbus_int_info_s info5 = { m_gpio2, IOTBUS_GPIO, test_cb1, IOTBUS_GPIO_FALLING };

    int fd;
    int ret;

    printf("test_cb1 : %p\n", test_cb1);
	printf("test_cb2 : %p\n", test_cb2);

    printf("Handler1-1 : %p\n", info1.handle);
	printf("Pin type1-1 : %d\n", info1.pin_type);
	printf("Int type1-1 : %s\n", iotbus_int_str[info1.int_type]);
    fflush(stdout);

    fd = open(IOTBUS_IRQPATH, O_NONBLOCK);
    if (fd < 0) {
        ret = get_errno();
        printf("Open IOTBUS fail[%d, %d]\n", fd, ret);
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_REGISTER, (unsigned long)&info1);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_REGISTER1 fail\n");
        return -1;
    }

    printf("Handler1-2 : %p\n", info2.handle);
	printf("Pin type1-2 : %d\n", info2.pin_type);
	printf("Int type1-2 : %s\n", iotbus_int_str[info2.int_type]);
    fflush(stdout);

    ret = ioctl(fd, IOTBUS_INTR_REGISTER, (unsigned long)&info2);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_REGISTER2 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_INFO, 0);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_INFO1 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_REGISTER, (unsigned long)&info3);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_REGISTER3 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_INFO, 0);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_INFO1 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_REGISTER, (unsigned long)&info4);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_REGISTER4 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_INFO, 0);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_INFO1 fail\n");
        return -1;
    }

    ret = ioctl(fd, IOTBUS_INTR_UNREGISTER, (unsigned long)&info2);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_UNREGISTER1 fail\n");
        return -1;
    }
    ;

    ret = ioctl(fd, IOTBUS_INTR_INFO, 0);
    if (ret < 0) {
        printf("Open IOTBUS_INTR_INFO2 fail\n");
        return -1;
    }

    // ret = ioctl(fd, IOTBUS_INTR_REGISTER, (unsigned long)&info5);
    // if (ret < 0) {
    //     printf("Open IOTBUS_INTR_REGISTER5 fail\n");
    //     return -1;
    // }

    // ret = ioctl(fd, IOTBUS_INTR_INFO, 0);
    // if (ret < 0) {
    //     printf("Open IOTBUS_INTR_INFO1 fail\n");
    //     return -1;
    // }

    int fd2 = (struct _iotbus_gpio_s *)(m_gpio1->handle)->fd;
    ret = ioctl(fd2, GPIOIOC_SET_INTERRUPT, GPIO_EDGE_FALLING);

    sleep(1);

    // iotbus_gpio_write(m_gpio2, IOTBUS_GPIO_HIGH);
    // gettimeofday(&tv1, NULL);
    // iotbus_gpio_write(m_gpio2, IOTBUS_GPIO_LOW);
    ret = iotbus_gpio_read(m_gpio1);
    if (ret < 0) {
        printf("Error GPIO READ0\n");
    } else {
        printf("Read0 : %d\n", ret);
        fflush(stdout);
    }

	for (int i = 0; i < 10; i++) {
		iotbus_gpio_write(m_gpio2, IOTBUS_GPIO_HIGH);
        usleep(10 * 1000);
        ret = iotbus_gpio_read(m_gpio1);
        if (ret < 0) {
            printf("Error GPIO READ1\n");
        } else {
            printf("Read1 : %d\n", ret);
            fflush(stdout);
        }
        usleep(100 * 1000);
        gettimeofday(&tv1, NULL);
        iotbus_gpio_write(m_gpio2, IOTBUS_GPIO_LOW);
        usleep(10 * 1000);
        ret = iotbus_gpio_read(m_gpio1);
        if (ret < 0) {
            printf("Error GPIO READ1\n");
        } else {
            printf("Read2 : %d\n", ret);
            fflush(stdout);
        }
        usleep(300 * 1000);
	}

    iotbus_gpio_close(m_gpio1);
    iotbus_gpio_close(m_gpio2);

    return 0;
}