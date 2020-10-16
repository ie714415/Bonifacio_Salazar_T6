#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "freertos_i2c.h"
#include "BMI160.h"
#include "task.h"
#include "semphr.h"
/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */
SemaphoreHandle_t i2c_sem;

void init_acc_gyro(void *parameters)
{
	uint8_t g_bmi160_sucess = freertos_i2c_fail;
	g_bmi160_sucess = bmi160_init();
	if(freertos_i2c_sucess == g_bmi160_sucess)
	{
		PRINTF("BMI160 configured\n\r");
	}
	xSemaphoreGive(i2c_sem);
	vTaskSuspend(NULL);
}

void read_data(void *parameters)
{
	bmi160_raw_data_t acc_data;
	bmi160_raw_data_t gyro_data;

	xSemaphoreTake(i2c_sem, portMAX_DELAY);

	for(;;)
	{
		acc_data = bmi160_get_data_accel();
		gyro_data = bmi160_get_data_gyro();
		PRINTF("Data from acc:  X = %d  Y = %d  Z = %d\n\r", acc_data.x, acc_data.y, acc_data.z );
		PRINTF("Data from gyro:  X = %d  Y = %d  Z = %d\n\r", gyro_data.x, gyro_data.y, gyro_data.z );
		vTaskDelay(pdMS_TO_TICKS(300));
	}
}

int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    i2c_sem = xSemaphoreCreateBinary();

    xTaskCreate(init_acc_gyro, "init_acc_gyro", 110, NULL, 1, NULL);
    xTaskCreate(read_data, "read_data", 110, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1)
    {

    }
    return 0 ;
}
