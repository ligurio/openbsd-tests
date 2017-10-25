#include "benchmark/benchmark.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>

void bind_perf() {

/*
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     portno = 1888;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        perror("error opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              perror("error on bind");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
     if (newsockfd < 0)
          perror("error on accept");

     bzero(buffer, 256);
     n = read(newsockfd, buffer, 255);
     if (n < 0) perror("error reading from socket");
     n = write(newsockfd, "I got your message", 18);
     if (n < 0) perror("error writing to socket");
*/
}

void BM_bind(benchmark::State& state) {
	while (state.KeepRunning()) bind_perf();
}

BENCHMARK(BM_bind);

BENCHMARK_MAIN()
