#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "provided.h"
using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at) {
	m_mdb = &mdb;
	m_at = &at;
}

MatchMaker::~MatchMaker() {

}

bool operator< (const AttValPair& v1, const AttValPair& v2) {
	return v1.attribute + v1.value < v2.attribute + v2.value;
}

bool operator< (const EmailCount& v1, const EmailCount& v2) {
	if (v1.count > v2.count)
		return true;
	if (v1.count < v2.count)
		return false;
	return v1.email < v2.email;
}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const {
	const PersonProfile* lonlyMember = m_mdb->GetMemberByEmail(email);

	if (lonlyMember == nullptr)
		return {};

	int att_nums = lonlyMember->GetNumAttValPairs();

	// retrieve all compatible attribute-value pairs of current member
	vector<AttValPair> avPairs;
	set<AttValPair> targetAVPairs;
	for (int i = 0; i < att_nums; i++) {
		AttValPair av;
		lonlyMember->GetAttVal(i, av);
		auto compatibleAVPairs = m_at->FindCompatibleAttValPairs(av);
		for (int j = 0; j < compatibleAVPairs.size(); j++) {
			if (find(targetAVPairs.begin(), targetAVPairs.end(), compatibleAVPairs[j]) == targetAVPairs.end())
				targetAVPairs.insert(compatibleAVPairs[j]);
		}
	}

	map<string, int> potentialLovers;
	for (auto it = targetAVPairs.begin(); it != targetAVPairs.end(); it++) {
		auto matching = m_mdb->FindMatchingMembers(*it);

		for (int i = 0; i < matching.size(); i++) {
			if (find(matching.begin(), matching.end(), matching[i]) == matching.end()) {
				potentialLovers[matching[i]] = 1;
			}
			else {
				potentialLovers[matching[i]]++;
			}
		}
	}

	if (threshold <= 0)
		threshold = 1;

	vector<EmailCount> loveBirds;
	for (auto it = potentialLovers.begin(); it != potentialLovers.end(); it++) {
		if ((*it).second >= threshold)
			loveBirds.push_back(EmailCount((*it).first, (*it).second));
	}

	sort(loveBirds.begin(), loveBirds.end());

	return loveBirds;
}