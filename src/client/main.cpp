#include <iostream>
#include <utility>
#include <pthread.h>
#include <unistd.h>
#include <sharedUtils.h>
#include <stdio.h>
#include <stdlib.h>
#include "networking/ResponderThread.h"
#include "download/DownloadManager.h"
#include "networking/CSocket.h"
#include "networking/SSocket.h"
#include "file/PeerInfo.h"
#include "database/Database.h"

#define TRACKER_PORT 59095
#define TRACKER_ADDRESS "172.28.1.1"
#define CLIENT_DEFAULT_PORT 59096
#define LISTENER_DEFAULT_TIMEOUT 5;
#define CLIENT_SEED_TEST_ADDRESS "172.28.1.2"
#define DEFAULT_DB_FILENAME "torrent.db"

extern uint32_t GLOB_responder_port;

void runMenu(int port, std::string &trackerIp, std::shared_ptr<Database> db);

struct ResponderThreadArgs {
    intptr_t connFd;
    Database* db;
    ResponderThreadArgs(intptr_t connFd, Database* db):connFd(connFd),db(db){};
};

struct ListenerArgs {
    int port;
    Database* db;
    ListenerArgs(int port, Database* db):port(port),db(db){};
};

void * runResponderThread(void * args) {
    auto* rta = (ResponderThreadArgs*) args;
    ResponderThread res;
    res.run(rta->connFd, rta->db);
}



void * connListen(void * args) {
    auto* la = (ListenerArgs*) args;
    int port = la->port;
    Database* db = la->db;
    delete la;
    std::string portS  = std::to_string(port);
    int socketFd = guard(socket(AF_INET, SOCK_STREAM, 0), "Could not create TCP listening socket");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    socklen_t addr_bytes = sizeof(addr);

    guard(bind(socketFd, (struct sockaddr *) &addr, addr_bytes), "Could not bind socket to port " + portS);
    guard(listen(socketFd, 100), "Could not listen on port " + portS);

    // default tracker-client socket timeout
//	struct timeval tv;
//	tv.tv_sec = LISTENER_DEFAULT_TIMEOUT;
//	tv.tv_usec = 0;

    syslogger->info("Listening on port " + std::to_string(ntohs(addr.sin_port)));
    for (;;) {
        intptr_t conn_fd = guard(accept(socketFd, NULL, NULL), "Could not accept");
//		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        pthread_t thread_id;
        ResponderThreadArgs rta(conn_fd, db);
        int ret = pthread_create(&thread_id, NULL, runResponderThread, (void*) &rta);
        if (ret != 0) {
            syslogger->error("Error from pthread: %d\n", ret); exit(1);
        }
        syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
    }
}

void runListener(int port, Database* db) {
    pthread_t thread_id;
    ListenerArgs* la = new ListenerArgs(port, db);
    pthread_create(&thread_id, NULL, connListen, (void*) la);
}

int main(int argc, char *argv[]) {
    initLogger("p2p-client");
    syslogger->info("p2p client starting");

    int clientPort = CLIENT_DEFAULT_PORT;
    std::string trackerIp = "172.28.1.1";

    auto database = std::make_shared<Database>();
    if(database->loadFromFile(DEFAULT_DB_FILENAME) == 1) {
        std::cout << "Missing or could not open the database file.\n";
    }
    int option;
    while((option = getopt(argc, argv, ":p:t:")) != -1) {
        switch(option) {
            case 'p': {
                int potentialPort = (int) strtol(optarg, nullptr, 10);
                if(potentialPort<1024 || potentialPort>65535) {
                    perror("Invalid clientPort number");
                    exit(1);
                }
                clientPort = potentialPort;
                break;
            }
            case 't': {
                trackerIp = optarg;
                break;
            }
            case ':':
                printf("option needs a value\n");
                break;
            case '?': //used for some unknown options
                printf("unknown option: %c\n", optopt);
                break;
            default:
                break;
        }
    }

    for(; optind < argc; optind++){ //when some extra arguments are passed
        printf("Given extra arguments: %s\n", argv[optind]);
    }
    if (trackerIp.empty()) {
        syslogger->error("No tracker ip provided");
        return 1;
    }

    GLOB_responder_port = clientPort;
    runMenu(clientPort, trackerIp, database);

    return 0;
}

