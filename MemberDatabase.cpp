#include <string>
#include <fstream>
#include <iostream>

#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "provided.h"
using namespace std;

// helper
// generate attribute-value pair from the string format "attriburte,value"
AttValPair MemberDatabase::generateAttValPair(string input) {
	// find the position of the first ',' char
	int breakPoint = input.find(',');
	if (breakPoint == -1) {
		cout << "invalid format" << endl;
		cout << input << endl;
		return AttValPair();
	}
	// the chars before ',' is the attributre, and the chars after ',' is the the value
	string attribute = input.substr(0, breakPoint);
	string value = input.substr(breakPoint + 1, input.size() - breakPoint);
	return AttValPair(attribute, value);
}

RadixTree<vector<string>>* MemberDatabase::getRadixTree(const string& attribute) const {
	for (int i = 0; i < m_attributes.size(); i++)
		if (m_attributes[i] == attribute)
			return m_avprsToEmails[i];
	return nullptr;
}

void MemberDatabase::addNewRadixTree(const std::string& attribute) {
	// if the attribute doesn't exist in the vector of attributes
	if (find(m_attributes.begin(), m_attributes.end(), attribute) == m_attributes.end()) {
		// add the new attribute
		m_attributes.push_back(attribute);
		// allocate a new radix tree of vector of strings
		m_avprsToEmails.push_back(new RadixTree<vector<string>>());
	}
	else {
		cerr << "RadixTree of the attribute already exit!" << endl;
	}
}

MemberDatabase::MemberDatabase() {

}

MemberDatabase::~MemberDatabase() {
	for (auto it = m_avprsToEmails.begin(); it != m_avprsToEmails.end(); it++) {
		delete (*it);
	}
}

bool MemberDatabase::LoadDatabase(std::string filename) {
	fstream newfile;
	newfile.open(filename, ios::in);
	if (newfile.is_open()) {
		string currLine;
		string name;
		string email;
		PersonProfile* pp = nullptr;
		int line = 1;
		for (;;) {
			// if there is no next line, break
			if (!getline(newfile, currLine)) {
				// if the email already exists in the radix tree, there is a duplicate!!!
				// return false;
				if (m_profiles.search(email) != nullptr) {
					cerr << email << " is duplicated!" << endl;
					delete pp;
					return false;
				}
				// else we map email to the person profile
				m_profiles.insert(email, *pp);
				delete pp;
				break;
			}

			// if the current line is empty, reset line variable and skip to the next line
			if (currLine == "") {
				line = 1;

				// else we map email to the person profile
				m_profiles.insert(email, *pp);
				delete pp;
				continue;
			}

			// if it is the first line, store the first line into the name var
			if (line == 1) {
				name = currLine;
			}
			// if it is the second line, store the second line in to the email var
			else if (line == 2) {
				email = currLine;
				if (m_profiles.search(email) != nullptr)
					return false;
			}
			// if it is the third line 
			else if (line == 3) {
				// construct a new person profile
				pp = new PersonProfile(name, email);
			}
			else {
				// convert currnet line into a attribute-value pair
				AttValPair newAVPair = generateAttValPair(currLine);
				// add the attribute-value pair to the current person profile
				pp->AddAttValPair(newAVPair);

				// map attribute-value pair to email addresses

				// first get the Radix Tree associated with the attribute
				auto rt = getRadixTree(newAVPair.attribute);

				// if current attribute doesn't exits in the list of attributes
				// add a new radix tree with the new attribute
				if (rt == nullptr) {
					addNewRadixTree(newAVPair.attribute);
					rt = getRadixTree(newAVPair.attribute);
				}
				// get the vector currently being mapped to the value
				vector<string> *db = rt->search(newAVPair.value);
				// if the current value doesn't exist in the radix tree, add it to the radix tree
				// along with the vector of emails
				if (db == nullptr) {
					rt->insert(newAVPair.value, { email });
				}
				// if the email is not already mapped to the value
				// add the email
				else if (find(db->begin(), db->end(), email) == db->end()) {
					db->push_back(email);
				}
			}
			// increment line number
			line++;
		}
		newfile.close();
	}
	cerr << "The Member Database Loaded Successfully." << endl;
	return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	auto rt = getRadixTree(input.attribute);
	// if the attribute does not exist, return empty vector
	if (rt == nullptr) {
		return {};
	}
	// if the value corresponding to the attribute does not have matching members
	// return empty vector
	auto matching_emails = rt->search(input.value);
	if (matching_emails == nullptr) {
		return {};
	}
	// return the matching emails
	return *matching_emails;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const {
	PersonProfile* profile = m_profiles.search(email);
	return profile;
}