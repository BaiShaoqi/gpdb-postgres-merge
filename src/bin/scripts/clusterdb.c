/*-------------------------------------------------------------------------
 *
 * clusterdb
 *
 * Portions Copyright (c) 2002-2009, PostgreSQL Global Development Group
 *
 * $PostgreSQL: pgsql/src/bin/scripts/clusterdb.c,v 1.22 2008/12/11 07:34:08 petere Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres_fe.h"
#include "common.h"
#include "dumputils.h"


static void cluster_one_database(const char *dbname, bool verbose, const char *table,
					 const char *host, const char *port,
					 const char *username, enum trivalue prompt_password,
					 const char *progname, bool echo);
static void cluster_all_databases(bool verbose, const char *host, const char *port,
<<<<<<< HEAD
					  const char *username, enum trivalue prompt_password,
=======
					  const char *username, bool password,
>>>>>>> 38e9348282e
					  const char *progname, bool echo, bool quiet);

static void help(const char *progname);


int
main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"host", required_argument, NULL, 'h'},
		{"port", required_argument, NULL, 'p'},
		{"username", required_argument, NULL, 'U'},
		{"no-password", no_argument, NULL, 'w'},
		{"password", no_argument, NULL, 'W'},
		{"echo", no_argument, NULL, 'e'},
		{"quiet", no_argument, NULL, 'q'},
		{"dbname", required_argument, NULL, 'd'},
		{"all", no_argument, NULL, 'a'},
		{"table", required_argument, NULL, 't'},
		{"verbose", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};

	const char *progname;
	int			optindex;
	int			c;

	const char *dbname = NULL;
	char	   *host = NULL;
	char	   *port = NULL;
	char	   *username = NULL;
	enum trivalue prompt_password = TRI_DEFAULT;
	bool		echo = false;
	bool		quiet = false;
	bool		alldb = false;
	char	   *table = NULL;
	bool		verbose = false;

	progname = get_progname(argv[0]);
	set_pglocale_pgservice(argv[0], PG_TEXTDOMAIN("pgscripts"));

	handle_help_version_opts(argc, argv, "clusterdb", help);

<<<<<<< HEAD
	while ((c = getopt_long(argc, argv, "h:p:U:wWeqd:at:v", long_options, &optindex)) != -1)
=======
	while ((c = getopt_long(argc, argv, "h:p:U:Weqd:at:v", long_options, &optindex)) != -1)
>>>>>>> 38e9348282e
	{
		switch (c)
		{
			case 'h':
				host = optarg;
				break;
			case 'p':
				port = optarg;
				break;
			case 'U':
				username = optarg;
				break;
			case 'w':
				prompt_password = TRI_NO;
				break;
			case 'W':
				prompt_password = TRI_YES;
				break;
			case 'e':
				echo = true;
				break;
			case 'q':
				quiet = true;
				break;
			case 'd':
				dbname = optarg;
				break;
			case 'a':
				alldb = true;
				break;
			case 't':
				table = optarg;
				break;
<<<<<<< HEAD
			/*
			 * -v accepted for PostgreSQL compatability, although GPDB
			 * doesn't actually handle CLUSTER VERBOSE.
			 */
=======
>>>>>>> 38e9348282e
			case 'v':
				verbose = true;
				break;
			default:
				fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
				exit(1);
		}
	}

	switch (argc - optind)
	{
		case 0:
			break;
		case 1:
			dbname = argv[optind];
			break;
		default:
			fprintf(stderr, _("%s: too many command-line arguments (first is \"%s\")\n"),
					progname, argv[optind + 1]);
			fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
			exit(1);
	}

	setup_cancel_handler();

	if (alldb)
	{
		if (dbname)
		{
			fprintf(stderr, _("%s: cannot cluster all databases and a specific one at the same time\n"),
					progname);
			exit(1);
		}
		if (table)
		{
			fprintf(stderr, _("%s: cannot cluster a specific table in all databases\n"),
					progname);
			exit(1);
		}

<<<<<<< HEAD
		cluster_all_databases(verbose, host, port, username, prompt_password,
=======
		cluster_all_databases(verbose, host, port, username, password,
>>>>>>> 38e9348282e
							  progname, echo, quiet);
	}
	else
	{
		if (dbname == NULL)
		{
			if (getenv("PGDATABASE"))
				dbname = getenv("PGDATABASE");
			else if (getenv("PGUSER"))
				dbname = getenv("PGUSER");
			else
				dbname = get_user_name(progname);
		}

		cluster_one_database(dbname, verbose, table,
<<<<<<< HEAD
							 host, port, username, prompt_password,
=======
							 host, port, username, password,
>>>>>>> 38e9348282e
							 progname, echo);
	}

	exit(0);
}


