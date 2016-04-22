#ifndef ENTITY_OBSERVER
#define ENTITY_OBSERVER
#include <vector>
#include "g_character.h" // Observer


// Observer (наблюдатель, издатель-подписчик)
class G_Subject
{
private:
	/* ============================================================================================================================= */
	std::vector<G_Character*> entity_list;
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	inline void attach(G_Character *obj) { entity_list.push_back(obj); }
	inline bool empty() const { return entity_list.empty(); }
	inline void clear() { entity_list.clear(); entity_list.shrink_to_fit(); }
	/* ============================================================================================================================= */
	void notify(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p)
	{
		for (std::size_t i = 0; i < entity_list.size(); i++)
			entity_list[i]->action_time(window, game_speed, no_iteraction, current_frame, p);
	}
	/* ============================================================================================================================= */
};
#endif /* ENTITY_OBSERVER */