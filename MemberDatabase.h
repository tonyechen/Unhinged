#ifndef MEMBER_DATABASE_H
#define MEMBER_DATABASE_H

#include <string>
#include <vector>
#include "RadixTree.h"

class PersonProfile;
struct AttValPair;
class MemberDatabase {
public:
	MemberDatabase();
	~MemberDatabase();
	bool LoadDatabase(std::string filename);
	std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(std::string email) const;
private:
	// vector of all the current attributes, each attribute has its own radix tree
	std::vector<std::string> m_attributes;

	// vector of radix trees, this associates to the vector of attributes
	// position in this vector correspond to the vector of attributes
	std::vector<RadixTree<std::vector<std::string>>*> m_avprsToEmails;

	// map email addresses to member profiles
	RadixTree<PersonProfile> m_profiles;

	// get the corresponding radix tree based on attribute
	RadixTree<std::vector<std::string>>* getRadixTree(const std::string& attribute) const;

	// add a RadixTree that corresponds to the new attribute
	void addNewRadixTree(const std::string& attribute);

	// convert the input string to attval pair
	AttValPair generateAttValPair(std::string input);
};

#endif