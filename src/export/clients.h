#pragma once

#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/ccsplayercontroller.h>

extern "C" PLUGIN_API uint64_t GetSteamAccountId(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr || !pPlayer->m_bAuthorized)
	{
		return -1;
	}

	auto pSteamId = pPlayer->GetSteamId();
	if (pSteamId == nullptr)
	{
		return -1;
	}

	return pSteamId->ConvertToUint64();
}

extern "C" PLUGIN_API void GetClientIp(std::string& output, int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return;
	}

	output = pPlayer->GetIpAddress();
}

extern "C" PLUGIN_API void GetClientName(std::string& output, int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return;
	}

	output = pPlayer->GetName();
}

extern "C" PLUGIN_API float GetClientTime(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return -1.0f;
	}

	return pPlayer->GetTimeConnected();
}

extern "C" PLUGIN_API float GetClientLatency(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return 0.0f;
	}

	return pPlayer->GetLatency();
}

extern "C" PLUGIN_API int GetClientUserId(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return 0;
	}

	return pPlayer->GetUserId();
}

extern "C" PLUGIN_API bool IsClientAuthorized(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsAuthorized();
}

extern "C" PLUGIN_API bool IsClientConnected(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsConnected();
}

extern "C" PLUGIN_API bool IsClientInGame(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsInGame();
}

extern "C" PLUGIN_API bool IsClientSourceTV(int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return false;
	}

	return client->m_bIsHLTV();
}

extern "C" PLUGIN_API bool IsFakeClient(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(clientIndex);
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsFakeClient();
}

extern "C" PLUGIN_API bool IsPlayerAlive(int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return false;
	}

	return client->IsAlive();
}

/////

extern "C" PLUGIN_API int GetClientTeam(int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return 0;
	}

	return client->m_iTeamNum();
}

extern "C" PLUGIN_API int GetClientHealth(int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return 0;
	}

	return client->m_iHealth();
}

extern "C" PLUGIN_API void GetClientAbsOrigin(Vector& output, int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return;
	}

	output = client->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin();
}



extern "C" PLUGIN_API void GetClientAbsAngles(QAngle& output, int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return;
	}

	output = client->m_CBodyComponent->m_pSceneNode->m_angRotation();
}

extern "C" PLUGIN_API void ProcessTargetString(std::vector<int>& output, int caller, const std::string& target)
{
	g_PlayerManager.TargetPlayerString(caller, target.c_str(), output);
}

extern "C" PLUGIN_API void ChangeClientTeam(int clientIndex, int team)
{
	auto client = reinterpret_cast<CCSPlayerController*>(utils::GetController(clientIndex));
	if (!client)
	{
		return;
	}

	client->ChangeTeam(team);
}

extern "C" PLUGIN_API void SwitchClientTeam(int clientIndex, int team)
{
	auto client = static_cast<CCSPlayerController*>(utils::GetController(clientIndex));
	if (!client)
	{
		return;
	}

	client->SwitchTeam(team);
}

extern "C" PLUGIN_API void RespawnClient(int clientIndex)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return;
	}

	if (client->GetPawn()->IsAlive())
	{
		// TODO: Fix players spawning under spawn positions.
		static_cast<CCSPlayerPawn*>(client->GetPawn())->Respawn();
	}
	else
	{
		static_cast<CCSPlayerController*>(client)->Respawn();
	}
}

extern "C" PLUGIN_API void CommitSuicide(int clientIndex, bool explode, bool force)
{
	auto client = utils::GetController(clientIndex);
	if (!client)
	{
		return;
	}

	client->GetPawn()->CommitSuicide(explode, force);
}

extern "C" PLUGIN_API void KickClient(int clientIndex)
{
	g_pEngineServer2->DisconnectClient(clientIndex, NETWORK_DISCONNECT_KICKED);
}

extern "C" PLUGIN_API void BanClient(int clientIndex, float duration, bool kick)
{
	g_pEngineServer2->BanClient(CPlayerSlot(clientIndex), duration, kick);
}

extern "C" PLUGIN_API void BanIdentity(uint64_t steamId, float duration, bool kick)
{
	g_pEngineServer2->BanClient(CSteamID(steamId), duration, kick);
}