/*
 * SPDX-License-Identifier: ISC
 * SPDX-URL: https://spdx.org/licenses/ISC.html
 *
 * Copyright (C) 2006-2011 William Pitcock, et al.
 * Copyright (C) 2018-2019 Atheme Development Group (https://atheme.github.io/)
 *
 * This file contains routines to handle the OperServ SET command.
 */

#include <atheme.h>

static mowgli_patricia_t **os_set_cmdtree = NULL;

static void
os_cmd_set_maxchans_func(struct sourceinfo *const restrict si, const int parc, char **const restrict parv)
{
	if (! parc)
	{
		(void) command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SET MAXCHANS");
		(void) command_fail(si, fault_needmoreparams, _("Syntax: SET MAXCHANS <value>"));
		return;
	}

	const char *const param = parv[0];
	unsigned int value;

	if (! string_to_uint(param, &value) || ! value)
	{
		(void) command_fail(si, fault_badparams, STR_INVALID_PARAMS, "SET MAXCHANS");
		(void) command_fail(si, fault_badparams, _("Syntax: SET MAXCHANS <value>"));
		return;
	}

	chansvs.maxchans = value;

	(void) command_success_nodata(si, _("You have successfully set \2%s\2 to \2%u\2."), "MAXCHANS", value);
	(void) logcommand(si, CMDLOG_ADMIN, "SET:MAXCHANS: \2%u\2", value);
}

static struct command os_cmd_set_maxchans = {
	.name           = "MAXCHANS",
	.desc           = N_("Changes the maximum number of channels one account may own."),
	.access         = PRIV_ADMIN,
	.maxparc        = 1,
	.cmd            = &os_cmd_set_maxchans_func,
	.help           = { .path = "oservice/set_maxchans" },
};

static void
mod_init(struct module *const restrict m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, os_set_cmdtree, "operserv/set_core", "os_set_cmdtree")

	(void) command_add(&os_cmd_set_maxchans, *os_set_cmdtree);
}

static void
mod_deinit(const enum module_unload_intent ATHEME_VATTR_UNUSED intent)
{
	(void) command_delete(&os_cmd_set_maxchans, *os_set_cmdtree);
}

SIMPLE_DECLARE_MODULE_V1("operserv/set_maxchans", MODULE_UNLOAD_CAPABILITY_OK)
