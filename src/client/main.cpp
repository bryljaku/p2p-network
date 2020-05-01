#include "iostream"
#include "pthread.h"
#include <sharedUtils.h>
#include <src/client/download/DownloadManager.h>
#include "networking/CSocket.h"
#include "networking/RequestHandler.h"
#include "networking/SSocket.h"
#include <file/PeerInfo.h>
#define PORT 59095
#define TRACKER_MAX_MESSAGE_SIZE 128*1024	// in bytes

int main(int argc, char const *argv[]) {
	initLogger("p2p-client");
	syslogger->info("p2p client starting");
	Database database;
	std::shared_ptr<File> file(std::make_shared<File>(0, 10, "./path"));
	file->addPeer(PeerInfo(1, "127.0.0.1", "", "9999"));
	file->addPeer(PeerInfo(2, "127.0.0.1", "", "9992"));
	file->addPeer(PeerInfo(3, "127.0.0.1", "", "9993"));
    DownloadManager manager(database, file);
    auto mngThread = manager.start_manager();
    mngThread.join();
	SSocket testTrackerSocket("127.0.0.1", PORT);
	testTrackerSocket.start();
	testTrackerSocket.sendOk();
	testTrackerSocket.sendSeedlistRequest(1);
	Torrent testTorrent(123, "test.txt");
	testTrackerSocket.sendNewTorrentRequest(testTorrent);
}