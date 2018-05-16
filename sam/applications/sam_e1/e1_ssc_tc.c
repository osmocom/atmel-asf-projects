

#include <string.h>
#include <asf.h>
#include "conf_board.h"

/* enable Transmit support */
#define TX_ENABLE

static int g_ssc_num_overruns;
static Pdc* g_pdc;


/* 1024 bytes covers 32 frames of 32bytes (256bits) each.
 * At frame rate of 8000 Hz, this means 1024 bytes represent 4ms */
#define BUFFER_SIZE	1024

static uint8_t g_pdc_ssc_buffer0[BUFFER_SIZE];
static pdc_packet_t g_pdc_ssc_rx_packet0 = {
	.ul_addr = &g_pdc_ssc_buffer0,
	.ul_size = BUFFER_SIZE/4, /* 32bit transfers, so 4 bytes per transfer */
};
static uint8_t g_pdc_ssc_buffer1[BUFFER_SIZE];
static pdc_packet_t g_pdc_ssc_rx_packet1 = {
	.ul_addr = &g_pdc_ssc_buffer1,
	.ul_size = BUFFER_SIZE/4, /* 32bit transfers, so 4 bytes per transfer */
};

static uint8_t g_pdc_ssc_buffer2[BUFFER_SIZE];
static pdc_packet_t g_pdc_ssc_tx_packet = {
	.ul_addr = &g_pdc_ssc_buffer2,
	.ul_size = BUFFER_SIZE/4, /* 32bit transfers, so 4 bytes per transfer */
};



/* We use one timer/counter block to generate an artificial frame signal from the
 * received/recovered clock, which we then feed into the SSC for bit/octet-alignment */

#define ID_TC_ALIGN		ID_TC4	/* TC4 == TC1 on instance 1 */
#define TC_ALIGN		TC1	/* instance 1 */
#define TC_CHANNEL_ALIGN	1	/* TC1 on instance 1 */

void e1_tc_align_init()
{
	printf("%s\n\r", __func__);
	sysclk_enable_peripheral_clock(ID_TC_ALIGN);

		//TC_CMR_EEVT_XC1 |	/* external event is XC1 */
		//TC_CMR_AEEVT_CLEAR |	/* clear TIOA on ext event */
	tc_init(TC_ALIGN, TC_CHANNEL_ALIGN,
		TC_CMR_TCCLKS_XC0 |	/* use XC0 as clock source */
		TC_CMR_WAVE |		/* waveform mode */
		TC_CMR_WAVSEL_UP_RC |	/* upward counting with trigger on RC compare */
		TC_CMR_ACPA_SET |	/* set TIOA on RA compare */
		TC_CMR_BCPB_SET |	/* set TIOB on RB compare */
		TC_CMR_ACPC_CLEAR |	/* clear TIOA on RC compare */
		TC_CMR_BCPC_CLEAR	/* clear TIOB on RC compare */
	       );

	/* Route TCLK0 to XC0 */
	tc_set_block_mode(TC_ALIGN, TC_BMR_TC0XC0S_TCLK0 | TC_BMR_TC1XC1S_TCLK1 | TC_BMR_TC2XC2S_TCLK2);

	/* one frame is 256 clock cycles */
	tc_write_rc(TC_ALIGN, TC_CHANNEL_ALIGN, 256);
	/* use RA to shift position of rising edge of generated frame signal */
	tc_write_ra(TC_ALIGN, TC_CHANNEL_ALIGN, 16);
	tc_write_rb(TC_ALIGN, TC_CHANNEL_ALIGN, 64);

	tc_start(TC_ALIGN, TC_CHANNEL_ALIGN);
}

uint32_t e1_tc_align_read()
{
	return tc_read_cv(TC_ALIGN, TC_CHANNEL_ALIGN);
}

/* Interrupt handler for SSC.  Linker magic binds this function based on name (startup_sam4s.c) */
void SSC_Handler(void)
{
	uint32_t status = ssc_get_status(SSC);

	if (status & SSC_SR_RXBUFF) {
		//printf("R");
		pdc_rx_init(g_pdc, &g_pdc_ssc_rx_packet1, NULL); /* FIXME: swap buffers */
	}
#ifdef TX_ENABLE
	if (status & SSC_SR_TXBUFE) {
		pdc_tx_init(g_pdc, &g_pdc_ssc_tx_packet, NULL);
	}
#endif
	if (status & SSC_SR_OVRUN) {
		g_ssc_num_overruns++;
	}
}

/* fill TX Buffer with (hopefully) valid TS0 idle pattern */
static void fill_tx_buf(uint8_t *buf, unsigned int size)
{
	unsigned int i, j = 0;
	const uint8_t ts0_pattern[] = { 0x1b, 0x40, 0x1b, 0x40, 0x1b, 0xc0, 0x1b, 0x40,
					0x1b, 0xc0, 0x9b, 0xc0, 0x9b, 0x40, 0x9b, 0x40 };

	memset(buf, 0xff, size);
	for (i = 0; i < size; i += 32) {
		buf[i] = ts0_pattern[j];
		j = (j + 1) % sizeof(ts0_pattern);
	}
}

