#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "BMI160.h"
#include "freertos_i2c"

/* TODO: insert other definitions and declarations here. */
freertos_i2c_flag_t g_bmi160_sucess = freertos_i2c_fail;

/*
 * @brief   Application entry point.
 */

void read_acc(void)
{
	bmi160_raw_data_t acc_data;
	acc_data = bmi160_get_data_accel();
	PRINTF("Data from acc:  X = %d  Y = %d  Z = %d \n", acc_data.x, acc_data.y, acc_data.z );
}

void read_gyro(void)
{
	bmi160_raw_data_t acc_gyro;
	gyro_data = bmi160_get_data_gyro();
	PRINTF("Data from gyro:  X = %d  Y = %d  Z = %d \n", acc_gyro.x, acc_gyro.y, acc_gyro.z );
}
int main(void) {


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("BMI160 tasks creation\n");
    g_bmi160_sucess = bmi160_init();
    /*Once communication was made effectively creat tasks and start scheduler*/
    if(freertos_i2c_fail != g_bmi160_sucess)
    {
    	xTaskCreate(read_acc, "read_acc_data", 110, NULL, 1, NULL);
    	xTaskCreate(read_gyro, "read_gyro_data", 110, NULL, 1, NULL);

    	vTaskStartScheduler();
    }

    while(1) {

    }
    return 0 ;
}
