#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN  // in only one cpp file
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Test_test) {
	int a = 4;
	int b = 4;
	BOOST_CHECK(4==4);
}