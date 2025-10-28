#ifndef IUVO_ENTITIES
#define IUVO_ENTITIES

#include <string>

namespace IuvoEntities {

	enum Race {
		Plant,
		Animal,
		Spirit,
		Goblin,
		Kobold,
		Human,
		Golem,
		Demon,
		Dwarf,
		Elf,
		Dragon,
		God,
		Null

	};
	std::string RaceToString(Race race) {
		switch (race) {

		case Plant:
			return "Plant";
			break;

		case Animal:
			return "Animal";
			break;

		case Spirit:
			return "Spirit";
			break;

		case Goblin:
			return "Goblin";
			break;

		case Kobold:
			return "Kobold";
			break;

		case Human:
			return "Human";
			break;

		case Golem:
			return "Golem";
			break;

		case Demon:
			return "Demon";
			break;

		case Dwarf:
			return "Dwarf";
			break;

		case Elf:
			return "Elf";
			break;

		case Dragon:
			return "Dragon";
			break;

		case God:
			return "God";
			break;

		case Null:
			return "Null";
			break;

		default:
			return "Unknown";
		}
	}



	class Entity {
	private:
		std::string m_Name;
		std::string m_Race;
	public:
		Entity(std::string name, Race race)
		{
			m_Name = name;
			m_Race = RaceToString(race);
		}

		std::string GetName()
		{
			return m_Name;
		}
		void SetName(std::string name)
		{
			m_Name = name;
		}
		std::string GetRace() {
			return m_Race;
		}
		void SetRace(Race race) {
			m_Race = RaceToString(race);
		}


	};

}

#endif 

