#ifndef VERSION_PARSER_H
#define VERSION_PARSER_H

#include "newParser.h"

namespace mappers
{
	class VersionParser : commonItems::parser
	{
	public:
		VersionParser();
		explicit VersionParser(std::istream& theStream);

		friend std::ostream& operator<<(std::ostream& output, const VersionParser& versionParser);

	private:
		void registerKeys();

		std::string name;
		std::string version;
		std::string descriptionLine;
	};
}

#endif // VERSION_PARSER_H