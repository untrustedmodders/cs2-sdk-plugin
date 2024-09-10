#include <core/sdk/utils.h>
#include <plugin_export.h>

extern "C" PLUGIN_API void PrintServer(const plg::string& msg)
{
	ConMsg("%s", msg.c_str());
}

extern "C" PLUGIN_API void PrintConsole(int clientIndex, const plg::string& message)
{
	utils::PrintConsole(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintChat(int clientIndex, const plg::string& message)
{
	utils::PrintChat(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintCentre(int clientIndex, const plg::string& message)
{
	utils::PrintCentre(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintAlert(int clientIndex, const plg::string& message)
{
	utils::PrintAlert(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintHtmlCentre(int clientIndex, const plg::string& message)
{
	utils::PrintHtmlCentre(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintConsoleAll(const plg::string& message)
{
	utils::PrintConsoleAll(message.c_str());
}

extern "C" PLUGIN_API void PrintChatAll(const plg::string& message)
{
	utils::PrintChatAll(message.c_str());
}

extern "C" PLUGIN_API void PrintCentreAll(const plg::string& message)
{
	utils::PrintCentreAll(message.c_str());
}

extern "C" PLUGIN_API void PrintAlertAll(const plg::string& message)
{
	utils::PrintAlertAll(message.c_str());
}

extern "C" PLUGIN_API void PrintHtmlCentreAll(const plg::string& message)
{
	utils::PrintHtmlCentreAll(message.c_str());
}

extern "C" PLUGIN_API void PrintChatColored(int clientIndex, const plg::string& message)
{
	utils::CPrintChat(CPlayerSlot(clientIndex - 1), message.c_str());
}

extern "C" PLUGIN_API void PrintChatColoredAll(const plg::string& message)
{
	utils::CPrintChatAll(message.c_str());
}
