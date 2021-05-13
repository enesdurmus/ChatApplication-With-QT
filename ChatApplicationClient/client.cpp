#include "client.h"
#include "thread"
#include <iostream>
#include <string>

using namespace std;

SOCKET Client::ourSocket;

int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddressA(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}

Client::Client(){

    // init winsock
    wsResult = WSAStartup(ver, &wsData);
    if(wsResult != 0){
        cout << "Initialize Winsock problem! Quitting..." << wsResult << endl;
        return;
    }
    cout << "Init winsock..." << endl;

    //Create Socket
    ourSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ourSocket == INVALID_SOCKET){
        cout << "Create Socket problem! Quitting..." << WSAGetLastError() << endl;
        return;
    }
    cout << "Socket Created..." << endl;

    //Fill a hint structure
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
}

void Client::ConnectToServer(){
    int conResult= connect(ourSocket, (sockaddr*)&hint, sizeof (hint));
    if(conResult == SOCKET_ERROR){
        cout << "Couldn't connect to server , err :" << WSAGetLastError() << endl;
        closesocket(ourSocket);
        WSACleanup();
        return;
    }
    cout << "Connection is succesfull" << endl;
}
