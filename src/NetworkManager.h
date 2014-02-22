#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <boost/thread/mutex.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <winsock2.h>

#define RCV_BUFFER_LENGTH 256

#define NM_QUEUE_EMPTY "QUEUE_EMPTY 0x11"
#define MSG_SHUTDOWN "shutdown"
#define MSG_SIZEWIN "sizewin"
#define MSG_CAM_MOVE "move"
#define MSG_WIRE "wire"
#define MSG_GRID "grid"

class NetworkManager
{
    public:
        NetworkManager(const int port);
        virtual ~NetworkManager();

        void listenData(void);
        void kill(void);

        std::string messageReceived(void);

    protected:
    private:
        void init(void);

        bool m_initialized;
        int m_port;
        SOCKET m_socket;
        SOCKADDR_IN m_socketInfo;

        SOCKET m_clientSocket;

        bool m_kill;
        boost::mutex m_queueMutex;
        std::queue<std::string> m_msgQueue;

};

#endif // NETWORKMANAGER_H
