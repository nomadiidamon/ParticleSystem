#ifndef IUVO_CHARACTERS
#define IUVO_CHARACTERS

#include "IuvoEntities.h"
#include <vector>

namespace IuvoCharacters {
	using namespace IuvoEntities;

	enum Skill {
		Persuassion,
		Deception,
		Crafting,
		Cooking,
		Hunting,
		Attacking,
		Defending
	};
	std::string SkillToString(Skill skill) {
		switch (skill) {
		case Persuassion:
			return "Persuassion";
			break;

		case Deception:
			return "Deception";
			break;

		case Crafting:
			return "Crafting";
			break;

		case Cooking:
			return "Cooking";
			break;

		case Hunting:
			return "Hunting";
			break;

		case Attacking:
			return "Attacking";
			break;

		case Defending:
			return "Defending";
			break;

		default:
			return "Unknown";
		}
	}

	enum Occupation {
		Artisan,
		Merchant,
		Warrior,
		Thief,
		Politician,
		Priest,
		Miner
	};
	std::string OccupationToString(Occupation occupation) {
		switch (occupation) {
		case Artisan:
			return "Artisan";
			break;

		case Merchant:
			return "Merchant";
			break;

		case Warrior:
			return "Warrior";
			break;

		case Thief:
			return "Thief";
			break;

		case Politician:
			return "Politician";
			break;

		case Priest:
			return "Priest";
			break;

		case Miner:
			return "Miner";
			break;

		default:
			return "Unknown";
		}
	}

	class Character {
	private:
		Entity m_Entity;
		std::string m_SecondaryRace;
		std::vector<Skill> m_Skills;
		std::string m_Occupation;
	public:
		Character(std::string name, Race primRace, Occupation occupation, Skill skill, Race secRace = Race::Null)
		:	m_Entity(name, primRace)
		{
			m_Occupation = OccupationToString(occupation);
			m_Skills.push_back(skill);
			m_SecondaryRace = RaceToString(secRace);
		}
		void AddSkill(Skill newSkill) {
			m_Skills.push_back(newSkill);
		}
		void ChangeSecondaryRace(Race secondRace) {
			m_SecondaryRace = secondRace;
		}


	};

}

#endif
