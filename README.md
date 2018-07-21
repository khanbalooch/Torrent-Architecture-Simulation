******************************  INFORMATION  ******************************

AUTHORS:		Muhammad Ibrahim Khan (i13-1821)
VARSION:		1.0
COPYRIGHT:		(c) Reserved
DESCRIPTION:		RDT Protocol with Peer2Peer Architecture

***************************************************************************


*******************************  FEATURES  ********************************

Our program supports multiple peers which works on RDT Protocol for
getting files and consists of a central node containing tracker file.

***************************************************************************


******************************  HOW TO RUN  *******************************

-> Open Terminal.
-> Navigate to the folder of the Project
-> Execute MakeFile which compiles the files
-> Open one terminal for Central Node
-> Open multiple terminals for multiple peers

***************************************************************************


*******************************  COMMANDS  ********************************

-> Run Central Node by using the ./node command
-> Run the peer by using ./peer command with the following command line
   arguments:

	T: Timeout
	C: Corrupt
	L: Loss
	F: Filename
	P: Port

	e.g. ./peer -T1 -C1 -L1 A.txt 0000
	
***************************************************************************


************************* IMPLEMENTED FUNCTIONS  **************************

For Segments:	void Display();	
		Segment();
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

For Tracker:	Tracker();
		void get_foundPort(string &port,string &ip);
		void open_File();
		bool search_File(string fname);

For Peer:	void OperateAsClient();
		void ConnectToNode(string& port,string &ip);
		void RecieveFile(string fileName);
		void Send(string obj);
		void RecieveAck();
		void SendFile(string fileName);
		Peer(int port,string ip,bool IsServer);
		void makePackets(string fileName);
		bool OperateAsCentral();

***************************************************************************
