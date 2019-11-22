#include <tinyara/config.h>
#include <stdio.h>

#include <iotbus/iotbus_pwm.h>
#include <iotbus/iotbus_error.h>

int pwm_test_main(int argc, char *argv[])
{
	int ret;

	printf("PWM TEST\n");

	iotbus_pwm_context_h m_pwm = iotbus_pwm_open(0, 1);

	ret = iotbus_pwm_set_period(m_pwm, 1000);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("iotbus set period error\n");
	}

    iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_ENABLE);

	// for (int i = 0; i < 65536; i = i + 100) {
	// 	ret = iotbus_pwm_set_duty_cycle_in_integer(m_pwm, i);
	// 	if (ret != IOTBUS_ERROR_NONE) {
	// 		printf("iotbus set period error\n");
	// 	}
    //     usleep(10 * 1000);
	// }

	// printf("Set duty 0\n");
	// ret = iotbus_pwm_set_duty_cycle(m_pwm, 0);
	// if (ret != IOTBUS_ERROR_NONE) {
	// 	printf("iotbus set period error\n");
	// }
	// usleep(1000 * 1000);

	// printf("Set duty 100\n");
	// ret = iotbus_pwm_set_duty_cycle(m_pwm, 100);
	// if (ret != IOTBUS_ERROR_NONE) {
	// 	printf("iotbus set period error\n");
	// }
	// usleep(1000 * 1000);

	// printf("Set pwm disable\n");	
    // iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_DISABLE);

	printf("80\% duty!\n");
	ret = iotbus_pwm_set_duty_cycle(m_pwm, 80);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("iotbus set period error\n");
	}
    usleep(1000 * 1000);

	printf("20\% duty!\n");
	ret = iotbus_pwm_set_duty_cycle(m_pwm, 80);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("iotbus set period error\n");
	}
    usleep(1000 * 1000);

	printf("duty done\n");

	iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_DISABLE);

    iotbus_pwm_close(m_pwm);

	return 0;
}
