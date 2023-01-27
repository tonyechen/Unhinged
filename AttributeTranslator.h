#ifndef AttributeTranslator_h
#define AttributeTranslator_h

#include <vector>
#include "RadixTree.h"
class AttributeTranslator {
public:
	AttributeTranslator();
	~AttributeTranslator();

	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	// vector of all the source attributes, each attribute has its own radix tree
	std::vector<std::string> m_attributes;
	// vector of radix trees, this associates to the vector of attributes
	// position in this vector correspond to the vector of attributes
	std::vector<RadixTree<std::vector<AttValPair>>*> m_sourceToCompatible;

	// get the corresponding radix tree based on attribute
	RadixTree<std::vector<AttValPair>>* getRadixTree(const std::string& attribute) const;

	// add a new RadixTree that corresponds to the new attribute
	void addNewRadixTree(const std::string& attribute);

	// get the src attval pair of the input string
	AttValPair getSrcAttValPair(const std::string& input) const;

	// get the compatible pair of the input string
	AttValPair getCompAttValPair(const std::string& input) const;

	// check if the input string is formatted correctly
	bool isValid(const std::string& input) const;
};

#endif
