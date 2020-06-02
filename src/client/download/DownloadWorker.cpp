#include <sharedUtils.h>
//created by Jakub
#include <utility>
#include "DownloadWorker.h"
#include "networking/CSocket.h"

#define CLIENT_DEFAULT_PORT 59095
std::thread DownloadWorker::startWorker() {
    return std::thread([&] {
        try {
            syslogger->info("new DownloadWorker initiated");
            work();
            syslogger->info("DownloadWorker finished");
        } catch (std::exception &e) {
            syslogger->error("Download Worker errored for torrent {}\n{}", torrent.hashed);
        }
        
    });
}

void DownloadWorker::work() {
    std::string peerIp = peer->getIpV4Address();
    if (peerIp.empty())
        peerIp = peer->getIpV6Address();
    if (peerIp.empty()) {
        syslogger->error("Peer without address");
        finished = true;
        return;
    }
    auto peerPort = peer->getPort();
    CSocket peerSocket(peerIp, peerPort);
    auto state = peerSocket.start();
    if (state != OPEN) {
        syslogger->warn("DownloadWorker problem occured while connecting to {}:{}", peerIp, peerPort);
        finished = true;
        return;
    }
    ListResponse peerFragments = peerSocket.requestFragmentsList(torrent);
    for (auto fragmentId: peerFragments.fragments()) {
        // if (file->tryToSetStateSegmentStateToDownload(fragmentId)) {
        //     file->setSegmentState(fragmentId, DOWNLOADING);
        //     auto fragmentResponse = peerSocket.requestFragment(torrent, fragmentId);
        //     if (fragmentResponse.filecode() == F_ERROR) {
        //         file->setSegmentState(fragmentId, FREE);
        //         syslogger->warn("DownloadWorker Peer didn't respond with segment bytes");
        //     } else {
        //         const auto &fragmentBytes = fragmentResponse.fragment();
        //         fileManager.storeSegmentToFile(file->getTorrent().fileName, file->getPath(), fragmentId, fragmentBytes); //todo - waiting for fileManager
        //         file->setSegmentState(fragmentId, COMPLETE);
        //         syslogger->info("DownloadWorker correctly downloaded segment {} for torrent {}", fragmentId, torrent.hashed);
        //     }
        // }
    }
    syslogger->info("downloaded all fragments from {}", peerIp);
    finished = true;
}

std::shared_ptr<PeerInfo> DownloadWorker::getPeer() {
    return peer;
}

bool DownloadWorker::isDone() {
    return finished;
}

DownloadWorker::~DownloadWorker() {
}
