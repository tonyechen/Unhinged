#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "provided.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"
using namespace std;

AttributeTranslator::AttributeTranslator() {

}

AttributeTranslator::~AttributeTranslator() {
	for (auto it = m_sourceToCompatible.begin(); it != m_sourceToCompatible.end(); it++) {
		delete (*it);
	}
}

bool AttributeTranslator::Load(string filename) {
	fstream newfile;
	newfile.open(filename, ios::in);
	if (newfile.is_open()) {
		string currLine;
		for (;;) {
			// if there is no next line, exist loop
			if (!getline(newfile, currLine)) {
				break;
			}

			// ignore empty line
			if (currLine == "")
				continue;

			// check if the currLine is properly formatted
			if (!isValid(currLine)) {
				return false;
			}

			// break up the import string into source attribute-pair and compatible attribute-pair
			AttValPair source = getSrcAttValPair(currLine);
			AttValPair compatible = getCompAttValPair(currLine);

			// get the pointer to radix tree associated with the current source attribute
			auto rt = getRadixTree(source.attribute);

			// if the Radix tree doesn't exist, create a new one corresponding to the new attribute
			// and get the new radix tree
			if (rt == nullptr) {
				addNewRadixTree(source.attribute);
				rt = getRadixTree(source.attribute);
			}

			// get the current vector of compatible pairs stored inside the radix tree
			vector<AttValPair>* cpPairs = rt->search(source.value);
			// if there is no such a vector, insert one with the corresponding source value
			if (cpPairs == nullptr) {
				rt->insert(source.value, { compatible });
			}
			// if the current AttValPair doesn't already exist, add it to the vector
			else if (find(cpPairs->begin(), cpPairs->end(), compatible) == cpPairs->end()) {
				cpPairs->push_back(compatible);
			}
		}
		newfile.close();
	}
	cerr << "Attribute Translator successfully loaded data." << endl;
	return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
	auto rt = getRadixTree(source.attribute);

	// if the source attribute doesn't exist, return empty vector
	if (rt == nullptr) {
		cerr << source.attribute << " is not a Source Attribute" << endl;
		return {};
	}
	// if the source attribute-value pair doesn't have compatible pairs, return empty vector
	auto cpPairs = rt->search(source.value);
	if (cpPairs == nullptr) {
		cerr << source.attribute << "->" << source.value << " doesn't have compitable attribute-value pairs" << endl;
		return {};
	}

	return *cpPairs;
}

// get the corresponding radix tree based on attribute
RadixTree<std::vector<AttValPair>>* AttributeTranslator::getRadixTree(const std::string& attribute) const {
	for (int i = 0; i < m_attributes.size(); i++)
		if (m_attributes[i] == attribute)
			return m_sourceToCompatible[i];
	return nullptr;
}

// add a new RadixTree that corresponds to the new attribute
void AttributeTranslator::addNewRadixTree(const std::string& attribute) {
	// if the attribute doesn't exist in the vector of attributes
	if (find(m_attributes.begin(), m_attributes.end(), attribute) == m_attributes.end()) {
		// add the new attribute
		m_attributes.push_back(attribute);
		// allocate a new radix tree of vector of attvalpairs
		m_sourceToCompatible.push_back(new RadixTree<vector<AttValPair>>());
	}
	else {
		cerr << "RadixTree of the attribute already exit!" << endl;
	}
}

AttValPair AttributeTranslator::getSrcAttValPair(const std::string& input) const {
	string formatStr = input;

	int breakpoint = formatStr.find(',');
	string sourceAtt = formatStr.substr(0, breakpoint);

	formatStr = formatStr.substr(breakpoint + 1, formatStr.size() - breakpoint);

	breakpoint = formatStr.find(',');
	string sourceVal = formatStr.substr(0, breakpoint);

	return AttValPair(sourceAtt, sourceVal);
}

AttValPair AttributeTranslator::getCompAttValPair(const std::string& input) const {
	string formatStr = input;

	int breakpoint = formatStr.find(',');
	formatStr = formatStr.substr(breakpoint + 1, formatStr.size() - breakpoint);

	breakpoint = formatStr.find(',');
	formatStr = formatStr.substr(breakpoint + 1, formatStr.size() - breakpoint);

	return getSrcAttValPair(formatStr);
}

bool AttributeTranslator::isValid(const std::string& input) const {
	string formatStr = input;
	for (int i = 0; i < 3; i++) {
		int breakpoint = formatStr.find(',');
		if (breakpoint == 0 || breakpoint == -1 ||
			formatStr.substr(breakpoint + 1, 1) == "," ||
			formatStr.substr(breakpoint - 1, 1) == ",") {
			cerr << "input string: \'" << input << "\' is improperly formatted" << endl;
			return false;
		}
		formatStr = formatStr.substr(breakpoint + 1, formatStr.size() - breakpoint);
	}
	if (formatStr.find(',') == -1)
		return true;
	cerr << "input string: \'" << input << "\' is improperly formatted" << endl;
	return false;
}