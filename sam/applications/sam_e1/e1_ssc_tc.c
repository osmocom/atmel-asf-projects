

#include <string.h>
#include <asf.h>
#include "conf_board.h"
#include "e1_ssc_tc.h"

#include <osmocom/core/utils.h>

/***********************************************************************
 * Timer/Counter block for FRAME generation
 ***********************************************************************/

/* We use one timer/counter block to generate an artificial frame signal from the
 * received/recovered clock, which we then feed into the SSC for bit/octet-alignment */

/* The SAM4S contains two TC peripheral blocks (let's call them instance 0 + 1),
 * each of which has three TC functional blocks (TC0..2, TC3..5).  The naming in
 * Atmel literature and source code is highly confusing as TC1 sometimes means
 * instance 1, but sometimes counter '1' at instance '0'. */

#if BOARD == OSMOCOM_E1_USB
#define ID_TC_ALIGN		ID_TC2	/* TC2 == TC2 on instance 0 */
#define TC_ALIGN		TC0	/* instance 0 */
#define TC_CHANNEL_ALIGN	2	/* TC2 on instance 0 */
#elif BOARD == SAM4S_XPLAINED_PRO
#define ID_TC_ALIGN		ID_TC4	/* TC4 == TC1 on instance 1 */
#define TC_ALIGN		TC1	/* instance 1 */
#define TC_CHANNEL_ALIGN	1	/* TC1 on instance 1 */
#else
#error "Unsupported BOARD"
#endif

void e1_tc_align_init(void)
{
	printf("%s\n\r", __func__);
	sysclk_enable_peripheral_clock(ID_TC_ALIGN);

		//TC_CMR_AEEVT_CLEAR |	/* clear TIOA on ext event */
	tc_init(TC_ALIGN, TC_CHANNEL_ALIGN,
		TC_CMR_EEVT_XC1 |	/* external event is XC1 to make TOIB an output */
#if BOARD == OSMOCOM_E1_USB
		TC_CMR_TCCLKS_XC2 |	/* use XC2 as clock source */
#elif BOARD == SAM4S_XPLAINED_PRO
		TC_CMR_TCCLKS_XC0 |	/* use XC0 as clock source */
#else
#error "Unsupported BOARD"
#endif
		TC_CMR_WAVE |		/* waveform mode */
		TC_CMR_WAVSEL_UP_RC |	/* upward counting with trigger on RC compare */
		TC_CMR_ACPA_SET |	/* set TIOA on RA compare */
		TC_CMR_BCPB_SET |	/* set TIOB on RB compare */
		TC_CMR_ACPC_CLEAR |	/* clear TIOA on RC compare */
		TC_CMR_BCPC_CLEAR	/* clear TIOB on RC compare */
	       );

	/* Route TCLK0 to XC0, TCLK2 to XC2 (on each TC instance) */
	tc_set_block_mode(TC_ALIGN, TC_BMR_TC0XC0S_TCLK0 | TC_BMR_TC2XC2S_TCLK2);

	/* one frame is 256 clock cycles */
	tc_write_rc(TC_ALIGN, TC_CHANNEL_ALIGN, 256);
	/* use RA to shift position of rising edge of generated frame signal */
	e1_tc_align_set(16);

	tc_start(TC_ALIGN, TC_CHANNEL_ALIGN);
}

uint32_t e1_tc_align_read(void)
{
	return tc_read_cv(TC_ALIGN, TC_CHANNEL_ALIGN);
}

void e1_tc_align_set(uint8_t pos)
{
#if BOARD == OSMOCOM_E1_USB
	tc_write_rb(TC_ALIGN, TC_CHANNEL_ALIGN, pos);
#elif BOARD == SAM4S_XPLAINED_PRO
	tc_write_ra(TC_ALIGN, TC_CHANNEL_ALIGN, pos);
#else
#error "Unsupported BOARD"
#endif
}

