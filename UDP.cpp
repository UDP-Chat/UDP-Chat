/*
 * UDP.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "UDP.h"
#include "global.h"

UDP::UDP() {
	this->port=0;
}

UDP::~UDP() {

}

/* Code referenced from http://www.xinotes.net/notes/note/1810/ */
ssize_t UDP::start_listen(string name) {

    char buf[BUF_SIZE];
    struct sockaddr_in selfAddress, otherAddress;
    socklen_t len = sizeof(struct sockaddr_in);
    ssize_t n, s;


    /* initialize socket */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        return 1;
    }

    /* bind to server port */
    memset((char *) &selfAddress, 0, sizeof(struct sockaddr_in));
    selfAddress.sin_family = AF_INET;
    selfAddress.sin_port = 0;
    selfAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &selfAddress, sizeof(selfAddress)) == -1) {
        perror("bind");
        return 1;
    }


    if (getsockname(s, (struct sockaddr *)&selfAddress, &len) == -1) {
        perror("getsockname");
        return 0;
    }

//  Bob started a new chat, listening on 192.168.5.2:7432 Succeeded, current users:
//	Bob 192.168.5.2.7432 (Leader)
//	Waiting for others to join...
	//getsockname(s, (struct sockaddr *) &self, &len);


    this->ip=this->getLocalIP();
    this->port=ntohs(selfAddress.sin_port);

	cout << name << " started a new chat, listening on "
			<< this->ip
			<< ":" << this->port << endl;
	cout << "Succeeded, current users:" << endl;

	while ((n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &otherAddress, &len)) != -1) {
		char received_buf[n];
		memcpy(received_buf, buf, n);
		std::thread t(&BufferParser::parse_buffer, bufferParser, (char*)received_buf);
	}


    close(s);

    return 0;
}

void UDP::test(int i){

}

string UDP::getLocalIP(){
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
