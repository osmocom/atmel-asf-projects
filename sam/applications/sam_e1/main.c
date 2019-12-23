#include <asf.h>
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_uart_serial.h"
#include "conf_usb.h"
#include "e1_ssc_tc.h"
#include "idt82v2081_asf.h"

#include "microvty.h"

#if !defined(PMC_PCK_PRES_CLK_1)
#define PMC_PCK_PRES_CLK_1   PMC_PCK_PRES(0)
#define PMC_PCK_PRES_CLK_2   PMC_PCK_PRES(1)
#define PMC_PCK_PRES_CLK_4   PMC_PCK_PRES(2)
#define PMC_PCK_PRES_CLK_8   PMC_PCK_PRES(3)
#define PMC_PCK_PRES_CLK_16  PMC_PCK_PRES(4)
#define PMC_PCK_PRES_CLK_32  PMC_PCK_PRES(5)
#define PMC_PCK_PRES_CLK_64  PMC_PCK_PRES(6)
#endif

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Osmocom E1 Example --\r\n" \
	"-- "BOARD_NAME " --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

#ifndef PLL_DEFAULT_MUL
#define PLL_DEFAULT_MUL  7
#endif

#ifndef PLL_DEFAULT_DIV
#define PLL_DEFAULT_DIV  1
#endif

#ifndef MCK_DEFAULT_DIV
#define MCK_DEFAULT_DIV  PMC_MCKR_PRES_CLK_4
#endif

#ifndef example_switch_clock
#define example_switch_clock(a, b, c, d) \
	do {                                 \
		pmc_enable_pllack(a, b, c);      \
		pmc_switch_mck_to_pllack(d);     \
	} while (0)
#endif

#ifndef example_disable_pll
#define example_disable_pll()  pmc_disable_pllack()
#endif

/** Current MCK in Hz */
uint32_t g_ul_current_mck;

volatile uint32_t g_ul_ms_ticks;

/** Button pressed flag */
volatile uint32_t g_ul_button_pressed = 0;

static struct idt82 g_idt;
/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 *  Reconfigure UART console for changed MCK and baudrate.
 */
#if SAMG55
static void reconfigure_console(uint32_t ul_mck, uint32_t ul_baudrate)
{
	sam_usart_opt_t uart_serial_options;
	
	uart_serial_options.baudrate = ul_baudrate,
	uart_serial_options.char_length = CONF_UART_CHAR_LENGTH,
	uart_serial_options.parity_type = US_MR_PAR_NO;
	uart_serial_options.stop_bits = CONF_UART_STOP_BITS,
	uart_serial_options.channel_mode= US_MR_CHMODE_NORMAL,
	uart_serial_options.irda_filter = 0,

	/* Configure PMC */
	flexcom_enable(CONF_FLEXCOM);
	flexcom_set_opmode(CONF_FLEXCOM, FLEXCOM_USART);

	/* Configure PIO */
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);

	/* Configure UART */
	usart_init_rs232(CONF_UART, &uart_serial_options, ul_mck);
	/* Enable the receiver and transmitter. */
	usart_enable_tx(CONF_UART);
	usart_enable_rx(CONF_UART);
}
#else
static void reconfigure_console(uint32_t ul_mck, uint32_t ul_baudrate)
{
	const sam_uart_opt_t uart_console_settings =
			{ ul_mck, ul_baudrate, UART_MR_PAR_NO };

	/* Configure PMC */
	pmc_enable_periph_clk(CONSOLE_UART_ID);

	/* Configure PIO */
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);

	/* Configure UART */
	uart_init(CONF_UART, &uart_console_settings);
}
#endif

bool microvty_cb_uart_rx_not_empty(void)
{
	return usart_serial_is_rx_ready(UART1);
}

/**
 * \brief Display test core menu.
 */
static void display_menu_core(void)
{
	printf("\n\r");
	printf("===============================================\n\r");
	printf("Menu: press a key to continue.\n\r");
	printf("===============================================\n\r");
	printf("Configure:\n\r");
	printf("  F : 128-bit flash access\n\r");
	printf("  G : 64-bit flash access\n\r");
	printf("Mode:\n\r");
	printf("  A : Active Mode\n\r");
	printf("  S : Sleep Mode\n\r");
	printf("  W : Wait Mode\n\r");
#if (!(SAMG51 || SAMG53 || SAMG54))
	printf("  B : Backup Mode(Entered %d times).\n\r", (int)gpbr_read(GPBR0));
#endif
	printf("Quit:\n\r");
	printf("  Q : Quit test.\n\r");

	printf("\n\r");
	printf("-----------------------------------------------\n\r");
	printf("Current configuration:\n\r");
	printf("  CPU Clock         : MCK=%d Hz\n\r", (int)g_ul_current_mck);
	if ((efc_get_flash_access_mode(EFC0) & EEFC_FMR_FAM) == EEFC_FMR_FAM) {
		printf("  Flash access mode : 64-bit\n\r");
	} else {
		printf("  Flash access mode : 128-bit\n\r");
	}

	printf("-----------------------------------------------\n\r");
	printf("\n\r");
}

