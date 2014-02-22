#include "NetworkManager.h"

NetworkManager::NetworkManager(const int port)
:m_initialized(false),
m_port(port),
m_socket(0),
m_kill(false)
{
    memset(&m_socketInfo, 0x00, sizeof(m_socketInfo));
    init();
}

NetworkManager::~NetworkManager()
{
    int errorCode;

    // ********************************************************
	// Fermeture de la session TCP Correspondant à la commande connect()
	// ********************************************************
	errorCode=shutdown(m_clientSocket,2); // 2 signifie socket d'émission et d'écoute
	if (errorCode!=0)
	{
	    std::cerr << "ServerSocket: Failed to shutdown client socket ( " << errorCode << " // " << WSAGetLastError() << " )" << std::endl;
	}

	// ********************************************************
	// Fermeture des deux socket correspondant à la commande socket() et accept()
	// ********************************************************
	errorCode=closesocket(m_clientSocket);
	if (errorCode!=0)
	{
	    std::cerr << "ServerSocket: Failed to close client socket ( " << errorCode << " // " << WSAGetLastError() << " )" << std::endl;
	}

	errorCode=closesocket(m_socket);
	if (errorCode!=0)
	{
	    std::cerr << "ServerSocket: Failed to close socket ( " << errorCode << " // " << WSAGetLastError() << " )" << std::endl;
	}

	// ********************************************************
	// Quitte proprement le winsock ouvert avec la commande WSAStartup
	// ********************************************************
	errorCode=WSACleanup(); // A appeler autant de fois qu'il a été ouvert.
	if (errorCode!=0)
    {
        std::cerr << "ServerSocket: Winsock cannot be closed ( " << errorCode << " // " << WSAGetLastError() << " )" << std::endl;
    }
}

void
NetworkManager::init(void)
{
    int errorCode;
    m_initialized = false;
    WSADATA initialisation_win32;

    errorCode = WSAStartup(MAKEWORD(2,2),&initialisation_win32);
    if (errorCode != NO_ERROR)
    {
        std::cerr << "Socket Initialization: Error with WSAStartup ( " << WSAGetLastError() << " )" << std::endl;
    }else
    {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == INVALID_SOCKET)
        {
            std::cerr << "Socket Initialization: Error creating socket ( " << WSAGetLastError() << " )" << std::endl;
        }else
        {
            m_socketInfo.sin_family = AF_INET;
            m_socketInfo.sin_addr.s_addr = INADDR_ANY;    // Ecoute sur toutes les IP locales
            m_socketInfo.sin_port = htons(m_port);        // Ecoute sur le port
            errorCode = bind(m_socket,(struct sockaddr*)&m_socketInfo,sizeof(m_socketInfo));

            if (errorCode != NO_ERROR)
            {
                std::cerr << "ServerSocket: Failed to connect ( " << WSAGetLastError() << " )" << std::endl;
            }else
            {
                std::cout << "ServerSocket: Server ready on port " << m_port << std::endl;
                m_initialized = true;
            }
        }
    }
}

void
NetworkManager::listenData(void)
{
    int server_length;
    int messageLength;
    char recMessage[RCV_BUFFER_LENGTH];

    int error = 99;

    if(m_initialized)
    {
        while((error !=0) && (!m_kill))
        {
            error = listen(m_socket, 1);
            if(error == 0)
            {
                std::cout << "ServerSocket: Connection requested...\n";
                server_length = sizeof(m_socketInfo);
                m_clientSocket = accept(m_socket, (SOCKADDR*) &m_socketInfo, &server_length);
                if(m_clientSocket == INVALID_SOCKET)
                {
                   std::cerr << "ServerSocket: Failed to accept the TCP session ( " << WSAGetLastError() << " )" << std::endl;
                   m_kill = true;
                }else
                {
                    std::cout << "ServerSocket: Connection accepted\n";
                }
            }
        }

        while((error ==0) && (!m_kill))
        {

            messageLength = recv(m_clientSocket, recMessage, RCV_BUFFER_LENGTH -1, 0);
            if(messageLength >0)
            {
                recMessage[messageLength] = '\0';
                {
                    boost::mutex::scoped_lock lock(m_queueMutex);
                    m_msgQueue.push(recMessage);
                }
            }
        }

    }else
    {
        std::cerr << "Network manager not initialized !" << std::endl;

    }
}

std::string
NetworkManager::messageReceived(void)
{
    std::string msg;

    {
       boost::mutex::scoped_lock lock(m_queueMutex);
       if(m_msgQueue.empty())
       {
           msg = NM_QUEUE_EMPTY;
       }else
       {
           msg = m_msgQueue.front();
           m_msgQueue.pop();
       }
    }

    return msg;
}

void
NetworkManager::kill(void)
{
    m_kill = true;
}
