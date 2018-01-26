#include <string.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/task.h"

#include "sdkconfig.h" // generated by "make menuconfig"

#include "ssd1366.h"
//#include "font8x8_basic.h"

#include "tdelay.h"
#include "test_led.h"
#include "oled.h"

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

//#define tag "SSD1306"

void
i2c_master_init( void )
{
	i2c_config_t i2c_config =
	{
		.mode = I2C_MODE_MASTER,
		.sda_io_num = SDA_PIN,
		.scl_io_num = SCL_PIN,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 1000000
	};
	i2c_param_config(I2C_NUM_0, &i2c_config);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

void
task_idle( void *ignore )
{
	init_test_led();
	while(1)
	{
		led_toggle();
		tdelay(1000);
	}
}

static const char * const text[8] = 
{
	"Hello",
	"Hello World",
	"Hello cruel World",
	"Hello funny World",
	"Hola Mundo",
	"Hola Mundo cruel",
	"Hola Mundo divertido",
	"Grusse Welt"
};


void
task_text( void *ignore )
{
	unsigned line;

	while(1)
	{
		for( line = 0; line < 32; ++line )
		{
			oled_text( text[line%8] );
			tdelay(100);
		}
		oled_clear();
		tdelay(500);
	}
}

void
app_main(void)
{
	i2c_master_init();

	oled_init();
	oled_clear();
	oled_contrast( 255 );

	xTaskCreate(&task_idle, "task_idle", 2048, NULL, 6, NULL );
	xTaskCreate(&task_text, "task_text", 2048, NULL, 6, NULL );

}
