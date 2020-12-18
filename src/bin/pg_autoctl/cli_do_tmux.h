/*
 * src/bin/pg_autoctl/cli_do_tmux.h
 *     Implementation of a CLI which lets you run operations on the local
 *     postgres server directly.
 *
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the PostgreSQL License.
 *
 */

#ifndef CLI_DO_TMUX_H
#define CLI_DO_TMUX_H

#include "postgres_fe.h"
#include "pqexpbuffer.h"
#include "snprintf.h"

#include "cli_common.h"
#include "cli_do_root.h"
#include "cli_root.h"
#include "commandline.h"
#include "config.h"
#include "env_utils.h"
#include "log.h"
#include "pidfile.h"
#include "signals.h"
#include "string_utils.h"

typedef struct TmuxOptions
{
	char root[MAXPGPATH];
	int firstPort;
	int nodes;                  /* number of nodes per groups, total */
	int asyncNodes;             /* number of async nodes, within the total */
	int numSync;                /* number-sync-standbys */
	char layout[BUFSIZE];
} TmuxOptions;

extern TmuxOptions tmuxOptions;

void tmux_add_command(PQExpBuffer script, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));

void tmux_add_send_keys_command(PQExpBuffer script, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));

bool tmux_has_session(const char *tmux_path, const char *sessionName);
void tmux_add_new_session(PQExpBuffer script,
						  const char *root, int pgport);

void tmux_add_xdg_environment(PQExpBuffer script);
void tmux_setenv(PQExpBuffer script, const char *sessionName, const char *root);
bool tmux_prepare_XDG_environment(const char *root,
								  bool createDirectories);

void tmux_pg_autoctl_create_monitor(PQExpBuffer script,
									const char *root,
									int pgport);

void tmux_pg_autoctl_create_postgres(PQExpBuffer script,
									 const char *root,
									 int pgport,
									 const char *name,
									 bool replicationQuorum,
									 int candidatePriority);

bool tmux_start_server(const char *scriptName);
bool pg_autoctl_getpid(const char *pgdata, pid_t *pid);

bool tmux_has_session(const char *tmux_path, const char *sessionName);
bool tmux_attach_session(const char *tmux_path, const char *sessionName);
bool tmux_kill_session(TmuxOptions *options);
bool tmux_kill_session_by_name(const char *sessionName);

void tmux_process_options(TmuxOptions *options);
void tmux_cleanup_stale_directory(TmuxOptions *options);


#endif  /* CLI_DO_TMUX_H */
