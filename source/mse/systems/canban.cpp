#include <mse/systems/canban.h>

namespace mse
{
	std::unordered_multimap<CanbanEvents, CanbanEvent> Canban::board;
	// std::vector<CanbanEvent> Canban::advancedBoard;
	
	bool Canban::GetTask(CanbanEvents event, CanbanEvent& data)
	{
		auto it = board.find(event);
		if (it != board.end())
		{
			data = it->second;
			board.erase(it);
			return true;
		}
		return false;
	}
	
	void Canban::PutTask(CanbanEvents event, const CanbanEvent& data)
	{
		board.insert({event, data});
	}
	
	void Canban::Clear()
	{
		board.clear();
	}
}

