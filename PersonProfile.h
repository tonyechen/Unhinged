#ifndef PERSONPROFILE_H
#define PERSONPROFILE_H

#include "RadixTree.h"
#include <vector>
#include <string>

struct AttValPair;
class PersonProfile {
public:
	PersonProfile(std::string name, std::string email);
	~PersonProfile();
	std::string GetName() const;
	std::string GetEmail() const;
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const;
	bool GetAttVal(int attribute_num, AttValPair& attval) const;

private:
	RadixTree<std::vector<std::string>> attToVal;
	std::vector<AttValPair> avprs;
	std::string m_name, m_email;
};

#endif