static void
cluster_one_database(const char *dbname, bool verbose, const char *table,
					 const char *host, const char *port,
					 const char *username, enum trivalue prompt_password,
					 const char *progname, bool echo)
{
	PQExpBufferData sql;

	PGconn	   *conn;

	initPQExpBuffer(&sql);

	appendPQExpBuffer(&sql, "CLUSTER");
	if (verbose)
		appendPQExpBuffer(&sql, " VERBOSE");
	if (table)
		appendPQExpBuffer(&sql, " %s", fmtId(table));
	appendPQExpBuffer(&sql, ";\n");

	conn = connectDatabase(dbname, host, port, username, prompt_password, progname);
	if (!executeMaintenanceCommand(conn, sql.data, echo))
	{
		if (table)
			fprintf(stderr, _("%s: clustering of table \"%s\" in database \"%s\" failed: %s"),
					progname, table, dbname, PQerrorMessage(conn));
		else
			fprintf(stderr, _("%s: clustering of database \"%s\" failed: %s"),
					progname, dbname, PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	PQfinish(conn);
	termPQExpBuffer(&sql);
}


static void
cluster_all_databases(bool verbose, const char *host, const char *port,
<<<<<<< HEAD
					  const char *username, enum trivalue prompt_password,
=======
					  const char *username, bool password,
>>>>>>> 38e9348282e
					  const char *progname, bool echo, bool quiet)
{
	PGconn	   *conn;
	PGresult   *result;
	int			i;

	conn = connectDatabase("postgres", host, port, username, prompt_password, progname);
	result = executeQuery(conn, "SELECT datname FROM pg_database WHERE datallowconn ORDER BY 1;", progname, echo);
	PQfinish(conn);

	for (i = 0; i < PQntuples(result); i++)
	{
		char	   *dbname = PQgetvalue(result, i, 0);

		if (!quiet)
		{
			printf(_("%s: clustering database \"%s\"\n"), progname, dbname);
			fflush(stdout);
		}

		cluster_one_database(dbname, verbose, NULL,
<<<<<<< HEAD
							 host, port, username, prompt_password,
=======
							 host, port, username, password,
>>>>>>> 38e9348282e
							 progname, echo);
	}

	PQclear(result);
}


static void
help(const char *progname)
{
	printf(_("%s clusters all previously clustered tables in a database.\n\n"), progname);
	printf(_("Usage:\n"));
	printf(_("  %s [OPTION]... [DBNAME]\n"), progname);
	printf(_("\nOptions:\n"));
	printf(_("  -a, --all                 cluster all databases\n"));
	printf(_("  -d, --dbname=DBNAME       database to cluster\n"));
	printf(_("  -e, --echo                show the commands being sent to the server\n"));
	printf(_("  -q, --quiet               don't write any messages\n"));
<<<<<<< HEAD
	printf(_("  -t, --table=TABLE         cluster specific table only\n"));
	//printf(_("  -v, --verbose             write a lot of output\n"));
=======
	printf(_("  -v, --verbose             write a lot of output\n"));
>>>>>>> 38e9348282e
	printf(_("  --help                    show this help, then exit\n"));
	printf(_("  --version                 output version information, then exit\n"));
	printf(_("\nConnection options:\n"));
	printf(_("  -h, --host=HOSTNAME       database server host or socket directory\n"));
	printf(_("  -p, --port=PORT           database server port\n"));
	printf(_("  -U, --username=USERNAME   user name to connect as\n"));
	printf(_("  -w, --no-password         never prompt for password\n"));
	printf(_("  -W, --password            force password prompt\n"));
	printf(_("\nRead the description of the SQL command CLUSTER for details.\n"));
	printf(_("\nReport bugs to <bugs@greenplum.org>.\n"));
}
