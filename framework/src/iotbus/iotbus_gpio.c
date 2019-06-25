/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file iotbus_gpio.c
 * @brief Iotbus APIs for Gpio
 */

#include <tinyara/config.h>
#include <tinyara/gpio.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#include <iotbus/iotbus_common.h>
#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>
#include "iotapi_evt_handler.h"

#ifndef CONFIG_IOTBUS_GPIO_INTERRUPT_PRIORITY
#define CONFIG_IOTBUS_GPIO_INTERRUPT_PRIORITY 224
#endif

/**
 * @brief Struct for iotbus_gpio_s
 */
struct _iotbus_gpio_s {
	int pin;
	iotbus_gpio_drive_e drive;
	iotbus_gpio_direction_e dir;
	iotbus_gpio_edge_e edge;
	int fd;
	gpio_isr_cb isr_cb;
	void *ud;
	/* For Signal Callback */
	iotbus_gpio_cb cb;
	pid_t pid;
};

struct _iotbus_gpio_wrapper_s {
	struct _iotbus_gpio_s *handle;
};

#ifdef __cplusplus
extern "C" {
#endif

void gpio_async_handler(void *data)
{
	struct _iotbus_gpio_s *handle = (struct _iotbus_gpio_s *)data;
	handle->isr_cb(handle->ud);

	return;
}

/**
 * @brief Initializes gpio_context, based on Gpio pin.
 */
iotbus_gpio_context_h iotbus_gpio_open(int gpiopin)
{
	int fd;
	char gpio_dev[16] = { 0, };
	struct _iotbus_gpio_s *handle;
	iotbus_gpio_context_h dev;

	snprintf(gpio_dev, 16, "/dev/gpio%d", gpiopin);

	fd = open(gpio_dev, O_RDWR);
	if (fd < 0) {
		ibdbg("open %s failed: %d\n", gpio_dev, errno);
		return NULL;
	}

	handle = (struct _iotbus_gpio_s *)malloc(sizeof(struct _iotbus_gpio_s));
	if (!handle) {
		goto errout_with_close;
	}

	dev = (struct _iotbus_gpio_wrapper_s *)malloc(sizeof(struct _iotbus_gpio_wrapper_s));
	if (!dev) {
		free(handle);
		goto errout_with_close;
	}

	handle->pin = gpiopin;
	handle->drive = IOTBUS_GPIO_DRIVE_FLOAT;
	handle->dir = IOTBUS_GPIO_DIRECTION_OUT;
	handle->edge = IOTBUS_GPIO_EDGE_NONE;
	handle->fd = fd;
	handle->isr_cb = NULL;
	handle->cb = NULL;

	dev->handle = handle;

	return dev;

errout_with_close:
	close(fd);
	return NULL;
}

/**
 * @brief Closes the gpio_context.
 */
int iotbus_gpio_close(iotbus_gpio_context_h dev)
{
	struct _iotbus_gpio_s *handle;
	int ret;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	if (handle->isr_cb != NULL) {
		ret = iotbus_gpio_unregister_cb(dev);
		if (ret != IOTBUS_ERROR_NONE) {
			return ret;
		}
	}

	if (handle->cb != NULL) {
		ret = iotbus_gpio_unset_interrupt(dev, IOTBUS_GPIO_EDGE_NONE);
		if (ret != IOTBUS_ERROR_NONE) {
			return ret;
		}
	}

	close(handle->fd);
	free(handle);
	dev->handle = NULL;
	free(dev);

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Sets Gpio direction.
 */
int iotbus_gpio_set_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e dir)
{
	int ret = -1;
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	switch (dir) {
	case IOTBUS_GPIO_DIRECTION_IN:
		ret = ioctl(handle->fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
		break;
	case IOTBUS_GPIO_DIRECTION_OUT:
		ret = ioctl(handle->fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
		break;
	case IOTBUS_GPIO_DIRECTION_NONE:
		ret = ioctl(handle->fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_NONE);
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (ret != 0) {
		switch (errno) {
		case EPERM:
			ibdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			ibdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			ibdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	handle->dir = dir;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Registers signal for current process based on rising or falling edge on the gpio line
 */
#ifndef CONFIG_DISABLE_SIGNALS
int iotbus_gpio_register_signal(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge)
{
	int ret = -1;
	struct _iotbus_gpio_s *handle;
	FAR struct gpio_notify_s notify;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	switch (edge) {
	case IOTBUS_GPIO_EDGE_NONE:
		notify.gn_rising = false;
		notify.gn_falling = false;
		break;
	case IOTBUS_GPIO_EDGE_BOTH:
		notify.gn_rising  = true;
		notify.gn_falling = true;
		break;
	case IOTBUS_GPIO_EDGE_RISING:
		notify.gn_rising  = true;
		notify.gn_falling = false;
		break;
	case IOTBUS_GPIO_EDGE_FALLING:
		notify.gn_rising  = false;
		notify.gn_falling = true;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	notify.gn_signo = SIGUSR1;
	ret = ioctl(handle->fd, GPIOIOC_REGISTER, (unsigned long)&notify);
	if (ret != 0) {
		switch (errno) {
		case EPERM:
			ibdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			ibdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			ibdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}
	return IOTBUS_ERROR_NONE;
}
#endif

/**
 * @brief Sets the edge mode on the Gpio.
 */
int iotbus_gpio_set_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge)
{
	int ret = -1;
	struct _iotbus_gpio_s *handle;
	struct gpio_pollevents_s pollevents;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	switch (edge) {
	case IOTBUS_GPIO_EDGE_NONE:
		pollevents.gp_rising  = false;
		pollevents.gp_falling = false;
		break;
	case IOTBUS_GPIO_EDGE_BOTH:
		pollevents.gp_rising  = true;
		pollevents.gp_falling = true;
		break;
	case IOTBUS_GPIO_EDGE_RISING:
		pollevents.gp_rising  = true;
		pollevents.gp_falling = false;
		break;
	case IOTBUS_GPIO_EDGE_FALLING:
		pollevents.gp_rising  = false;
		pollevents.gp_falling = true;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = ioctl(handle->fd, GPIOIOC_POLLEVENTS, (unsigned long)&pollevents);
	if (ret != 0) {
		switch (errno) {
		case EPERM:
			ibdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			ibdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			ibdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	handle->edge = edge;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Sets Gpio output mode.
 */
int iotbus_gpio_set_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e drive)
{
	int ret = -1;
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	switch (drive) {
	case IOTBUS_GPIO_DRIVE_PULLUP:
		ret = ioctl(handle->fd, GPIOIOC_SET_DRIVE, (unsigned long)GPIO_DRIVE_PULLUP);
		break;
	case IOTBUS_GPIO_DRIVE_PULLDOWN:
		ret = ioctl(handle->fd, GPIOIOC_SET_DRIVE, (unsigned long)GPIO_DRIVE_PULLDOWN);
		break;
	case IOTBUS_GPIO_DRIVE_FLOAT:
		ret = ioctl(handle->fd, GPIOIOC_SET_DRIVE, (unsigned long)GPIO_DRIVE_FLOAT);
		break;
	}

	if (ret != 0) {
		switch (errno) {
		case EPERM:
			ibdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			ibdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			ibdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	handle->drive = drive;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Registers event handler callback for interrupt.
 */
int iotbus_gpio_register_cb(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge, gpio_isr_cb isr_cb, void *user_data)
{
	int ret = -1;
	iotapi_elem elm;
	struct _iotbus_gpio_s *handle;

	if (isr_cb == NULL) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = iotbus_gpio_set_edge_mode(dev, edge);
	if (ret != IOTBUS_ERROR_NONE) {
		return ret;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	handle->ud = user_data;
	handle->isr_cb = isr_cb;
	elm.fd = handle->fd;
	elm.data = handle;
	elm.func = gpio_async_handler;

	iotapi_insert(&elm);

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Unregisters event handler callback for interrupt.
 */
int iotbus_gpio_unregister_cb(iotbus_gpio_context_h dev)
{
	iotapi_elem elm;
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	elm.fd = handle->fd;

	iotapi_remove(&elm);

	handle->isr_cb = NULL;
	handle->ud = NULL;

	return IOTBUS_ERROR_NONE;
}

static pthread_addr_t gpio_intr_thread(pthread_addr_t arg)
{
	iotbus_gpio_context_h dev = (iotbus_gpio_context_h)arg;
	struct _iotbus_gpio_s *handle;
	
	int ret;
	siginfo_t info;
	sigset_t sig_set;
	bool running = true;
	void *ptr;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIG_IOTBUS_GPIO_RISING);
	sigaddset(&sig_set, SIG_IOTBUS_GPIO_FALLING);

	pthread_sigmask(SIG_BLOCK, &sig_set, NULL);

	while (running) {
		ibdbg("Waiting GPIO Interrupt Signal...\n");
		ret = sigwaitinfo(&sig_set, &info);
#ifdef CONFIG_CAN_PASS_STRUCTS
		ptr = info.si_value.sival_ptr;
#else
		ptr = info.si_value;
#endif
		if (ptr == NULL) {
			ibdbg("Handler is not assigned.\n");
			continue;
		}
		dev = (iotbus_gpio_context_h)ptr;
		handle = (struct _iotbus_gpio_s *)dev->handle;
		if (ret == SIG_IOTBUS_GPIO_RISING) {
			ibdbg("Get GPIO Rising Interrupt in [%d]\n", handle->pin);
		} else if (ret == SIG_IOTBUS_GPIO_FALLING) {
			ibdbg("Get GPIO Falling Interrupt in [%d]\n", handle->pin);
		}
		// Call callback function.
		((iotbus_gpio_cb)handle->cb)(dev);
	}
	
	pthread_sigmask(SIG_UNBLOCK, &sig_set, NULL);
	pthread_exit(NULL);

	return NULL;
}

int iotbus_gpio_set_interrupt(iotbus_gpio_context_h dev, iotbus_gpio_edge_e int_type, iotbus_gpio_cb cb)
{
	int ret = -1;
	struct _iotbus_gpio_s *handle;
	FAR struct gpio_notify_s notify;

	if (!dev || !dev->handle || int_type == IOTBUS_GPIO_EDGE_NONE) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	switch (int_type) {
	case IOTBUS_GPIO_EDGE_BOTH:
		return IOTBUS_ERROR_NOT_SUPPORTED;
	case IOTBUS_GPIO_EDGE_RISING:
		notify.gn_rising  = true;
		notify.gn_falling = false;
		notify.gn_signo = SIG_IOTBUS_GPIO_RISING;
		break;
	case IOTBUS_GPIO_EDGE_FALLING:
		notify.gn_rising  = false;
		notify.gn_falling = true;
		notify.gn_signo = SIG_IOTBUS_GPIO_FALLING;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	struct intr_attr val = { CONFIG_IOTBUS_GPIO_INTERRUPT_PRIORITY, IOTBUS_GPIO, getpid() };

	handle->pid = create_intr_pthread(gpio_intr_thread, (void *)dev, &val);
	if (handle->pid < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	handle->cb = cb;
	notify.pid = handle->pid;
	notify.handle = (void *)dev;
	
	ret = ioctl(handle->fd, GPIOIOC_SET_INTERRUPT, (unsigned long)&notify);
	if (ret != 0) {
		ibdbg("ioctl failed \n");
		return ret;
	}
	return IOTBUS_ERROR_NONE;
}

int iotbus_gpio_unset_interrupt(iotbus_gpio_context_h dev, iotbus_gpio_edge_e int_type)
{
	/* 
	1. Diable Interrupt
	2. Kill Thread
	3. Clear Callback
	*/
	struct _iotbus_gpio_s *handle;
	FAR struct gpio_notify_s notify;
	int ret;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	// Disable Interrupt
	notify.gn_rising  = false;
	notify.gn_falling = false;
	notify.gn_signo = 0;
	notify.pid = 0;
	notify.handle = NULL;

	ret = ioctl(handle->fd, GPIOIOC_SET_INTERRUPT, (unsigned long)&notify);
	if (ret != 0) {
		ibdbg("ioctl failed \n");
		return ret;
	}

	// Kill Thread
	if (handle->pid > 0) {
		ret = pthread_cancel(handle->pid);
		if (ret < 0) {
			ibdbg("pthread cancel failed[%d].\n", ret);
			return IOTBUS_ERROR_UNKNOWN;
		}
		handle->pid = 0;
	}

	// Clear Callback
	handle->cb = NULL;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Reads the gpio value.
 */
int iotbus_gpio_read(iotbus_gpio_context_h dev)
{
	int ret = -1;
	char buf[4];
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;

	ret = read(handle->fd, buf, sizeof(buf));
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}
	ret = lseek(handle->fd, 0, SEEK_SET);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return buf[0] == '1';
}

/**
 * @brief Writes to the gpio value.
 */
int iotbus_gpio_write(iotbus_gpio_context_h dev, int value)
{
	int ret;
	char buf[4];
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (value != IOTBUS_GPIO_LOW && value != IOTBUS_GPIO_HIGH) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	ret = write(handle->fd, buf, snprintf(buf, sizeof(buf), "%d", !!value));
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a direction of the Gpio.
 */
int iotbus_gpio_get_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e * dir)
{
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	*dir = handle->dir;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a pin number of the Gpio.
 */
int iotbus_gpio_get_pin(iotbus_gpio_context_h dev)
{
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	return handle->pin;
}

/**
 * @brief Gets a edge mode of the Gpio.
 */
int iotbus_gpio_get_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e * edge)
{
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	*edge = handle->edge;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a drive mode of the Gpio.
 */
int iotbus_gpio_get_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e * drive)
{
	struct _iotbus_gpio_s *handle;

	if (!dev || !dev->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_gpio_s *)dev->handle;
	*drive = handle->drive;

	return IOTBUS_ERROR_NONE;
}

void *iotbus_gpio_get_callback(iotbus_gpio_context_h dev)
{
	return (void *)dev->handle->cb;
}

#ifdef __cplusplus
}
#endif
