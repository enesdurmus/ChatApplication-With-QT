#include <iostream>
#include "server.h"

using namespace std;

int main()
{
    Server *server = new Server();
    server->Start();
    return 0;
}
