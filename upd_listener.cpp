/*
 * main_listener.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: yuexi
 */

#include "upd_listener.h"

/* Code referenced from http://www.xinotes.net/notes/note/1810/ */
int start(ssize_t port) {

    printf("server starting\n");

    char buf[BUF_SIZE];
    struct sockaddr_in self, other;
    socklen_t len = sizeof(struct sockaddr_in);
    ssize_t n, s;


    /* initialize s ocket */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        return 1;
    }

    /* bind to server port */
    memset((char *) &self, 0, sizeof(struct sockaddr_in));
    self.sin_family = AF_INET;
    self.sin_port = htons(port);
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &self, sizeof(self)) == -1) {
        perror("bind");
        return 1;
    }

    printf("server started\n");


    while ((n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &other, &len)) != -1) {
    	char received_buf[n];
    	memcpy(received_buf, buf, n);
		pthread_t pid;
		pthread_create(&pid, NULL, parse_buffer, (void*) received_buf);
    }


    close(s);

    return 0;
}





