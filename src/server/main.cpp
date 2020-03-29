#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
void syslog_example()
{
    // w konsoli "tail -f /var/log/syslog" i powinno byc widac ze wchodzi
    std::string ident = "spdlog-example";
    auto syslog_logger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
    syslog_logger->warn("This is warning that will end up in syslog.");
}

int guard(int r, char * err) {if (r == -1) { perror(err); exit(1); } return r; }

void * thread_func(void * arg) {
    intptr_t conn_fd = (uintptr_t) arg;
    printf("thread: serving fd %ld\n", conn_fd);
    char buf[1024];
    for (;;) {
        int bytes_received = guard(recv(conn_fd, buf, sizeof(buf), 0), "Could not recv");
        if (bytes_received == 0) { goto stop_serving; }
        int bytes_sent = 0;
        while (bytes_sent < bytes_received) {
            ssize_t bytes_sent_this_call = send(conn_fd, buf+bytes_sent, bytes_received-bytes_sent, 0);
            if (bytes_sent_this_call == -1) { goto stop_serving; }
            bytes_sent += bytes_sent_this_call;
        }
    }
    stop_serving:
    guard(close(conn_fd), "Could not close socket");
    printf("thread: finished serving %ld\n", conn_fd);
    return NULL;
}

int main() {
    int listen_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "Could not create TCP listening socket");
    guard(listen(listen_fd, 100), "Could not listen");
    struct sockaddr_in addr;
    socklen_t addr_bytes = sizeof(addr);
    guard(getsockname(listen_fd, (struct sockaddr *) &addr, &addr_bytes), "Could not get sock name");
    printf("Listening on port %d\n", ntohs(addr.sin_port));
    for (;;) {
        intptr_t conn_fd = guard(accept(listen_fd, NULL, NULL), "Could not accept");
        pthread_t thread_id;
        int ret = pthread_create(&thread_id, NULL, thread_func, (void*) conn_fd);
        if (ret != 0) { printf("Error from pthread: %d\n", ret); exit(1); }
        printf("main: created thread to handle connection %ld\n", conn_fd);
    }
    return 0;
}
