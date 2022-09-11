#pragma once
#include "BlazeEngine/Logging/Log.h"
#include <vector>

namespace Blaze
{
	class BLAZE_API LogListener
	{
	public:
		LogListener();
		LogListener(const LogListener&) = delete;		
		~LogListener();


		void AddLog(const Log& log);

		void StartListening();
		void StopListening();

		void SupressLogs(bool supress);
		void PushLogs();
		void ClearLogs();

		inline std::vector<Log> GetLogs() const { return logs; }
		inline bool DoesSupress() const { return supress; }

		LogListener& operator=(const LogListener&) = delete;
	private:
		bool supress;
		std::vector<Log> logs;
	};
}