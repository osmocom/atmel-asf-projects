#include <stdint.h>
#include <asf.h>

#include "idt82v2081.h"
#include "idt82v2081_asf.h"

/* Adaption layer between idt82 driver and Atmel ASF SPI driver */

/*! reverse the bit order in a byte
 *  \param[in] x 8bit input value
 *  \returns 8bit value where bits order has been reversed
 *
 * See Chapter 7 "Hackers Delight"
 */
static uint32_t osmo_revbytebits_8(uint8_t x)
{
	x = (x & 0x55) <<  1 | (x & 0xAA) >>  1;
	x = (x & 0x33) <<  2 | (x & 0xCC) >>  2;
	x = (x & 0x0F) <<  4 | (x & 0xF0) >>  4;

	return x;
}

#define B_READ	(1 << 5)

static struct spi_device g_spidev = {
	.id = 1,
};

/* backend function for core idt82 driver */
int idt82_reg_read(struct idt82 *idt, uint8_t reg)
{
	uint8_t req[2] = { (reg & 0x1F) | B_READ, 0 };
	uint8_t res[2];

	req[0] = osmo_revbytebits_8(req[0]);
	req[1] = osmo_revbytebits_8(req[1]);

	spi_select_device(idt->priv, &g_spidev);
#if 0
	spi_write_packet(idt->priv, (reg & 0x1F) | B_READ, 16);
	while (!SPI_IsFinished(idt->priv));
	spi_read_packet(idt->priv, &res, 16);
#else
	spi_transceive_packet(idt->priv, req, res, sizeof(res));
#endif
	spi_deselect_device(idt->priv, &g_spidev);
	res[1] = osmo_revbytebits_8(res[1]);

	return res[1];
}
/* backend function for core idt82 driver */
int idt82_reg_write(struct idt82 *idt, uint8_t reg, uint8_t val)
{
	uint8_t req[2] = { (reg & 0x1F), val };
	req[0] = osmo_revbytebits_8(req[0]);
	req[1] = osmo_revbytebits_8(req[1]);

	spi_select_device(idt->priv, &g_spidev);
	spi_write_packet(idt->priv, req, sizeof(req));
	spi_deselect_device(idt->priv, &g_spidev);

	return 0;
}

/* initialize the SPI interface to the IDT82 */
int idt82_asf_init(struct idt82 *idt, Spi *spi, uint8_t cs)
{
	g_spidev.id = cs;
	idt->priv = spi;

	printf("%s: spi=%p, cs=%u\n\r", __func__, spi, cs);

	pmc_enable_periph_clk(ID_SPI);

	spi_master_init(idt->priv);
	spi_master_setup_device(idt->priv, &g_spidev, SPI_MODE_0, 100000, cs);
	//spi_configure_cs_behavior(idt->priv, g_spidev.id, SPI_CS_RISE_NO_TX);
	spi_enable(idt->priv);

	return 0;
}
