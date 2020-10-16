#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "BMI160.h"

/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */
void init_acc_gyro(void *parameters)
{
	uint8_t g_bmi160_sucess = 1;
	g_bmi160_sucess = bmi160_init();
	if(0 == g_bmi160_sucess)
	{
		PRINTF("BMI160 configured\n");
	}
	vTaskSuspend(NULL);
}

void read_acc(void *parameters)
{
	bmi160_raw_data_t acc_data;
	for(;;)
	{
		acc_data = bmi160_get_data_accel();
		PRINTF("Data from acc:  X = %d  Y = %d  Z = %d \n", acc_data.x, acc_data.y, acc_data.z );
	}
}

void read_gyro(void *parameters)
{
	bmi160_raw_data_t gyro_data;
	for(;;)
	{
		gyro_data = bmi160_get_data_gyro();
		PRINTF("Data from gyro:  X = %d  Y = %d  Z = %d \n", gyro_data.x, gyro_data.y, gyro_data.z );
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

    xTaskCreate(init_acc_gyro, "init_acc_gyro", 110, NULL, 1, NULL);
    xTaskCreate(read_acc, "read_acc_data", 110, NULL, 1, NULL);
    xTaskCreate(read_gyro, "read_gyro_data", 110, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1)
    {

    }
    return 0 ;
}