void runMenu(int port, std::string &trackerIp, std::shared_ptr<Database> db) {
    bool end = false;
    bool listenerRunning = false;
    while (!end) {
        printf("___________________________________ \n");
        printf("___ Welcome to Concrete Torrent ___ \n\n");
        printf("1. Create a torrent file and send it to tracker\n");						// add local file to DB so it can be further shared
        printf("2. Allocate space for a torrent\n");				// create local file out of torrentFile, obligatory before requesting download
        printf("3. Request download\n");					// start download manager to begin downloading segments of a file
        printf("4. QUIT\n");
        printf("Choose option: ");

        int choice;

        for(;;) {
            std::cin >> choice;
            if (std::cin.fail()) {
                fprintf(stderr, "\nWrong input! Choose option: ");
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            break;
        }
        if(!listenerRunning) {
            listenerRunning = true;
            runListener(port, db.get());
        }
        switch (choice) {
            case 1: {
                Filename fileName;

                printf("Enter file name: ");
                std::cin >> fileName;
                if (!std::cin.fail()){
                    long potentialSize = getFilesize(fileName);
                    if(potentialSize == -1) {   // file can't be opened
                        std::cout << "File missing or cannot be opened.\n";
                        break;
                    }

                    std::cout << "Attempting to notify tracker about file '" << fileName << "'\n";
                    Torrent t(potentialSize, fileName);
                    SSocket sock(trackerIp, TRACKER_PORT);
                    if(sock.start() == OPEN) {
                        Hash newTorrentHash = sock.sendNewTorrentRequest(t);
                        if(newTorrentHash == 0) {
                            std::cout << "Communication error :/\n";
                            break;
                        }
                        t.hashed = newTorrentHash;
                        t.isSeed = true;
                    } else {
                        std::cout << "Problem connecting to the tracker!\n";
                        break;
                    }
                    t.saveToFile(t.fileName + ".torrent");
                    std::cout << "Saved as " + t.fileName + ".torrent\n";
                    File f(t, t.fileName);
                    f.markComplete();
                    db->addFile(f);
                    sock.sendImSeed(t);
                    std::cout << "Adding to tracker successful.\n";
                } else {
                    fprintf(stderr, "Wrong file name\n");
                }

                break;
            }
            case 2: {
                Filename fileName;

                printf("Enter file name(.torrent will be added automatically): ");
                std::cin >> fileName;
                fileName += ".torrent";
                if (!std::cin.fail()){
                    FileManager fm;
                    Torrent torrent(std::move(fileName));
                    if(torrent.size == -1) {
                        std::cout << "File doesn't exist or can't be opened.";
                        break;
                    }
                    if(torrent.hashed == 0) {
                        std::cout << "Torrent file not complete (tracker-provided hash missing).";
                        break;
                    }
                    fm.createLocalFile(torrent, "");      // create an empty file on disk, same folder as client
                    torrent.setPath(torrent.fileName+".torrent");
                    File f(torrent, torrent.fileName);
                    db->addFile(f);
                    db->getFile(torrent.hashed)->markComplete(); // to nie chce działac :o

                    std::cout << "CREATED: " << fileName << "\n";
                } else {
                    fprintf(stderr, "Wrong file name\n");
                }

                break;
            }
            case 3: {
                Filename torrentFileName;
                FileManager fm;
                printf("Enter requested torrent file name: (.torrent will be added automatically)");
                std::cin >> torrentFileName;
                torrentFileName += ".torrent";
                Torrent requestedTorrent(torrentFileName);
                if (!std::cin.fail()){
                    if(db->isFileInDatabase(requestedTorrent)) {
                        IpAddress trackerIpAddress(trackerIp, TRACKER_PORT);
                        DownloadManager dm(db, db->getFile(requestedTorrent.hashed), fm, trackerIpAddress);
                        auto dmThread = dm.start_manager();
                        dmThread.join();	// ???
                        std::cout << "REQUESTED: " << torrentFileName << "\n";
                    } else {
                        std::cout << "Requested torrent file not found.";
                        break;
                    }
                } else {
                    fprintf(stderr, "Wrong file name\n");
                }

                break;
            }
            case 4: {
                end = true;
                db->saveToFile(DEFAULT_DB_FILENAME);
                std::cout << "\nGood Bye :)\n";
                break;
            }
        }
    }
}
