# p2p-network
Peer-to-peer file transfer network project

Running the application:

- sudo run-after-clone.sh
- cmake .
- make

client binary -> ./src/client/client 

server binary -> ./src/server/server

If you want to create test environment:
- docker network create --subnet 172.28.0.0/16 bittorrent-network
- docker-compose run client_seed sh

^ These commands create virtual network and then create two containers. 
One of them(ipV4 172.28.1.1) hosts listening server.
The other one(ipV4 172.28.1.2) has client binary in /app folder.


