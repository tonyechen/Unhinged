#include "PersonProfile.h"
#include "provided.h"
#include "RadixTree.h"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

PersonProfile::PersonProfile(std::string name, std::string email) 
	:m_name(name), m_email(email)
{}

PersonProfile::~PersonProfile() {
}

string PersonProfile::GetName() const {
	return m_name;
}

string PersonProfile::GetEmail() const {
	return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval) {
	vector<string>* values = attToVal.search(attval.attribute);

	// if the attribute doesn't exist in the radix tree, 
	// create a new pair and add it to the database
	
	/*
	if (values == nullptr) {
		vector<string> v = { attval.value };
		attToVal.insert(attval.attribute, v);
	}
	else if (find(values->begin(), values->end(), attval.value) == values->end()) {
		vector<string> v = *values;
		v.push_back(attval.value);
		attToVal.insert(attval.attribute, v);
	}
	*/

	avprs.push_back(attval);
}

int PersonProfile::GetNumAttValPairs() const {
	return avprs.size();
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
	if (attribute_num < 0 || attribute_num >= GetNumAttValPairs())
		return false;
	attval.attribute = avprs[attribute_num].attribute;
	attval.value = avprs[attribute_num].value;
	return true;
}