static uint32_t e1_tc_align_get(void)
{
#if BOARD == OSMOCOM_E1_USB
	return tc_read_rb(TC_ALIGN, TC_CHANNEL_ALIGN);
#elif BOARD == SAM4S_XPLAINED_PRO
	return tc_read_ra(TC_ALIGN, TC_CHANNEL_ALIGN);
#else
#error "Unsupported BOARD"
#endif
}

static void e1_tc_align_increment(void)
{
	uint32_t ra = e1_tc_align_get();

	ra++;
	if (ra >= 256) {
		/* we cannot set RA to 0 (no pulse generated), so restart */
		e1_tc_align_set(1);
		tc_start(TC_ALIGN, TC_CHANNEL_ALIGN);
	} else {
		/* generate frame signal one bit later */
		e1_tc_align_set(ra);
	}
}


/***********************************************************************
 * SSC code (using PDC)
 ***********************************************************************/

/* enable Transmit support */
#define TX_ENABLE

static int g_ssc_num_overruns;
static Pdc* g_pdc;


/* 1024 bytes covers 32 frames of 32bytes (256bits) each.
 * At frame rate of 8000 Hz, this means 1024 bytes represent 4ms */
#define BUFFER_SIZE	1024
#define NUM_RX_BUF_SSC	4
#define NUM_TX_BUF_SSC	2

struct ssc_buffer {
	uint8_t buffer[BUFFER_SIZE];
	pdc_packet_t packet;
};

static int g_pdc_ssc_cur_rx_idx = 0;
static struct ssc_buffer g_pdc_ssc_rx_buffer[NUM_RX_BUF_SSC];
static int g_pdc_ssc_cur_tx_idx = 0;
static struct ssc_buffer g_pdc_ssc_tx_buffer[NUM_TX_BUF_SSC];

static void ssc_buffer_init(struct ssc_buffer *buf, unsigned int num)
{
	unsigned int i;
	for (i = 0; i < num; i++) {
		buf[i].packet.ul_addr = (uint32_t) &buf[i].buffer;
		buf[i].packet.ul_size = sizeof(buf[i].buffer)/4;
	}
}

static void ensure_alignment(struct ssc_buffer *buf)
{
	unsigned int i;
	/* check every second TS0 byte for FAS symbol (G.704 Section 2.3) */
	for (i = sizeof(buf->buffer)-512; i < sizeof(buf->buffer); i += 32*2) {
		if ((buf->buffer[i] & 0x7f) != 0x1b) {
			e1_tc_align_increment();
			return;
		}
	}
}

static void usb_iso_in_cb(udd_ep_status_t status, iram_size_t nb_transfered, udd_ep_id_t ep)
{
	if (status != UDD_EP_TRANSFER_OK)
		printf("U%d", status);
}

