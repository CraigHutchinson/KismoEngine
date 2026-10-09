/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoNET\Socket.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoNetwork/Socket.cpp (same name, content differs)
 */
#include "Socket.h"

#include <iostream>

static Count l_socketCount = 0; ///< Socket instance counter for WinSock initialisation/cleanup detection

void initialiseWSASockets()
{	
	if ( !l_socketCount ) //Only initialise WinSock on the first socket creation
	{
		const int kRequiredWinsockVersion = 2;   // Minimum winsock version required
		WSADATA wsaData;
		if ( WSAStartup( MAKEWORD(kRequiredWinsockVersion,2), &wsaData) == 0 )
		{
			// Check if major version is at least iReqWinsockVer
			if ( LOBYTE(wsaData.wVersion) < kRequiredWinsockVersion )
			{
				// Required version not available... Cleanup winsock
				if ( WSACleanup()!=0 )
				{
					// cleanup failed
				}
			}

		}
		else
		{
			//  startup failed
		}
	}

	++l_socketCount;
}

void destroyWSASockets()
{	
	--l_socketCount;
	if ( !l_socketCount )
	{
		//Cleanup winsock
		if ( WSACleanup()!=0 )
		{
			// cleanup failed
		}
	}
}

namespace kismo
{
	Socket::Socket() : m_handle(INVALID_SOCKET)
	{
	}

	/*
	Socket::Socket(SOCKET s) : m_handle(s)
	{
		initialiseSocket();
	}
	*/

	bool Socket::open()
	{
		if ( m_handle != INVALID_SOCKET )
		{
			assert( false == "Socket already open!");
			return true;
		}

		//Initialise winsock if necessary
		incrementWSASocket();

		// UDP: use SOCK_DGRAM instead of SOCK_STREAM
		m_handle = socket( AF_INET, SOCK_STREAM, 0 );

		if ( m_handle == INVALID_SOCKET ) 
		{
			throw "INVALID_SOCKET";
		}

	}


	Socket::~Socket() 
	{
		close()
	}

	void Socket::close() 
	{
		if ( m_handle == INVALID_SOCKET ) 
			return;

		closesocket( m_handle );
		decrementWSASocket();
	}

	//std::string Socket::ReceiveBytes() 
	//{
	//	std::string ret;
	//	char buf[1024];

	//	while (1) {
	//		u_long arg = 0;
	//		if (ioctlsocket(m_handle, FIONREAD, &arg) != 0)
	//			break;

	//		if (arg == 0)
	//			break;

	//		if (arg > 1024) arg = 1024;

	//		int rv = recv (m_handle, buf, arg, 0);
	//		if (rv <= 0) break;

	//		std::string t;

	//		t.assign (buf, rv);
	//		ret += t;
	//	}

	//	return ret;
	//}

	//std::string Socket::ReceiveLine() {
	//	std::string ret;
	//	while (1) {
	//		char r;

	//		switch(recv(m_handle, &r, 1, 0)) {
	//	  case 0: // not connected anymore;
	//		  // ... but last line sent
	//		  // might not end in \n,
	//		  // so return ret anyway.
	//		  return ret;
	//	  case -1:
	//		  return "";
	//		  //      if (errno == EAGAIN) {
	//		  //        return ret;
	//		  //      } else {
	//		  //      // not connected anymore
	//		  //      return "";
	//		  //      }
	//		}

	//		ret += r;
	//		if (r == '\n')  return ret;
	//	}
	//}

	//void Socket::SendLine(std::string s) {
	//	s += '\n';
	//	send(m_handle,s.c_str(),s.length(),0);
	//}

	//void Socket::SendBytes(const std::string& s) {
	//	send(m_handle,s.c_str(),s.length(),0);
	//}

	//SocketServer::SocketServer(int port, int connections, TypeSocket type) {
	//	sockaddr_in sa;

	//	memset(&sa, 0, sizeof(sa));

	//	sa.sin_family = PF_INET;             
	//	sa.sin_port = htons(port);          
	//	m_handle = socket(AF_INET, SOCK_STREAM, 0);
	//	if (m_handle == INVALID_SOCKET) {
	//		throw "INVALID_SOCKET";
	//	}

	//	if(type==NonBlockingSocket) {
	//		u_long arg = 1;
	//		ioctlsocket(m_handle, FIONBIO, &arg);
	//	}

	//	/* bind the socket to the internet address */
	//	if (bind(m_handle, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
	//		closesocket(m_handle);
	//		throw "INVALID_SOCKET";
	//	}

	//	listen(m_handle, connections);                               
	//}

	//Socket* SocketServer::Accept() {
	//	SOCKET new_sock = accept(m_handle, 0, 0);
	//	if (new_sock == INVALID_SOCKET) {
	//		int rc = WSAGetLastError();
	//		if(rc==WSAEWOULDBLOCK) {
	//			return 0; // non-blocking call, no request pending
	//		}
	//		else {
	//			throw "Invalid Socket";
	//		}
	//	}

	//	Socket* r = new Socket(new_sock);
	//	return r;
	//}

	//SocketClient::SocketClient(const std::string& host, int port) : Socket() {
	//	std::string error;

	//	hostent *he;
	//	if ((he = gethostbyname(host.c_str())) == 0) {
	//		error = strerror(errno);
	//		throw error;
	//	}

	//	sockaddr_in addr;
	//	addr.sin_family = AF_INET;
	//	addr.sin_port = htons(port);
	//	addr.sin_addr = *((in_addr *)he->h_addr);
	//	memset(&(addr.sin_zero), 0, 8); 

	//	if (::connect(m_handle, (sockaddr *) &addr, sizeof(sockaddr))) {
	//		error = strerror(WSAGetLastError());
	//		throw error;
	//	}
	//}

	//SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type) 
	//{
	//	FD_ZERO(&fds_);
	//	FD_SET(const_cast<Socket*>(s1)->m_handle,&fds_);
	//	if(s2) 
	//	{
	//		FD_SET(const_cast<Socket*>(s2)->m_handle,&fds_);
	//	}     

	//	TIMEVAL tval;
	//	tval.tv_sec  = 0;
	//	tval.tv_usec = 1;

	//	TIMEVAL *ptval;
	//	if(type==NonBlockingSocket) {
	//		ptval = &tval;
	//	}
	//	else { 
	//		ptval = 0;
	//	}

	//	if ( select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR ) 
	//		throw "Error in select";
	//}

	//bool SocketSelect::Readable(Socket const* const s) 
	//{
	//	if ( FD_ISSET(s->m_handle,&fds_) ) 
	//		return true;
	//	return false;
	//}

} //END: kismo