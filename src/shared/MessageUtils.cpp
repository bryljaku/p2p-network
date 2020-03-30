#include "MessageUtils.h"

size_t VarVisitWriter::operator() (string& input) const {
	memcpy(targetBuffer, input.c_str(), input.length()+1);
	return (input.length()+1);
}

size_t VarVisitWriter::operator()(int &input) const {
	memcpy(targetBuffer, &input, sizeof(int));
	return sizeof(int);
}

VarVisitWriter::VarVisitWriter(void *targetBuffer) {
	this->targetBuffer = targetBuffer;
}
