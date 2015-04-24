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
	started=false;
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

    this->ip=this->getLocalIP();
    this->port=ntohs(selfAddress.sin_port);
    this->processID=this->ip+":"+to_string(this->port);



	started=true;

	while ((n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &otherAddress, &len)) != -1) {
//		cout << "received connection" << endl;
		char received_buf[sizeof(Message)];
		memcpy(received_buf, buf, n);

		Message* msg_in=(Message*)received_buf;

		if(encrypt){
			string data=string(msg_in->data);
			data=messageStore->decrypt(data);
			memcpy(msg_in->data,data.c_str(),data.length()+1);
		}

		std::thread t(&BufferParser::parse_buffer, bufferParser, msg_in);
		t.detach();
	}


    close(s);

    return 0;
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


int UDP::send_msg(string processID, Message message){
	return this->send_msg(messageStore->getIP(processID), messageStore->getPort(processID), message);
}

int UDP::send_msg(string host_in, string port_in, Message msg_in){
		if(encrypt){
			string data=string(msg_in.data);

//string temp=data;
//cout<<"before encrypt: "+temp<<endl;

			string encrypted=messageStore->encrypt(data);

			memcpy(msg_in.data,encrypted.c_str(),strlen(encrypted.c_str())+1);

//string temp2=messageStore->decrypt(string(msg_in.data));
//cout<<"after encrypt: "+temp2<<endl;
//bool same=temp.compare(temp2)==0;
//string s=same?"t":"f";
//cout<<"after encrypt: "+s<<endl;
		}


		const char* host_addr=host_in.c_str();
		int port=atoi(port_in.c_str());
		//const char* msg=msg_in.c_str();

	//    printf("client starting");

	    struct sockaddr_in server;
	    int len = sizeof(struct sockaddr_in);
	    struct hostent *host;
	    int s;



	    host = gethostbyname(host_addr);
	    if (host == NULL) {
	        perror("gethostbyname");
	        return 1;
	    }


	    /* initialize socket */
	    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	        perror("socket");
	        return 1;
	    }

	    /* initialize server addr */
	    memset((char *) &server, 0, sizeof(struct sockaddr_in));
	    server.sin_family = AF_INET;
	    server.sin_port = htons(port);
	    server.sin_addr = *((struct in_addr*) host->h_addr);

	    /* send message */
	    //if (sendto(s, msg, strlen(msg), 0, (struct sockaddr *) &server, len) == -1) {
		if (sendto(s, &msg_in, sizeof(Message), 0, (struct sockaddr *) &server, len) == -1) {
	        perror("sendto()");
	        return 1;
	    }

	    close(s);
	    return 0;

}

