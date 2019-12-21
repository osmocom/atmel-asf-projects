#pragma once

/* micrVTY - small implementation of a limited feature command line interface
 * (C) 2019 by Harald Welte <laforge@gnumonks.org>
 *
 * modelled after libosmovty (part of libosmocore.git), which in turn is a fork
 * of the command line interface of GNU zebra / quagga.
 *
 * microVTY is intended for very small bare-iron microcontroller. It doesn't need
 * any dynamic allocations/heap, and tries to stay very simplistic to conserve
 * resources.
 */

/*! Total size in bytes for microvty command buffer (max command line length) */
#define MICROVTY_CMD_BUF_SIZE 128

/*! Maximum number of VTY commands that can be registered with microvty_register() */
#define MICROVTY_MAX_CMD 32

struct microvty_fn {
	const char *command;
	const char *help;
	void (*fn)(int argc, char **argv);
};

#define DEFUN(funcname, cmdname, cmdstr, helpstr)	\
	static void funcname(int argc, char **argv);		\
	static struct microvty_fn cmdname = { 			\
		.command = cmdstr,				\
		.help = helpstr,				\
		.fn = funcname,					\
	};							\
	static void funcname(int argc, char **argv)

void microvty_init(const char *prompt);
int microvty_register(const struct microvty_fn *cmd);
void microvty_try_recv(void);
void microvty_print_prompt(void);

/* to be provided by implementation: tell the code if UART Rx is non-
 * empty and hence if a subsequent getchar() would return something */
bool microvty_cb_uart_rx_not_empty(void);
