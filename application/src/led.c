#include <drivers/gpio.h>
#include <logging/log.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include "led.h"

#define LED_ON 0
#define LED_OFF 1

LOG_MODULE_REGISTER(LED);

static void _led_init(const char *device_name, u32_t led_pin)
{
	struct device *led_controller = device_get_binding(device_name);
	gpio_pin_configure(led_controller, led_pin, GPIO_DIR_OUT);
	gpio_pin_set(led_controller, led_pin, LED_OFF);
}

static void _timer_0_expiry_fn(struct k_timer *timer)
{
	gpio_pin_set(device_get_binding(DT_ALIAS_LED0_GPIOS_CONTROLLER),
		     DT_ALIAS_LED0_GPIOS_PIN, LED_OFF);
}

static void _timer_1_expiry_fn(struct k_timer *timer)
{
	gpio_pin_set(device_get_binding(DT_ALIAS_LED1_GPIOS_CONTROLLER),
		     DT_ALIAS_LED1_GPIOS_PIN, LED_OFF);
}

static void _timer_2_expiry_fn(struct k_timer *timer)
{
	gpio_pin_set(device_get_binding(DT_ALIAS_LED2_GPIOS_CONTROLLER),
		     DT_ALIAS_LED2_GPIOS_PIN, LED_OFF);
}

static K_TIMER_DEFINE(_led_timer_0, _timer_0_expiry_fn, _timer_0_expiry_fn);
static K_TIMER_DEFINE(_led_timer_1, _timer_1_expiry_fn, _timer_1_expiry_fn);
static K_TIMER_DEFINE(_led_timer_2, _timer_2_expiry_fn, _timer_2_expiry_fn);

void led_init()
{
	_led_init(DT_ALIAS_LED0_GPIOS_CONTROLLER, DT_ALIAS_LED0_GPIOS_PIN);
	_led_init(DT_ALIAS_LED1_GPIOS_CONTROLLER, DT_ALIAS_LED1_GPIOS_PIN);
	_led_init(DT_ALIAS_LED2_GPIOS_CONTROLLER, DT_ALIAS_LED2_GPIOS_PIN);
}

void led_flash(enum led led)
{
	struct device *led_controller = NULL;
	u32_t led_pin = 0;
	struct k_timer *led_timer = NULL;

	switch (led) {
	case LED_RED:
		led_controller =
			device_get_binding(DT_ALIAS_LED0_GPIOS_CONTROLLER);
		led_pin = DT_ALIAS_LED0_GPIOS_PIN;
		led_timer = &_led_timer_0;
		break;
	case LED_GREEN:
		led_controller =
			device_get_binding(DT_ALIAS_LED1_GPIOS_CONTROLLER);
		led_pin = DT_ALIAS_LED1_GPIOS_PIN;
		led_timer = &_led_timer_1;
		break;
	case LED_BLUE:
		led_controller =
			device_get_binding(DT_ALIAS_LED2_GPIOS_CONTROLLER);
		led_pin = DT_ALIAS_LED2_GPIOS_PIN;
		led_timer = &_led_timer_2;
		break;
	}

	gpio_pin_set(led_controller, led_pin, LED_ON);
	k_timer_start(led_timer, K_MSEC(50), 0);
}