/* Interrupt handler for SSC.  Linker magic binds this function based on name (startup_sam4s.c) */
void SSC_Handler(void)
{
	uint32_t status = ssc_get_status(SSC);
	unsigned int i;

	if (status & SSC_SR_ENDRX) {
		struct ssc_buffer *sb_cur = &g_pdc_ssc_rx_buffer[g_pdc_ssc_cur_rx_idx];
		/* next to be submitted buffer is current + 2 (!) */
		int next_submit_idx = (g_pdc_ssc_cur_rx_idx + 2) % NUM_RX_BUF_SSC;
		bool rc;
		/* refill only the 'next' DMA buffer; PDC has copied previous next to current */
		pdc_rx_init(g_pdc, NULL, &g_pdc_ssc_rx_buffer[next_submit_idx].packet);
		//printf("E%d", g_pdc_ssc_cur_rx_idx);
		/* reverse byte-order of every DWORD in the buffer to fix timeslot positions */
		for (i = 0; i < sizeof(sb_cur->buffer); i += 4) {
			uint32_t *u32 = (uint32_t *) &sb_cur->buffer[i];
			*u32 = __builtin_bswap32(*u32);
		}
		if (g_pdc_ssc_cur_rx_idx == 0)
			ensure_alignment(sb_cur);
		/* hand over to USB ISO IN */
		rc = udi_vendor_iso_in_run(sb_cur->buffer, sizeof(sb_cur->buffer), usb_iso_in_cb);
		if (rc == false) printf("x");
		/* currently receiving (next to complete) is current + 1 */
		g_pdc_ssc_cur_rx_idx = (g_pdc_ssc_cur_rx_idx + 1) % NUM_RX_BUF_SSC;
	}
	if (status & SSC_SR_RXBUFF) {
		/* this means both current and next buffer have ended.  Shouldn't happen,
		 * as due to double buffering we always refill the next buffer in ENDRX */
		printf("RXBUFF!\r\n");
		/* re-start from scratch */
		g_pdc_ssc_cur_rx_idx = 0;
		pdc_rx_init(g_pdc, &g_pdc_ssc_rx_buffer[0].packet, &g_pdc_ssc_rx_buffer[1].packet);
	}
#ifdef TX_ENABLE
	if (status & SSC_SR_ENDTX) {
		/* next to be submitted buffer is current + 2 (!) */
		int next_submit_idx = (g_pdc_ssc_cur_tx_idx + 2) % NUM_TX_BUF_SSC;
		pdc_tx_init(g_pdc, NULL, &g_pdc_ssc_tx_buffer[next_submit_idx].packet);
		//printf("e%d", g_pdc_ssc_cur_tx_idx);
		/* currently transmitting (next to complete) is current + 1 */
		g_pdc_ssc_cur_tx_idx = (g_pdc_ssc_cur_tx_idx + 1) % NUM_TX_BUF_SSC;
	}
	if (status & SSC_SR_TXBUFE) {
		printf("TXBUFE!\r\n");
		g_pdc_ssc_cur_tx_idx = 0;
		pdc_tx_init(g_pdc, &g_pdc_ssc_tx_buffer[0].packet, &g_pdc_ssc_tx_buffer[1].packet);
	}
#endif
	if (status & SSC_SR_OVRUN) {
		g_ssc_num_overruns++;
		printf("OVRUN!\r\n");
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
		buf[i+0] = ts0_pattern[j];
		for (int k=1; k < 32; k++)
			buf[i+k] = k;//i+k;
		j = (j + 1) % sizeof(ts0_pattern);
	}
}

void e1_init_gpio(void)
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
#if BOARD == OSMOCOM_E1_USB
	pio_configure_pin(PIO_PA29_IDX, PIO_PERIPH_B); /* TCLK2 */
	pio_configure_pin(PIO_PA27_IDX, PIO_PERIPH_B); /* TIOB2 */
#elif BOARD == SAM4S_XPLAINED_PRO
	pio_configure_pin(PIO_PC25_IDX, PIO_PERIPH_B); /* TCKL3 */
	pio_configure_pin(PIO_PC26_IDX, PIO_PERIPH_B); /* TIOA4 */
#else
#error "Unsupported BOARD"
#endif
	pio_configure_pin(PIO_PA20_IDX, PIO_PERIPH_A); /* RF */

	/* SPI to LIU */
#if BOARD == OSMOCOM_E1_USB
	pio_configure_pin(PIO_PA7_IDX, PIO_INPUT); /* !LIU_INT */
	pio_configure_pin(PIO_PA31_IDX, PIO_PERIPH_A); /* NPCS1 / !LIU_CS */
#elif BOARD == SAM4S_XPLAINED_PRO
	pio_configure_pin(PIO_PC27_IDX, PIO_INPUT); /* !LIU_INT */
	pio_configure_pin(PIO_PA9_IDX, PIO_PERIPH_B); /* NPCS1 / !LIU_CS */
#else
#error "Unsupported BOARD"
#endif
	pio_configure_pin(PIO_PA12_IDX, PIO_PERIPH_A); /* LIU_MISO */
	pio_configure_pin(PIO_PA14_IDX, PIO_PERIPH_A); /* LIU_SCLK */
	pio_configure_pin(PIO_PA13_IDX, PIO_PERIPH_A); /* LIU_MOSI */
	pio_configure_pin(PIO_PA8_IDX, PIO_OUTPUT_1); /* !RST_LIU */
}

