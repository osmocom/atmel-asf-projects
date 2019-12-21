#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <osmocom/core/utils.h>
#include "microvty.h"

/* microVTY - small implementation of a limited feature command line interface
 * (C) 2019 by Harald Welte <laforge@gnumonks.org>
 *
 * modelled after libosmovty (part of libosmocore.git), which in turn is a fork
 * of the command line interface of GNU zebra / quagga.
 *
 * microVTY is intended for very small bare-iron microcontroller. It doesn't need
 * any dynamic allocations/heap, and tries to stay very simplistic to conserve
 * resources.
 */

struct microvty_state {
	const char *prompt;
	char buf[MICROVTY_CMD_BUF_SIZE];
	unsigned int buf_idx;
	const struct microvty_fn *cmd[MICROVTY_MAX_CMD];
	unsigned int cmd_idx;
};

static struct microvty_state g_cmds;

/*! register a command with microVTY */
int microvty_register(const struct microvty_fn *cmd)
{
	if (g_cmds.cmd_idx >= ARRAY_SIZE(g_cmds.cmd))
		return -1;
	g_cmds.cmd[g_cmds.cmd_idx++] = cmd;
	return 0;
}

/*! built-in help command */
DEFUN(help, help_cmd, "help", "Print command reference")
{
	unsigned int i;
	printf("Help:\r\n");
	printf(" Command          Help\r\n");
	printf(" ---------------- ----\r\n");
	for (i = 0; i < g_cmds.cmd_idx; i++)
		printf(" %-16s %s\r\n", g_cmds.cmd[i]->command, g_cmds.cmd[i]->help);
}

static void cmd_execute()
{
	char *argv[16];
	unsigned int i;
	int argc = 0;
	char *cur;

	printf("\r\n");
	memset(argv, 0, sizeof(argv));

	for (cur = strtok(g_cmds.buf, " "); cur; cur = strtok(NULL, " ")) {
		if (argc >= ARRAY_SIZE(argv))
			break;
		argv[argc++] = cur;
	}

	for (i = 0; i < g_cmds.cmd_idx; i++) {
		if (!strcmp(g_cmds.cmd[i]->command, argv[0])) {
			g_cmds.cmd[i]->fn(argc, argv);
			return;
		}
	}
	printf("Unknown command: '%s'\r\n", argv[0]);
}

static void cmd_buf_reset(void)
{
	memset(g_cmds.buf, 0, sizeof(g_cmds.buf));
	g_cmds.buf_idx = 0;
}

static void cmd_buf_append(char c)
{
	g_cmds.buf[g_cmds.buf_idx++] = c;
}

/*! print the prompt to the console (stdout) */
void microvty_print_prompt(void)
{
	printf(g_cmds.prompt);
}

/*! try to receive characters from the console, dispatching them */
void microvty_try_recv(void)
{
	unsigned int i = 0;

	/* yield CPU after maximum of 10 received characters */
	while (microvty_cb_uart_rx_not_empty() && (i < 10)) {
		int c = getchar();
		if (c < 0)
			return;
		if (c == '\r' || c == '\n' || g_cmds.buf_idx >= sizeof(g_cmds.buf)-1) {
			/* skip empty commands */
			if (g_cmds.buf_idx == 0)
				return;
			cmd_execute();
			cmd_buf_reset();
			printf(g_cmds.prompt);
			return;
		} else {
			/* print + append character */
			putchar(c);
			cmd_buf_append(c);
		}

		i++;
	}
}

/* initialize microVTY. Should be called once on start-up */
void microvty_init(const char *prompt)
{
	g_cmds.prompt = prompt;
	microvty_register(&help_cmd);
}
