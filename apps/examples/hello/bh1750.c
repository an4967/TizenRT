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
/****************************************************************************
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include <fcntl.h>
#include <tinyara/i2c.h>

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

static float readLightLevel(void) {

	// Measurement result will be stored here
	float level = -1.0;

	// Send mode to sensor
	uint8_t cmd[100] = { 0, };
	int ret;
	cmd[0] = 0x10; // CONTINUOUS_HIGH_RES_MODE 

	ret = i2c_write(i2c_dev, &configs, cmd, 1);
	if (ret < 0) {
		printf("i2c_write fail(%d)\n", ret);
		return -ret;
	}

	// Read two bytes from the sensor, which are low and high parts of the sensor
	// value
	uint8_t outdata[2] = { 0, };
	ret = i2c_read(i2c_dev, &configs, outdata, 2);
	if (ret < 0) {
		printf("i2c_read fail(%d)\n", ret);
		return -ret;
	}

	// printf("outdata[0] : %02x\n", outdata[0]);
	// printf("outdata[1] : %02x\n", outdata[1]);

	unsigned int tmp = 0;
	tmp = outdata[0];
	tmp <<= 8;
	tmp |= outdata[1];
	// printf("tmp : %d\n" , tmp);
	level = (float)tmp;

	if (level != -1.0) {
		// Print raw value if debug enabled
		// printf("[BH1750] Raw value: %f\n", level);

		// Convert raw value to lux
		level /= 1.2; //BH1750_CONV_FACTOR;

		// Print converted value if debug enabled
		// printf("[BH1750] Converted float value: %f\n", level);
	}

	return level;
}

static int bh1750_initialize(void)
{
	uint8_t data[1];
	uint8_t outdata[10] = { 0, };
	int ret;
	data[0] = 0x10; // CONTINUOUS_HIGH_RES_MODE 

	ret = i2c_write(i2c_dev, &configs, data, 1);
	if (ret < 0) {
		printf("i2c_write fail(%d)\n", ret);
		return -ret;
	}

	outdata[0] = -1;
	ret = i2c_read(i2c_dev, &configs, outdata, 1);
	if (ret < 0) {
		printf("i2c_read fail(%d)\n", ret);
		return -ret;
	}

	printf("!!! OUT : %d !!!\n", outdata[0]);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	int ret = -1;
	int i;

	printf("Run bh1750! \n");

	configs.frequency = 100000;
	configs.address = 0x23;
	configs.addrlen = 7;

	int port = 1;
	i2c_dev = up_i2cinitialize(port);
	if (i2c_dev == NULL) {
		printf("i2ctest_main: up_i2cinitialize(i2c:%d) failed\n", port);
		return -1;
	}

	ret = bh1750_initialize();
	printf("bh1750 init result : %d\n", ret);

	float lux = -1.0f;
	for (i = 0; i < 20; i++) {
		lux = readLightLevel();
		printf("lux : %.1f\n", lux);
		usleep(1000 * 1000);
	}
	
	return 0;
}
