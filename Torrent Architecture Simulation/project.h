#ifndef PROJECT
#define PROJECT
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <vector>
#include<errno.h>
#include<stdio.h>

using namespace std;

static char commands[][10] = {"DATA","ACK","SEND","ACCEPTED","NO FILE","END",""};

class Segment
{
	public:
		int seqNo,ackNo;
		string action;
		string checksum;
		string data;
	
		void Display();	
		Segment();
		void RemoveError();
		void AddError();
		void makeNULL();
		bool IsCorrupt();
		string ToString();
		Segment(string &obj);
		bool ISAck(string &obj);
		void MakeAck(int SeqNo);
		void setSegment(string &obj);
		Segment(string data,int SeqNo,int AckNo,string Action);
		void setSegment(string Data,int SeqNo,int AckNo, string Action);
		void setSegment(string Data,int SeqNo,int AckNo, string Action,string &CheckSum);
		
		
		

};

class Tracker
{
	public:
		string ip[100],port[100],file_name[100];
		string found_port;
		string found_ip;
		int i;
	
		Tracker();
		void get_foundPort(string &port,string &ip);
		void open_File();
		bool search_File(string fname);
		
	
};

class Peer
{
	public:
	
		Segment *packets;
		char *window;	
		int socket , port, NOP;
		char buf[9999];
		struct sockaddr_in sadr;
		socklen_t SizeOfAddr; 
		
		
		string Recieve();
		void OperateAsServer(int Corrupt,int Loss ,int Timeout,int WinSize);
		void OperateAsClient(string &FileName);
		void ConnectToNode(string& port,string &ip,string &fileName);
		void RecieveFile(string fileName);
		void Send(string obj);
		void RecieveAck();
		void SendFile(string fileName,int Corrupt,int Loss,int Timeout,int Winsize);
		Peer(int port,string ip,bool IsServer);
		void makePackets(string fileName,int Corrupt);
		bool OperateAsCentral();
		
		
		

};


	string getDate();
	bool check2(char* arr);
	void flush(char * arr,int size);
	int Count(string data);
	bool IsAck(string &obj);
	void  RShift(string &data);
	void  LShift(string &data);
	void copy(string &x,char *p);
	int getCommand(char *message);
	void createSocket(int &Socket);
	string getCheckSum(string data);
	bool doesExist(string &filename);
	void XOR(string &crc,string &poly);
	void makeFile(string fileName,string data);
	void ReadFile(string fileName, string &data);
	void bindSocket(int Socket,struct sockaddr_in & sadr);
	void setAddr(struct sockaddr_in &sadr,int port,string ip);
	void ParseArgv(int argc, char *argv[ ],int &port,int &Corrupt,int &Loss ,int &Timeout,int &WinSize,string &FileName );
	bool ableTowrite(string& mailfrom, string& rcptto,string & subject,string& data);

#endif
