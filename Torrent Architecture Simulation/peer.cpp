#include "project.h"


int main(int argc ,char *argv[])
{
	int Corrupt,Loss ,Timeout,Port,WinSize;
	string FileName;
	cout << commands[7] << endl;
	ParseArgv(argc,argv,Port,Corrupt,Loss,Timeout,WinSize,FileName);
	string fname =FileName;
	
	
	Peer client1(1234,"127.0.0.1",false);
	string Sport,Sip;
	client1.ConnectToNode(Sport,Sip,FileName);
	
	//cout << "My client port and IP: " << Sport << "   " << Sip << endl;
	
	int isParent = getpid();
	
	Peer client(atoi(Sport.c_str()),Sip,false);
	
	Peer server(Port,"127.0.0.1",true);
	
	
	
	fork();
	
	
	if( isParent != getpid())
	{
		int  isChild1 = fork();
		
		
		if( isChild1 == 0 )
		{
			
			//string allow;
			//cout << "Press Enter to start Downloading and Uploading if Any\n";
			//cin >> allow;
	
			cout << "waiting for clients ...\n";
	
			
			server.OperateAsServer(Corrupt,Loss ,Timeout,WinSize);	
		
		}	
	
	}else
	{
			
		client.OperateAsClient(fname);
		
	}
		 
}