static void dump_all_tc_cv()
{
	int i;
	for (i = 0; i < 3; i++)
		printf("TC0/%d: %u\n\r", i, tc_read_cv(TC0, i));
	for (i = 0; i < 3; i++)
		printf("TC1/%d: %u\n\r", i, tc_read_cv(TC1, i));
}

DEFUN(vty_tc_dump_cv, tc_dump_cv_cmd, "tc-dump-cv", "Dump the CV of all TC")
{
	dump_all_tc_cv();
}

DEFUN(vty_idt_read, idt_read_cmd, "idt-read", "Read IDT82 register")
{
	long reg;

	if (argc < 2) {
		/* dump them all */
		for (reg = 0; reg < 32; reg += 8) {
			printf("\tIDT82 0x%02lx: %02x %02x %02x %02x %02x %02x %02x %02x\r\n", reg,
				idt82_reg_read(&g_idt, reg),
				idt82_reg_read(&g_idt, reg+1),
				idt82_reg_read(&g_idt, reg+2),
				idt82_reg_read(&g_idt, reg+3),
				idt82_reg_read(&g_idt, reg+4),
				idt82_reg_read(&g_idt, reg+5),
				idt82_reg_read(&g_idt, reg+6),
				idt82_reg_read(&g_idt, reg+7)
				);
		}
		return;
	}
	reg = strtol(argv[1], NULL, 0);

	printf("IDT82 Register 0x%02lx: 0x%02x\r\n", reg, idt82_reg_read(&g_idt, reg));
}

DEFUN(vty_idt_write, idt_write_cmd, "idt-write", "Write to SPI register")
{
	long reg, val;

	if (argc < 2) {
		printf("You must specify the register\r\n");
		return;
	}
	reg = strtol(argv[1], NULL, 0);

	if (argc < 3) {
		printf("You must specify the value\r\n");
		return;
	}
	val = strtol(argv[2], NULL, 0);

	idt82_reg_write(&g_idt, reg, val);
	printf("IDT82 Register 0x%02lx: Written 0x%02lx\r\n", reg, val);
}

DEFUN(vty_tca_write, tca_write_cmd, "tc-align-write", "Write to TC Align register")
{
	long val;

	if (argc < 2) {
		printf("You must specify the value\r\n");
		return;
	}
	val = strtol(argv[1], NULL, 0);

	if (val < 0 || val > 256) {
		printf("Value out of range <0..255>\r\n");
		return;
	}

	e1_tc_align_set(val);
}


static void main_vbus_action(bool b_high)
{
	if (b_high)
		udc_attach();
	else
		udc_detach();
}

void SysTick_Handler(void)
{
	g_ul_ms_ticks++;
}

/**
 * \brief Low power application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	g_ul_current_mck = sysclk_get_cpu_hz();
	board_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000);

	udc_start();
	udc_detach();

	/* Initialize the console uart */
	configure_console();

	microvty_init("sam_e1> ");
	microvty_register(&idt_read_cmd);
	microvty_register(&idt_write_cmd);
	microvty_register(&tc_dump_cv_cmd);
	microvty_register(&tca_write_cmd);

	/* Set default clock and re-configure UART */
	reconfigure_console(g_ul_current_mck, CONF_UART_BAUDRATE);

	/* Output example information */
	puts(STRING_HEADER);

	if (!udc_include_vbus_monitoring())
		main_vbus_action(true);

	/* General SSC for E1 init */
	e1_init_gpio();
	e1_tc_align_init();

	/* LIU specific bits */
	idt82_asf_init(&g_idt, SPI, 1);
	idt82_init(&g_idt);

	microvty_print_prompt();
	while (1) {
		microvty_try_recv();
	}
}


/* enable the interface */
bool usb_vendor_e1_enable(void)
{
	printf("%s\n\r", __func__);
	e1_ssc_init();
	return true;
}

/* disable the interface */
void usb_vendor_e1_disable(void)
{
	printf("%s\n\r", __func__);
	e1_ssc_exit();
}

/* handle a control request directed to an interface */
//bool usb_vendor_e1_setup(void) { }


/* enable the interface */
bool main_cdc_enable(uint8_t port)
{
	printf("%s\n\r", __func__);
	return true;
}

/* disable the interface */
void main_cdc_disable(uint8_t port)
{
	printf("%s\n\r", __func__);
}

void main_cdc_rx_notify(uint8_t port)
{
	printf("%s\n\r", __func__);
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	printf("%s\n\r", __func__);
}
