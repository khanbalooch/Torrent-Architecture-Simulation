#include "project.h"

using namespace std;


int main ()
{
	cout << commands[7] << endl;
	
	Peer Node(1234,"127.0.0.1",true);
	
	Node.OperateAsCentral();
	
	return 0;
}