void e1_init_gpio()
{
	printf("%s\n\r", __func__);

	/* enable clock to peripherals */
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOC);

	/* LIU / SSC interface */
	pio_configure_pin(PIO_PA19_IDX, PIO_PERIPH_A); /* RK */
	pio_configure_pin(PIO_PA18_IDX, PIO_PERIPH_A); /* RD */
	pio_configure_pin(PIO_PA16_IDX, PIO_PERIPH_A); /* TK */
	pio_configure_pin(PIO_PA17_IDX, PIO_PERIPH_A); /* TD */

	/* receive frame generation via TC */
	pio_configure_pin(PIO_PC25_IDX, PIO_PERIPH_B); /* TCKL3 */
	pio_configure_pin(PIO_PA20_IDX, PIO_PERIPH_A); /* RF */
	pio_configure_pin(PIO_PC26_IDX, PIO_PERIPH_B); /* TIOA4 */
	//pio_configure_pin(PIO_PC27_IDX, PIO_PERIPH_B); /* TIOB4 */
}

void e1_ssc_init()
{
	g_pdc = ssc_get_pdc_base(SSC);
	printf("%s\n\r", __func__);
	fill_tx_buf(g_pdc_ssc_buffer2, sizeof(g_pdc_ssc_buffer2));

	sysclk_enable_peripheral_clock(ID_SSC);

	/* disable register write protect */
	ssc_set_writeprotect(SSC, 0);
	/* no chip-internal loopback */
	ssc_set_normal_mode(SSC);

	static const clock_opt_t rx_clk = {
		.ul_ckg = SSC_RCMR_CKG_CONTINUOUS,
		.ul_cki = 0, //SSC_RCMR_CKI
		.ul_cko = SSC_RCMR_CKO_NONE,
		.ul_cks = SSC_RCMR_CKS_RK,
		.ul_period = 0,
		.ul_start_sel = SSC_RCMR_START_RF_RISING,
		//.ul_start_sel = SSC_RCMR_START_CONTINUOUS,
		.ul_sttdly = 0,
	};
	static const data_frame_opt_t rx_opt = {
		.ul_datlen = 32-1,	/* 32 bit per word */
		.ul_datnb = 8-1,	/* 8 words (=> 8*32=256 bits) */
		.ul_fsedge = SSC_RFMR_FSEDGE_POSITIVE,
		.ul_fslen = 0,
		.ul_fslen_ext = 0,
		.ul_fsos = SSC_RFMR_FSOS_NONE,
		.ul_msbf = SSC_RFMR_MSBF,
	};

	printf("ssc_set_receiver\n\r");
	ssc_set_receiver(SSC, &rx_clk, &rx_opt);

#ifdef TX_ENABLE
	static const clock_opt_t tx_clk = {
		.ul_ckg = SSC_TCMR_CKG_CONTINUOUS,
		.ul_cki = 0, //SSC_RCMR_CKI
		.ul_cko = SSC_TCMR_CKO_NONE,
		.ul_cks = SSC_TCMR_CKS_TK,
		.ul_period = 0,
		.ul_start_sel = SSC_TCMR_START_CONTINUOUS,
		.ul_sttdly = 0,
	};
	static const data_frame_opt_t tx_opt = {
		.ul_datlen = 32-1,
		.ul_datnb = 8-1,
		.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE,
		.ul_fslen = 0,
		.ul_fslen_ext = 0,
		.ul_fsos = SSC_TFMR_FSOS_NONE,
		.ul_msbf = SSC_TFMR_MSBF,
	};

	printf("ssc_set_transmitter\n\r");
	ssc_set_transmitter(SSC, &tx_clk, &tx_opt);
#endif

	/* set up Peripheral DMA controller */
	//pdc_rx_init(g_pdc, &g_pdc_ssc_rx_packet0, &g_pdc_ssc_rx_packet1);
	pdc_rx_init(g_pdc, &g_pdc_ssc_rx_packet0, NULL);
#ifdef TX_ENABLE
	pdc_tx_init(g_pdc, &g_pdc_ssc_tx_packet, NULL);
#endif
	pdc_enable_transfer(g_pdc, PERIPH_PTCR_RXTEN
#ifdef TX_ENABLE
			| PERIPH_PTCR_TXTEN
#endif
			);

	printf("SSC NVIC interrupt enable\n\r");
	/* enable SSC interrupt at NVIC level */
	NVIC_DisableIRQ(SSC_IRQn);
	NVIC_ClearPendingIRQ(SSC_IRQn);
	NVIC_SetPriority(SSC_IRQn, 0);
	NVIC_EnableIRQ(SSC_IRQn);

	/* enable SSC interrupts */
	ssc_enable_interrupt(SSC, SSC_IER_RXBUFF
#ifdef TX_ENABLE
			| SSC_IER_TXBUFE
#endif
			);

	ssc_enable_rx(SSC);
#ifdef TX_ENABLE
	ssc_enable_tx(SSC);
#endif
}

#if 0
static struct idt82 g_idt;

static void idt_init()
{
	idt82_at91_init(&g_idt, SPI, ID_SPI, );
	idt82_init(&g_idt);
	idt82_mode(&g_idt, IDT_MODE_E1);
	idt82_termination(&g_idt, IDT_TERM_INT_120);
}
#endif
