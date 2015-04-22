/*
 * main_listener.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: yuexi
 */

#include "upd_listener.h"

/* Code referenced from http://www.xinotes.net/notes/note/1810/ */
int start(string name) {

    char buf[BUF_SIZE];
    struct sockaddr_in self, other;
    socklen_t len = sizeof(struct sockaddr_in);
    ssize_t n, s;


    /* initialize socket */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        return 1;
    }

    /* bind to server port */
    memset((char *) &self, 0, sizeof(struct sockaddr_in));
    self.sin_family = AF_INET;
    self.sin_port = 0;
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &self, sizeof(self)) == -1) {
        perror("bind");
        return 1;
    }


    if (getsockname(s, (struct sockaddr *)&self, &len) == -1) {
        perror("getsockname");
        return 0;
    }

//  Bob started a new chat, listening on 192.168.5.2:7432 Succeeded, current users:
//	Bob 192.168.5.2.7432 (Leader)
//	Waiting for others to join...
	//getsockname(s, (struct sockaddr *) &self, &len);


	cout << name << " started a new chat, listening on "
			<< getLocalIP()
			<< ":" << ntohs(self.sin_port) << endl;
	cout << "Succeeded, current users:" << endl;

//	while ((n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &other, &len)) != -1) {
//		char received_buf[n];
//		memcpy(received_buf, buf, n);
//		pthread_t pid;
//		pthread_create(&pid, NULL, parse_buffer, (void*) received_buf);
//	}


    close(s);

    return 0;
}

string getLocalIP(){
	const char* google_dns_server = "8.8.8.8";
	int dns_port = 53;
	struct sockaddr_in serv;
	int sock = socket ( AF_INET, SOCK_DGRAM, 0);
	char buf[BUF_SIZE];

	//Socket could not be created
	if(sock < 0)
	{
		perror("Socket error");
	}

	memset( &serv, 0, sizeof(serv) );
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr( google_dns_server );
	serv.sin_port = htons( dns_port );

	int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );
	if(err<0){
		perror("Connect error");
	}

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*) &name, &namelen);

	string res(inet_ntop(AF_INET,&name.sin_addr,buf,100));
	return res;
}




