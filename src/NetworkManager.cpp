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

    errorCode = closesocket(m_socket);

    if (errorCode != NO_ERROR)
    {
        std::cerr << "ServerSocket: Failed to close socket ( " << WSAGetLastError() << " )" << std::endl;
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
        m_socket = socket(AF_INET, SOCK_DGRAM, 0);
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

    if(m_initialized)
    {
        do
        {
            server_length = sizeof(struct sockaddr_in);
            messageLength = recvfrom(m_socket, recMessage, RCV_BUFFER_LENGTH, 0, (SOCKADDR*) &m_socketInfo, &server_length);
            recMessage[messageLength] = '\0';
            //std::cout<< messageLength <<": " <<recMessage <<std::endl;
            {
                boost::mutex::scoped_lock lock(m_queueMutex);
                m_msgQueue.push(recMessage);
            }
            //sendto(m_socket, sendMes , strlen(sendMes), 0, (SOCKADDR*) &m_socketInfo, server_length);
        }while(!m_kill);

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
