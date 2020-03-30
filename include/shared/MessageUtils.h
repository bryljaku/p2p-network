#ifndef P2P_NETWORK_MESSAGEUTILS_H
#define P2P_NETWORK_MESSAGEUTILS_H
#include <string>
#include <cstring>

using namespace std;
struct VarVisitWriter {
	void *targetBuffer;

	size_t operator() (int& input) const;

	size_t operator() (string& input) const;

	VarVisitWriter(void *targetBuffer);
};

#endif //P2P_NETWORK_MESSAGEUTILS_H
