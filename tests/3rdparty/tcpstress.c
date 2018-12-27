/*
 * Copyright (c) 2010, Stathis Kamperis
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define _POSIX_C_SOURCE 200112L

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define BUFSIZE 32768

typedef struct request {
	const char *host;
	int port;
	size_t actconns;	/* Active connections in steady state (per-thread) */
	size_t maxconns;	/* Max connections until completion (per-thread) */
} request_t;

/* Function prototypes */
static void *thread(void *arg);
static void diep(const char *s);
static int tcpopen(const char *host, int port);
static void sendbuftosck(int sckfd, const char *buf, int len);
static void usage(const char *progname);

int
main(int argc, char *argv[])
{
	int c;
	int act, max, nthreads, errflag;

	extern char *optarg;
	extern int optind, opterr, optopt;

	/*
	 * Save program name, for passing it on usage().
	 * Don't be tempted to call getprogname(), as it isn't portable.
	 */
	const char *progname = argv[0];

	/* Parse command line arguments */
	errflag = 0;
	while ((c = getopt(argc, argv, "t:a:m:")) != -1) {
		switch(c) {
		case 't':
			nthreads = atoi(optarg);
			break;
		case 'a':
			act = atoi(optarg);
			break;
		case 'm':
			max = atoi(optarg);
			break;
		case ':':
			fprintf(stderr,
			    "Option -%c requires an operand\n", optopt);
			errflag++;
			break;
		case '?':
			fprintf(stderr,
			    "Unrecognized option: -%c\n", optopt);
			errflag++;
			break;
		default:
			errflag++;
			break;
		}
	}

	if (errflag || (argc != (optind + 2))) {
		usage(progname);
		/* NEVER REACHED */
	}

	/* Extract the host:port pair */
	const char *host = argv[optind++];
	int port = atoi(argv[optind]);

	/* Rudimentary input validation */
	if (act < 0 || max < 0 || act > max || nthreads < 1 || port < 0) {
		usage(progname);
		/* NEVER REACHED */
	}

	/* Construct a request */
	request_t req;

	req.host = host;
	req.port = port;
	req.actconns = act;
	req.maxconns = max;

	/* Allocate memory for the thread pool */
	pthread_t *tpool;

	tpool = malloc(nthreads * sizeof(pthread_t));
	if (tpool == NULL)
		diep("malloc()");

	/* Populate the thread pool */
	int i, rv;

	for (i = 0; i < nthreads; i++) {
		rv = pthread_create(&tpool[i], NULL, thread, &req);
		if (rv != 0)
			diep("pthread_create()");
	}

	/* Wait for threads to complete */
	for (i = 0; i < nthreads; i++) {
		rv = pthread_join(tpool[i], NULL);
		if (rv != 0)
			diep("pthread_join()");
	}

	return (EXIT_SUCCESS);
}

static void *
thread(void *arg)
{
	/* Extract the request */
	request_t *req = (request_t *)arg;

	/* Allocate memory for active connections */
	int *sckfd;

	sckfd = malloc(req->actconns * sizeof(int));
	if (sckfd == NULL)
		diep("malloc()");

	/* Spawn the connections and send data to server */
	char buf[BUFSIZE];
	int i, j, rv;

	for (i = 0, j = 0; i < req->maxconns; i++, j++) {
		sckfd[j] = tcpopen(req->host, req->port);

		sendbuftosck(sckfd[j], buf, sizeof(buf));

		if (j == (req->actconns - 1)) {
			rv = close(sckfd[j]);
			if (rv == -1)
				diep("close()");
			j--;
		}
	}

	/* Close the connections */
	for (i = 0; i < req->actconns - 1; i++) {
		rv = close(sckfd[i]);
		if (rv == -1)
			diep("close()");
	}

	/* Free up resources */
	free(sckfd);

	pthread_exit(NULL);
}


static int
tcpopen(const char *host, int port)
{
	struct sockaddr_in server;
	struct hostent *hp;
	int sckfd, rv;

	/* Search information for the host */
	hp = gethostbyname(host);
	if (hp == NULL)
		diep("gethostbyname()");

	/* Create an endpoint for  communication */
	sckfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sckfd < 0)
		diep("socket()");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr = *((struct in_addr *)hp->h_addr);
	memset(&(server.sin_zero), 0, sizeof(server.sin_zero));

	/* Initiate the connection on the socket */
	rv = connect(sckfd, (struct sockaddr *)&server,
	    sizeof(struct sockaddr));
	if (rv < 0)
		diep("connect()");

	return (sckfd);
}

static void
sendbuftosck(int sckfd, const char *buf, int len)
{
	int bytessent, pos;

	pos = 0;
	do {
		bytessent = send(sckfd, buf + pos, len - pos, 0);
		if (bytessent < 0)
			diep("send()");

		pos += bytessent;
	} while (bytessent > 0);
}

static void
diep(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

static void
usage(const char *progname)
{
	fprintf(stderr, "usage: %s -t threads -a active -m max host port\n",
	    progname);
	exit(EXIT_FAILURE);
}
