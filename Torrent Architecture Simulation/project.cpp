#include "project.h"
	
void Peer::ConnectToNode(string &Sport,string &Sip,string &fileName)
{
	int sport;
	//char buf[1024];
	string ip, files, pipp;
	struct sockaddr_in saddr;
	//socklen_t addrSize;
	cout << "Enter Server Port: ";
	cin >> sport;
	cout << "Enter Server Ip: ";
	cin >> ip;
	
	createSocket(sport);
	setAddr(saddr,sport,ip);
	//addrSize = sizeof saddr;
		
	Send(fileName);
	string x = Recieve();
	
	stringstream ss(x);
	
	getline(ss,Sport,'#');
	getline(ss,Sip,'#');
	
}	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Segment::Display()
{
	cout << this->action <<"\t" <<  this->seqNo << "\t" << this->checksum << endl;
 
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::makeNULL()
{
	seqNo = 0;
	action ="";
	checksum = "";
	data = "";
	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Peer::OperateAsCentral()
{
	bool isFound = false;
	
	cout << "\t\t\tWelcome To the Central Node\n\n";
	cout << "\t\t\tMy Port:" << this->port << endl;
	cout << "\n\t\t\tWaiting for clients...!\n";
	
	while(1)
	{
		
		string fileName = Recieve();
		
		
		Tracker t;
		t.open_File();
		

		if(t.search_File(fileName))
		{
			
			isFound = true;
			string fport,fip;
			t.get_foundPort(fport,fip);
			string pi = fport+"#"+fip+"#";
			Send(pi);
		}
		else
		{
			string msgs="NOT FOUND";
			Send(msgs);
			
		}
	}
	return isFound;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Segment::Segment(){}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
Segment::Segment(string data,int SeqNo,int AckNo,string action)
{
	setSegment(data,SeqNo,AckNo,action);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Segment::Segment(string &obj)
{
	setSegment(obj);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
string Segment::ToString()
{

	string SEQNO = to_string(this->seqNo);
	string ACKNO = to_string(this->ackNo);
	string ACTION =  this->action;
	string CHECKSUM = this->checksum;
	string DATA = this->data;
	
	string obj = ACTION + "," + SEQNO + "," + ACKNO + "," + DATA + "," +CHECKSUM  ;
	
	return obj;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Peer::OperateAsServer(int Corrupt,int Loss ,int Timeout,int WinSize)
{
	
	char buf[9999];
	
	string message = Recieve();
	
	Segment packet(message);
	//server.makePackets(packet.data);
	cout << packet.action << ": ";
	
	strcpy(buf,message.c_str());
	int action = getCommand(buf);
	
	switch(action)
	{
	
		case 2:
			
			if(doesExist(packet.data) )
			{
				cout << "OBTAINED\n";
				
				packet.action = commands[3];
				
				Send(packet.ToString());

				SendFile(packet.data, Corrupt,Loss ,Timeout,WinSize);
			}else
			{
			
				cout << "REJECTED\n";
				cout << packet.data << endl;
				packet.action = commands[4];
				Send(packet.ToString());
			}	
			
			break;
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Peer::OperateAsClient(string &FileName)
{
	
	
		
	char buf [9999];
	
	Segment packet(FileName,23,45,"SEND");
	Send(packet.ToString());
	//cout << packet.ToString() << endl;
	cout << packet.action << ": ";
	
	string message = Recieve();
	packet.setSegment(message);
	
	cout << packet.action << endl;
	
	flush(buf,sizeof (buf) );
	strcpy(buf,message.c_str() );
	int action  = getCommand(buf);
	
	//cout << action << endl;
	
	switch(action)
	{
	
		case 3:
			
			//cout << commands[action] << endl;
			RecieveFile(FileName);
			break;
		case 4:
			cout << commands[action] << endl;
			break;
	}
	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::MakeAck(int SeqNo)
{

	this->action  = "ACK";
	this->seqNo = SeqNo;
	data =checksum ="";

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::setSegment(string &obj)
{
	stringstream ss(obj);
			string x;
			
			getline(ss, x, ',');
			string tempAction =x; 
			x = "\0";
			
			getline(ss, x, ',');
			int tempSequenceNo = atoi(x.c_str());
			x = "\0";
			
			getline(ss, x, ',');
			int tempAckNo = atoi(x.c_str());
			x = "\0";
			
			getline(ss, x, ',');
			string tempData = x;
			x = "\0";
			
			getline(ss, x, ',');
			string tempCheckSum = x;
			
			setSegment(tempData,tempSequenceNo,tempAckNo,tempAction,tempCheckSum);
			
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::setSegment(string Data, int SeqNo,int AckNo, string Action)
{
	this-> seqNo = SeqNo;
	this-> action = Action;
	this-> data = Data;  
	this-> ackNo = AckNo;
	this-> checksum = getCheckSum(this->data);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::setSegment(string Data,int SeqNo,int AckNo, string Action,string &CheckSum)
{
	this-> seqNo = SeqNo;
	this-> action = Action;
	this-> data = Data;  
	this-> ackNo = AckNo;
	this-> checksum = CheckSum;
	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Segment::ISAck(string &obj)
{

	bool IsAck;
	char message[256];
	
	strcpy(message,obj.c_str());
	
	(getCommand(message)) ? IsAck=true : IsAck = false;
	
	return IsAck;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::AddError()
{
	int Size = data.length();
	for(int i=0; i<Size/2; i++)
		RShift(this->data);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Segment::RemoveError()
{	int Size = data.length();
	for(int i=0; i<Size/2; i++)
		LShift(this->data);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Segment::IsCorrupt()
{
	bool IsCorrupted;
	
	(this->checksum == (getCheckSum(this->data).c_str()) ) ? IsCorrupted= false : IsCorrupted= true;
	
	return IsCorrupted;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Peer::Peer(int port,string ip,bool IsServer)
{
			
			this->port = port;
			createSocket(socket);
			setAddr(sadr,port,ip);
			SizeOfAddr = sizeof sadr;
			if(IsServer)
			{
				bindSocket(socket,sadr);
			}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
string Peer::Recieve()
{
	string message;
	flush(buf,sizeof (buf));
	recvfrom(socket,buf,sizeof(buf),0,(struct sockaddr *) &sadr,&SizeOfAddr);
	
	copy(message,buf);
	return message;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Peer::Send(string obj)
{
	flush(buf,sizeof(buf));
	strcpy(buf,obj.c_str());
		
	sendto(socket,buf,strlen(buf),0,(struct sockaddr *) &sadr,SizeOfAddr);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
void Peer::makePackets(string fileName,int Corrupt)
{
	string data , temp ;
	
	ReadFile(fileName,data);
	
	NOP = Count(data);

	window  = new char[NOP];
	flush(window,NOP);
	
	packets = new Segment[NOP];
	
	stringstream ss(data);
	
	//cout << count << endl;
	for(int i=0; i<NOP; i++ )
	{
		getline(ss,temp,'#');
		temp +="\n";
		packets[i].setSegment(temp,i,0,"GET");
		
		if(Corrupt == i+1)
		{
			packets[i].AddError();
		}
		
		
	}
	/*for(int i=0; i<count-1; i++)
	{
		//cout << 
		packets[i].Display();
		// << endl; 
	}*/
	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Peer::SendFile(string fileName,int Corrupt,int Loss,int Timeout,int WinSize)
{
	int  FBS = 0, LBS =WinSize+ FBS;//, result;
	makePackets(fileName, Corrupt);
	fd_set rfds ,wfds , nfds;
	FD_ZERO(&rfds);
	FD_ZERO(&nfds);
	FD_ZERO(&wfds);
	struct timeval timeout;
	bool doEnd =false;
	
	while( !doEnd )
	{
	int i=FBS;
	int j=LBS;
		while( i < j )
		{
			rfds =wfds = nfds ; 
		
			FD_SET(socket, &rfds);
			FD_SET(socket, &wfds);
			
		
			select(socket+1, &rfds, &wfds, NULL, &timeout);
			
			if(FD_ISSET(socket, &wfds))
			{		
	
				cout << "SENT: " << i << endl;
			
				if( i == Loss )
				{
					Loss = -1;
				}else
				{
					Send( packets[i].ToString());
				}
				
				
				window[i]='0';
    				FD_CLR(socket, &wfds); // reset socket writable
    			}
			if(FD_ISSET(socket, &rfds))
			{		
				
     				string x = Recieve();
     				FD_SET(socket, &wfds);     //set socket wirtable
     				//cout << x << endl;
     				//cout << "Recieving...\n";
     				Segment Ack(x);
     				cout << "\t\t\t\tACK:  " << Ack.seqNo << endl;
     				window[Ack.seqNo] ='1';
     			
			}
			
			
			//sleep(1);
			
			i++;
		}
			
			while( window[FBS] == '1')
			{
				FBS++;
				LBS += (LBS != NOP) ?1:0;
				doEnd = (NOP == LBS && window[LBS-1] == '1')?1:0; 
			}
	
		sleep(Timeout);
				
	}
		
	
		Segment packet("END",0,0,"END");
		
		Send(packet.ToString() );
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Peer::RecieveFile(string fileName)
{
	string data , x ="" , loop = "GET";
	int  LPR=-1;//, WinSize = 4;//, isEnd = 0,;

		while(loop == "GET")
		{
		
			string obj = Recieve();
			Segment packet(obj);
			
			if(LPR >= packet.seqNo && packet.action =="GET")
			{
				
				cout << "\t\t\t\tPacket " << packet.seqNo <<" Duplicate Dropped" << endl;
				
			}
			else
			{
				if( packet.action == "END")
				{
					loop = packet.action;
				}else
				{
					if(packet.IsCorrupt() )
					{
						cout << "\t\t\tPacket " << packet.seqNo+1 <<  " Corrupted" << endl;
						packet.RemoveError();
						cout << "Corrupt packet Corrected" << endl;
					}
					
					data += packet.data;
					LPR  = packet.seqNo;
					packet.Display();
					//cout << "data: " << LPR << endl;
					packet.action = "ACK";
					Send(packet.ToString() );	
				}
			}
			
			
			//cout << packet.ToString() << endl;
			
			
		
		}	
			
	makeFile(fileName,data);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ReadFile(string fileName,string &data)
{

	string temp; 
	ifstream ifile;
	ifile.open(fileName.c_str(),ios::in);
	while(ifile)
	{
		getline(ifile,temp);
		data+=temp;
		data+="#";
	}
	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int Count(string data)
{
	int count  =0;
	istringstream is(data);
	string temp;
	while(getline(is,temp,'#'))
	{
		count++;
	}
	
	return count;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
void makeFile(string fileName,string data)
{
	
	ofstream ofile(fileName.c_str());
	ofile << data.c_str();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
string getCheckSum(string data)
{
	
	int RMB =15;
	string  CheckSum     ="1111111111111111";
	string Secret_Number ="1000010000001000";  

	
	RShift(data);
	RShift(CheckSum);
	XOR(CheckSum,Secret_Number);    

	for(int i=0; i<16; i++)
	{
	
		int RMB_CRC = static_cast<int>(CheckSum[RMB])-48;
		int RMB_DATA = (static_cast<int>(data[data.length()-1]))%2;

		if(RMB_CRC == RMB_DATA)
		{
			RShift(data);
			RShift(CheckSum);
		}
		else
		{
			RShift(data);
			RShift(CheckSum);
			XOR(CheckSum,Secret_Number);
		}
	}
	//cout << CheckSum << endl;
	return CheckSum;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void copy(string &x,char *p)
{

	string temp=" ";
	for(int i=0; p[i]!='\0'; i++)
	{
		temp[0]=p[i];
		x+=temp;
		
	}
	x +="\0";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void  LShift(string &operand)
{
	
	char LSB= operand[0];	
	int Size = operand.length();
	for(int i=1; i<Size ; i++ )
		operand[i-1] = operand[i];
	
	operand[operand.length()-1]= LSB;
	
	//cout << operand << endl;	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void  RShift(string &operand)
{
	
	char FSB= operand[operand.length()-1];	
	
	for(int i=operand.length()-1; i>0 ; i-- )
		operand[i] = operand[i-1];
	
	operand[0]= FSB;	
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void XOR(string &op1,string &op2)
{
	for(int i=0; i<16; i++)
	
		(op1[i] == op2[i]) ? op1[i]='0':op1[i]='1';	

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					//Functions returns true if the file exists else false 		
bool doesExist(string &filename)		
{
	fstream file(filename.c_str() );
	
	return file;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
								//the function returns true if it wasable to write the given string
								//to specific file else returns false
bool ableTowrite(string& mailfrom, string& rcptto,string & subject ,string& data)
{
	string file="";
	int x=0;
	
	
	for(int i=0; rcptto[i] != '@'; i++)
	{
		file+=rcptto[i];			///Extracting user name  from mail user@host:port   
	}
	
	if(doesExist(file) ){				//check if file exists then write to file
		
		
		x=1;					//x=1 condition true .i was able to find the specific file and write to that 
		
		ofstream ofile;	
		ofile.open(file.c_str(),ios::out|ios::app); 
		ofile << "\n\t\t\t\t\t=======================================\t\t\t\t";
		ofile << "\nTo: " << rcptto.c_str() << "\n";		// TO
		ofile << "From: " << mailfrom.c_str() << "\n";		//FROM
		ofile << "Subject: " << subject.c_str() << "\n";        //subject
		ofile <<  "Date: " << getDate();			//DATE
		ofile << "\n\t\t\t\t\t=======================================\t\t\t\t";
		ofile <<"\n\t\t\t\t\tMessage Body\t\t\t\n";
		ofile << data.c_str();					//DATA
		ofile.close();
	}

	return  x;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
string getDate()			//function returns a string containing the date and time of message sending 
{
	time_t t = time(NULL);
    	struct tm  *rtm= localtime(&t);
    	string date = asctime(rtm);
    	return date.c_str();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool check2(char* arr)			//functions returns true if a 2 exists in the given string 
{
	if(arr[0 ] == '2') 
			return true;
	else		   
			return false;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void flush(char * arr,int size)
{
	
	
	for(int i=0; i<size; i++)
	{
		arr[i]='\0'; 			//Empty  Array for resuse
	}

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void createSocket(int &Socket)
{
	Socket = socket(AF_INET,SOCK_DGRAM,0);			//create socket

	if(Socket ==-1 )
	{
		cout<<"Unabale to create socket \n";		//Inform user if unable to create socket	
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setAddr(struct sockaddr_in &sadr,int port, string ip)
{

	sadr.sin_family = AF_INET;
	sadr.sin_port = htons(port);				//	setting Addr
	sadr.sin_addr.s_addr = inet_addr(ip.c_str());
	memset(sadr.sin_zero, '\0', sizeof sadr.sin_zero);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bindSocket(int Socket,struct sockaddr_in & sadr)
{
	int siz = sizeof(sadr);
	int bindcheck = bind(Socket,(struct sockaddr *) &sadr,siz); //   binding socket with port
	if(bindcheck == -1 )
	{
		cout<<"Unable to bind the socket\n";
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int getCommand(char *message)		//so it returns 0,1,2,3 or 4  for HELO ,MAIL,RCPT,DATA or QUIT commands respectively 
{
	int command=-1; 
	char *p;

	if( (p=strstr(message,commands[0])) != NULL  )			
	{
		command =0;					
	}else
	{
		if( (p=strstr(message,commands[1])) != NULL )
		{
			command = 1;
		}else
		{
			if( (p=strstr(message,commands[2])) != NULL )
			{
				command = 2;
			}else
			{
				if( (p=strstr(message,commands[3])) != NULL )
				{
					command = 3;
				}else
				{
					if( (p=strstr(message,commands[4])) != NULL )
					{
						command = 4;
					}else
					{
						if((p=strstr(message,commands[5])) != NULL)
						{
							command = 5;
						}else
						{
							if((p=strstr(message,commands[6])) != NULL)
							{
								command = 6;
							}
						}
					}
				
				}
			
			}
		
		}
		
	}
return command;	
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Tracker::Tracker(){}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Tracker::open_File()
		{
			string line;
			ifstream configfile ("config.txt");
			if (configfile.is_open())
			{
				i=0;
				while ( getline (configfile,line) )
				{
					//cout << line << '\n';
					istringstream iss(line);
					vector<string> tokens;
					copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(tokens));

					ip[i]=tokens[0];
					port[i]=tokens[1];
					file_name[i]=tokens[2];

//					cout<<ip[i]<<"\t";
//					cout<<port[i]<<"\t";
//					cout<<file_name[i]<<endl;

					i++;
				}
				configfile.close();
			}
			else
				cout << "Unable to open file"; 
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
bool Tracker::search_File(string fname)
{
	for(i=0;i<4;i++)
	{
		if(file_name[i]==fname)
		{
			found_port=port[i];
			found_ip = ip[i];
			return true;
		}
	}
		return false;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
void Tracker::get_foundPort(string &port,string &ip)
{
	port = found_port;
	ip = found_ip;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ParseArgv(int argc, char *argv[ ],int &Port,int &Corrupt,int &Loss ,int &Timeout,int &WinSize,string &FileName)
{
  
  int Char;
  
  extern char *optarg;
  //extern int optind;//, optopt;
  
  
  while ((Char = getopt(argc, argv, "C:L:T:F:P:W:")) != -1)
  {
	switch (Char)
	{
	case 'P':
		Port =atoi(optarg);
		break;
	case 'C':
		Corrupt =atoi(optarg);
		
		break;	  

	case 'L':
		Loss = atoi(optarg);
		
		break;
	case 'T':
		Timeout = atoi(optarg);
		break;
	case 'F':
		FileName = *optarg;
		break;
	case 'W':
		WinSize = atoi(optarg);
		break;
	
	
	}
  }
cout << Corrupt << "  " << Loss << "  " << Timeout << "  " << Port << "  " <<FileName << endl;
}
	
