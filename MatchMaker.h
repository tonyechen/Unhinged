#ifndef MatchMaker_h
#define MatchMaker_h

class MemberDatabase;
class AttributeTranslator;
struct EmailCount;
class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
	~MatchMaker();
	std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
private:
	const MemberDatabase* m_mdb;
	const AttributeTranslator* m_at;
};

#endif // !MatchMaker_h

