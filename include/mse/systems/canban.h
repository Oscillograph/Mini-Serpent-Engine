#ifndef MSE_SYSTEMS_CANBAN_H
#define MSE_SYSTEMS_CANBAN_H

#include <mse/core.h>

namespace mse
{
	// the pattern is
	// XXX - for Canban-specific events
	// YYY_ZZZ - ZZZ event for a system YYY
	enum class CanbanEvents
	{
		None					= 0,
		Application_Shutdown 	= 1,
		Clear					= 2,
		Physics_ChangeType		= 3,
		Backend_Create			= 4,
		Backend_Run				= 5,
		Backend_Stop			= 6,
		Backend_Pause			= 7,
		Backend_Recreate		= 8,
	};
	
	enum class CanbanReceiver
	{
		None					= 0,
		Anyone					= 1,
		Layer					= 2,
		Scene					= 3,
		Physics					= 4,
		Renderer				= 5,
		App						= 6,
		Backend					= 7,
	};
	
	struct CanbanEvent
	{
		CanbanEvents type = CanbanEvents::None;
		CanbanReceiver receiver = CanbanReceiver::None;
		void* data = nullptr; // sender data; can be of type Entity*
	};
	
	struct Canban
	{
		static std::unordered_multimap<CanbanEvents, CanbanEvent> board;
		// static std::vector<CanbanEvent> advancedBoard;
		
		static bool GetTask(CanbanEvents event, CanbanEvent& data);
		static void PutTask(CanbanEvents event, const CanbanEvent& data);
		static void Clear();
	};
}

#endif

