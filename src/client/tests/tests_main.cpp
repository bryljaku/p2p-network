#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN  // in only one cpp file
#include <boost/test/unit_test.hpp>
#include <database/Database.h>
#include <networking/CSocket.h>
#include "../../shared/Torrent.h"

BOOST_AUTO_TEST_CASE(Test_test) {
	//main(0, nullptr);
	BOOST_CHECK(4==4);
}


BOOST_AUTO_TEST_CASE(database_save_read) {
	initLogger("p2p-client-test");
	Database db;
	Torrent t1(101,909,"test1.html");
	t1.saveToFile("torrent1.tor");
	Torrent t1_read("torrent1.tor");
	File f1(t1_read, "test1.html");
	db.addFile(f1);
	db.saveToFile("saveTest");
	db.loadFromFile("saveTest");
	db.saveToFile("saveTestFinal");
	db.loadFromFile("saveTestFinal");
	db.saveToFile("saveTestMoreFinal");
	auto f = db.getFiles();

	BOOST_CHECK(f[0]->getTorrent().fileName == "test1.html");
}

//BOOST_AUTO_TEST_CASE(get_seedlist) {
//	initLogger("p2p-client-test");
//	CSocket cs("127.0.0.1", 59096);
//	cs.start();
//	Torrent tor(1337, 10, "./path");
//	ListResponse lr = cs.requestFragmentsList(tor);
//	for(auto frag : lr.fragments()) {
//		syslogger->debug(frag);
//	}
//	cs.requestFragment(tor, 1);
//}