void e1_ssc_init(void)
{
	unsigned int i;

	g_pdc = ssc_get_pdc_base(SSC);
	printf("%s\n\r", __func__);
	ssc_buffer_init(g_pdc_ssc_rx_buffer, ARRAY_SIZE(g_pdc_ssc_rx_buffer));
	ssc_buffer_init(g_pdc_ssc_tx_buffer, ARRAY_SIZE(g_pdc_ssc_tx_buffer));

	for (i = 0; i < ARRAY_SIZE(g_pdc_ssc_tx_buffer); i++)
		fill_tx_buf(g_pdc_ssc_tx_buffer[i].buffer, sizeof(g_pdc_ssc_tx_buffer[i].buffer));

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
	ssc_set_receiver(SSC, (clock_opt_t *) &rx_clk, (data_frame_opt_t *) &rx_opt);

#ifdef TX_ENABLE
	static const clock_opt_t tx_clk = {
		.ul_ckg = SSC_TCMR_CKG_CONTINUOUS,
		.ul_cki = 0, //SSC_RCMR_CKI
#if BOARD == OSMOCOM_E1_USB
		/* TX clock is generated by SSC peripheral, not externally */
		.ul_cko = SSC_TCMR_CKO_CONTINUOUS,
		.ul_cks = SSC_TCMR_CKS_MCK,
#elif BOARD == SAM4S_XPLAINED_PRO
		/* TX clock is generated externally by 2.048Mhz XO */
		.ul_cko = SSC_TCMR_CKO_NONE,
		.ul_cks = SSC_TCMR_CKS_TK,
#else
#error "Unsupported BOARD"
#endif
		.ul_period = 0,
		.ul_start_sel = SSC_TCMR_START_CONTINUOUS,
		.ul_sttdly = 0,
	};
	static const data_frame_opt_t tx_opt = {
		.ul_datlen = 32-1,	/* 32 bit per word */
		.ul_datnb = 8-1,	/* 8 words (=> 8*32=256 bits) */
		.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE,
		.ul_fslen = 0,
		.ul_fslen_ext = 0,
		.ul_fsos = SSC_TFMR_FSOS_NONE,
		.ul_msbf = SSC_TFMR_MSBF,
	};

	printf("ssc_set_transmitter\n\r");
	ssc_set_transmitter(SSC, (clock_opt_t *) &tx_clk, (data_frame_opt_t *) &tx_opt);
#if BOARD == OSMOCOM_E1_USB
	/* somehow ssc_set_clock_divider(SSC, 2048000, sysclk_get_peripheral_hz()) doesn't work */
	SSC->SSC_CMR = SSC_CMR_DIV(27);
#endif /* BOARD */
#endif /* TX_ENABLE */

	/* set up Peripheral DMA controller */
	g_pdc_ssc_cur_rx_idx = 0;
	pdc_rx_init(g_pdc, &g_pdc_ssc_rx_buffer[0].packet, &g_pdc_ssc_rx_buffer[1].packet);
#ifdef TX_ENABLE
	pdc_tx_init(g_pdc, &g_pdc_ssc_tx_buffer[0].packet, &g_pdc_ssc_tx_buffer[1].packet);
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
	ssc_enable_interrupt(SSC, SSC_IER_RXBUFF | SSC_IER_ENDRX | SSC_IER_OVRUN
#ifdef TX_ENABLE
			| SSC_IER_TXBUFE | SSC_IER_ENDTX
#endif
			);

	ssc_enable_rx(SSC);
#ifdef TX_ENABLE
	ssc_enable_tx(SSC);
#endif
}

void e1_ssc_exit(void)
{
#ifdef TX_ENABLE
	ssc_disable_tx(SSC);
#endif
	ssc_disable_rx(SSC);

	NVIC_DisableIRQ(SSC_IRQn);

	pdc_disable_transfer(g_pdc, PERIPH_PTCR_RXTEN
#ifdef TX_ENABLE
			| PERIPH_PTCR_TXTEN
#endif
			);
}
