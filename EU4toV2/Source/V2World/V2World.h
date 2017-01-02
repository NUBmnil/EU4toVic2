/*Copyright (c) 2017 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef V2WORLD_H_
#define V2WORLD_H_



#include "V2Country.h"
#include "V2Diplomacy.h"
#include "V2Factory.h"
#include "V2TechSchools.h"
#include "V2Party.h"
#include "V2Province.h"
#include "../EU4World/EU4Army.h"
#include "../EU4World/EU4Province.h"
#include <list>
#include <set>

class V2Country;
class V2Army;
class V2LeaderTraits;



class V2World {
	public:
		V2World(const EU4World& sourceWorld);

	private:
		void importProvinces();
		void getProvinceLocalizations(string file);

		void importDefaultPops();
		void findCoastalProvinces();
		void importPotentialCountries();
		void importTechSchools();

		void convertCountries(const EU4World& sourceWorld);
		void checkForCivilizedNations();

		void convertProvinces(const EU4World& sourceWorld);
		vector<V2Demographic> determineDemographics(vector<EU4PopRatio>& popRatios, EU4Province* eProv, V2Province* vProv, EU4Country* oldOwner, int destNum, double provPopRatio);

		void convertDiplomacy(const EU4World& sourceWorld);
		void setupColonies();
		void setupStates();
		void convertUncivReforms();
		void convertTechs(const EU4World& sourceWorld);
		void allocateFactories(const EU4World& sourceWorld);
		void setupPops(const EU4World& sourceWorld);
		void addUnions();
		void convertArmies(const EU4World& sourceWorld);

		void output() const;
		void createModFile() const;
		void outputPops() const;

		map<int, V2Province*> provinces;
		map<string, V2Country*> countries;
		map<string, V2Country*> potentialCountries;
		map<string, V2Country*> dynamicCountries;
		V2Diplomacy diplomacy;
		map<int, set<string>> colonies;
		map<string, list<int>*>	popRegions;
		vector<techSchool> techSchools;
		map<int, int> leaderIDMap; // <EU4, V2>
		long totalWorldPopulation;
		bool isRandomWorld;
};



#endif // V2WORLD_H_