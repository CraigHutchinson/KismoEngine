/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoNetwork\Socket.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMONETWORK_SOCKET_H
#define KISMONETWORK_SOCKET_H

//#include "KismoFramework/Platform.h"
#include "KismoFramework/Types.h"
#include "KismoNetwork\Common.h"

#include <winsock2.h>
#include <ws2tcpip.h>

namespace kismo { namespace network
{
		
	class Socket 
	{
	public:

		Socket(SOCKET s);
		Socket();
		virtual ~Socket();

		// Socket(const Socket&);
		// Socket& operator=(Socket&);

		bool create();
		void destroy();

		//void listen( int connections )
		//{
		//	listen(m_handle, connections);
		//}

		/**
			\brief Bind the socket to a port
			\remarks Should be called before a call to listen().
			\par By default binds wihtout IP so the port is active
			on allconnected networks.
		*/
		//bool bind( int port, bool blocking = true  )
		//{
		//	if ( m_handle == INVALID_SOCKET 
		//		&& !create() ) 
		//	{
		//		return false;
		//	}

		//	Assert( m_handle != INVALID_SOCKET );

		//	sockaddr_in addr;
		//	memset(&addr, 0, sizeof(addr));
		//	addr.sin_family = PF_INET;             
		//	addr.sin_port = htons(port);    
		//	//TODO: we could bind to a speicifc IP/device

		//	if( blocking )
		//	{
		//		u_long arg = 1;
		//		ioctlsocket(m_handle, FIONBIO, &arg);
		//	}

		//	// bind the socket to the internet address
		//	if ( bind(m_handle, (sockaddr *)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) 
		//	{
		//		close();
		//		throw "INVALID_SOCKET";
		//		return false;
		//	}

		//	return true;
		//}

		///**
		//	\brief
		//	\param[in] host Host name or IP as numeric host address string as dotted-decimal IPv4 address or an IPv6 hex address.
		//	\param[in] portService Optional null terminated string service-name or port number represented as a string. A service name is a string 
		//		alias for a port number. For example, “http” is an alias for port 80 defined by the Internet Engineering Task Force (IETF) 
		//		as the default port used by web servers for the HTTP protocol.
		//*/
		//bool getAddressInfo( const String& host, const String& portService = String() )
		//{
		//	
		//	// Setup the hints address info structure which is passed to the getaddrinfo() function
		//	struct addrinfo socketHints;
		//	ZeroMemory( &hints, sizeof(hints) );
		//	hints.ai_family = AF_UNSPEC;
		//	hints.ai_socktype = SOCK_STREAM;
		//	hints.ai_protocol = IPPROTO_TCP;

		//	// Call getaddrinfo(). If the call succeeds, the result variable will hold a linked list of addrinfo structures containing response information
		//	dwRetval = getaddrinfo( host.c_str(), host.c_str(), &socketHints, &result);
		//	if ( dwRetval != 0 ) 
		//	{
		//		printf("getaddrinfo failed with error: %d\n", dwRetval);
		//		return false;
		//	}

		//	// Retrieve each address and print out the hex bytes
		//	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		//		printf("getaddrinfo response %d\n", i++);
		//		printf("\tFlags: 0x%x\n", ptr->ai_flags);
		//		printf("\tFamily: ");
		//		switch (ptr->ai_family) {
		//			case AF_UNSPEC:
		//				printf("Unspecified\n");
		//				break;
		//			case AF_INET:
		//				printf("AF_INET (IPv4)\n");
		//				sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
		//				printf("\tIPv4 address %s\n",
		//					inet_ntoa(sockaddr_ipv4->sin_addr) );
		//				break;
		//			case AF_INET6:
		//				printf("AF_INET6 (IPv6)\n");
		//				// the InetNtop function is available on Windows Vista and later
		//				// sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
		//				// printf("\tIPv6 address %s\n",
		//				//    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
		//                
		//				// We use WSAAddressToString since it is supported on Windows XP and later
		//				sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
		//				// The buffer length is changed by each call to WSAAddresstoString
		//				// So we need to set it for each iteration through the loop for safety
		//				ipbufferlength = 46;
		//				iRetval = WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, 
		//					ipstringbuffer, &ipbufferlength );
		//				if (iRetval)
		//					printf("WSAAddressToString failed with %u\n", WSAGetLastError() );
		//				else    
		//					printf("\tIPv6 address %s\n", ipstringbuffer);
		//				break;
		//			case AF_NETBIOS:
		//				printf("AF_NETBIOS (NetBIOS)\n");
		//				break;
		//			default:
		//				printf("Other %ld\n", ptr->ai_family);
		//				break;
		//		}
		//		printf("\tSocket type: ");
		//		switch (ptr->ai_socktype) {
		//			case 0:
		//				printf("Unspecified\n");
		//				break;
		//			case SOCK_STREAM:
		//				printf("SOCK_STREAM (stream)\n");
		//				break;
		//			case SOCK_DGRAM:
		//				printf("SOCK_DGRAM (datagram) \n");
		//				break;
		//			case SOCK_RAW:
		//				printf("SOCK_RAW (raw) \n");
		//				break;
		//			case SOCK_RDM:
		//				printf("SOCK_RDM (reliable message datagram)\n");
		//				break;
		//			case SOCK_SEQPACKET:
		//				printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
		//				break;
		//			default:
		//				printf("Other %ld\n", ptr->ai_socktype);
		//				break;
		//		}
		//		printf("\tProtocol: ");
		//		switch (ptr->ai_protocol) {
		//			case 0:
		//				printf("Unspecified\n");
		//				break;
		//			case IPPROTO_TCP:
		//				printf("IPPROTO_TCP (TCP)\n");
		//				break;
		//			case IPPROTO_UDP:
		//				printf("IPPROTO_UDP (UDP) \n");
		//				break;
		//			default:
		//				printf("Other %ld\n", ptr->ai_protocol);
		//				break;
		//		}
		//		printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
		//		printf("\tCanonical name: %s\n", ptr->ai_canonname);
		//	}

		//	freeaddrinfo(result);
		//}


		/**
			\brief Connects to a port on the specified host machines socket
			\remarks THis function will fail if the host mahcine is not listening on the specified port
		*/
	/*	void connect( const String& host, int port )
		{
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr = *((in_addr *)he->h_addr);
			memset(&(addr.sin_zero), 0, 8); 

			if ( connect( m_handle, (sockaddr *) &addr, sizeof(sockaddr)))
			{
				String error = strerror(WSAGetLastError());
				throw error;
			}
		}*/

		//std::string ReceiveLine();
		//std::string ReceiveBytes();


		// The parameter of SendLine is not a const reference
		// because SendLine modifes the std::string passed.
		//void   SendLine (std::string);

		// The parameter of SendBytes is a const reference
		// because SendBytes does not modify the std::string passed 
		// (in contrast to SendLine).
		//void   SendBytes(const std::string&);


	private:
		SOCKET m_handle;
	};

	//class SocketClient : public Socket {
	//public:
	//	SocketClient(const std::string& host, int port);
	//};

	//class SocketServer : public Socket {
	//public:
	//	SocketServer(int port, int connections, TypeSocket type=BlockingSocket);

	//	Socket* Accept();
	//};

	//// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
	//class SocketSelect {
	//public:
	//	SocketSelect(Socket const * const s1, Socket const * const s2=NULL, TypeSocket type=BlockingSocket);

	//	bool Readable(Socket const * const s);

	//private:
	//	fd_set fds_;
	//}; 
	
} /* END: network */ } /*END: kismo */
#endif

