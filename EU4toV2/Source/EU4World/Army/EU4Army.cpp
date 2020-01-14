#include "EU4Army.h"
#include <algorithm>
#include <random>
#include "ParserHelpers.h"

EU4::EU4Army::EU4Army(std::istream& theStream)
{
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			armyId = idblock.getIDNum();
		});
	registerKeyword("leader", [this](const std::string& unused, std::istream& theStream)
		{
			EU4::ID idblock(theStream);
			leaderId = idblock.getIDNum();
		});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerRegex("regiment|ship", [this](const std::string& unused, std::istream& theStream)
		{
			EU4Regiment regimentBlock(theStream);
			regimentList.push_back(regimentBlock);
		});
	registerKeyword("location", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt locationInt(theStream);
			location = locationInt.getInt();
		});
	// Dropped from saves at 1.20
	registerKeyword("at_sea", [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt atSeaInt(theStream);
			atSea = atSeaInt.getInt();
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}

double EU4::EU4Army::getAverageStrength(REGIMENTCATEGORY category) const
{
	int regimentCount = regimentList.size();
	double totalStrength = 0.0;
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			totalStrength += regiment.getStrength();
		}
	}
	if (regimentCount > 0) return (totalStrength / regimentCount);
	return 0;
}

int EU4::EU4Army::getTotalTypeStrength(REGIMENTCATEGORY category) const
{
	int totalStrength = 0;
	for (const auto& regiment : regimentList)
	{
		if (regiment.getCategory() == category)
		{
			totalStrength += regiment.getTypeStrength();
		}
	}
	return totalStrength;
}

void EU4::EU4Army::resolveRegimentTypes(const mappers::UnitTypeMapper& unitTypeMapper)
{
	auto regimentTypeMap = unitTypeMapper.getUnitTypeMap();
	for (auto& regiment : regimentList)
	{
		try
		{
			regiment.setCategory(regimentTypeMap[regiment.getType()].getCategory());
			regiment.setTypeStrength(regimentTypeMap[regiment.getType()].getStrength());
			home_provinces[regiment.getCategory()].push_back(regiment.getHome());
		}
		catch (std::exception&)
		{
			std::runtime_error exception("Illegal unit type: " + regiment.getType() + ", aborting!");
			throw exception;
		}
	}
}

std::optional<int> EU4::EU4Army::getProbabilisticHomeProvince(EU4::REGIMENTCATEGORY category) const
{
	if (home_provinces.find(category) == home_provinces.end())
	{
		return std::nullopt;
	}
	const auto& candidates = home_provinces.at(category);
	if (candidates.size() == 0)
	{
		return std::nullopt;
	}

	std::set<int> randomProvince;
	std::sample(candidates.begin(), candidates.end(), std::inserter(randomProvince, randomProvince.begin()), 1, std::mt19937{ std::random_device{}() });
	return *randomProvince.begin();
}

void EU4::EU4Army::blockHomeProvince(const int blocked)
{
	for (const auto& regType : EU4::RegimentCategoryTypes)
	{
		auto& homes = home_provinces[regType.first];
		homes.erase(std::remove(homes.begin(), homes.end(), blocked), homes.end());
	}
}