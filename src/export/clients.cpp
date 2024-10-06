#include "client.h"

#include <core/player_manager.h>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/ccsplayercontroller.h>
#include <core/sdk/utils.h>
#include <plugin_export.h>

//! 
extern "C" PLUGIN_API int GetClientIndexFromEntityPointer(CBaseEntity* entity)
{
	return utils::GetEntityPlayerSlot(entity).Get() + 1;
}

/*//! 
extern "C" PLUGIN_API void* GetClientFromIndex(int clientIndex)
{
	CUtlVector<CServerSideClient *>* pClients = utils::GetClientList();
	if (!pClients)
	{
		return nullptr;
	}
	
	return pClients->Element(clientIndex - 1);
}

//! 
extern "C" PLUGIN_API int GetIndexFromClient(CServerSideClient* client)
{
	CUtlVector<CServerSideClient *>* pClients = utils::GetClientList();
	if (!pClients)
	{
		return -1;
	}
	
	if (pClients->Find(client) != -1)
	{
		return client->GetEntityIndex().Get();
	}

	return -1;
}*/

//! Retrieves a client's authentication string (SteamID).
extern "C" PLUGIN_API void GetClientAuthId(plg::string& output, int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr || !pPlayer->m_bAuthorized)
	{
		return;
	}

	auto pSteamId = pPlayer->GetSteamId();
	if (pSteamId == nullptr)
	{
		return;
	}

	std::construct_at(&output, pSteamId->Render());
}

//! Returns the client's Steam account ID, a number uniquely identifying a given Steam account. This number is the basis for the various display SteamID forms, see the AuthIdType enum for examples.
extern "C" PLUGIN_API uint64_t GetClientAccountId(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr || !pPlayer->m_bAuthorized)
	{
		return 0;
	}

	auto pSteamId = pPlayer->GetSteamId();
	if (pSteamId == nullptr)
	{
		return 0;
	}

	return pSteamId->ConvertToUint64();
}

//! Retrieves a client's IP address.
extern "C" PLUGIN_API void GetClientIp(plg::string& output, int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return;
	}

	std::construct_at(&output, pPlayer->GetIpAddress());
}

//! Returns the client's name.
extern "C" PLUGIN_API void GetClientName(plg::string& output, int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return;
	}

	std::construct_at(&output, pPlayer->GetName());
}

//! Returns the client's connection time in seconds.
extern "C" PLUGIN_API float GetClientTime(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return -1.0f;
	}

	return pPlayer->GetTimeConnected();
}

//! Returns the client's current latency (RTT), more accurate than GetAvgLatency but jittering.
extern "C" PLUGIN_API float GetClientLatency(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return 0.0f;
	}

	return pPlayer->GetLatency();
}

//! Returns client access flags.
extern "C" PLUGIN_API uint64 GetUserFlagBits(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->GetAdminFlags();
}

//! Sets access flags on a client using bits instead of flags.
extern "C" PLUGIN_API void SetUserFlagBits(int clientIndex, uint64 flags)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->SetAdminFlags(flags);
}

//! Sets access flags on a client. If the client is not an admin, a temporary, anonymous AdminId is given.
extern "C" PLUGIN_API void AddUserFlags(int clientIndex, uint64 flags)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->SetAdminFlags(pPlayer->GetAdminFlags() | flags);
}

//! Removes flags from a client. If the client is not an admin, this has no effect.
extern "C" PLUGIN_API void RemoveUserFlags(int clientIndex, uint64 flags)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->SetAdminFlags(pPlayer->GetAdminFlags() & ~flags);
}

//! Returns if a certain player has been authenticated.
extern "C" PLUGIN_API bool IsClientAuthorized(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsAuthorized();
}

//! Returns if a certain player is connected.
extern "C" PLUGIN_API bool IsClientConnected(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsConnected();
}

//! Returns if a certain player has entered the game.
extern "C" PLUGIN_API bool IsClientInGame(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsInGame();
}

//! Returns if a certain player is the SourceTV bot.
extern "C" PLUGIN_API bool IsClientSourceTV(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return false;
	}

	return client->m_bIsHLTV();
}

//! Returns if the client is alive or dead.
extern "C" PLUGIN_API bool IsClientAlive(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return false;
	}

	return client->IsAlive();
}

//! Returns if a certain player is a fake client.
extern "C" PLUGIN_API bool IsFakeClient(int clientIndex)
{
	auto pPlayer = g_PlayerManager.GetPlayerBySlot(CPlayerSlot(clientIndex - 1));
	if (pPlayer == nullptr)
	{
		return false;
	}

	return pPlayer->IsFakeClient();
}

/////

//! Retrieves a client's team index.
extern "C" PLUGIN_API int GetClientTeam(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return 0;
	}

	return client->m_iTeamNum();
}

//! Returns the client's health.
extern "C" PLUGIN_API int GetClientHealth(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return 0;
	}

	return client->m_iHealth();
}

//! Returns the client's armor.
extern "C" PLUGIN_API int GetClientArmor(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return 0;
	}

	return 0;//client->m_Armor();
}

//! Returns the client's origin vector.
extern "C" PLUGIN_API void GetClientAbsOrigin(Vector& output, int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return;
	}

	output = client->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin();
}

//! Returns the client's position angle.
extern "C" PLUGIN_API void GetClientAbsAngles(QAngle& output, int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return;
	}

	std::construct_at(&output, client->m_CBodyComponent->m_pSceneNode->m_angRotation());
}

//! Returns whether a user can target another user.
extern "C" PLUGIN_API void ProcessTargetString(std::vector<int>& output, int caller, const plg::string& target)
{
	g_PlayerManager.TargetPlayerString(caller, target.c_str(), output);
}

//! Changes a client's team.
extern "C" PLUGIN_API void ChangeClientTeam(int clientIndex, int team)
{
	auto client = static_cast<CCSPlayerController*>(utils::GetController(CPlayerSlot(clientIndex - 1)));
	if (!client)
	{
		return;
	}

	client->ChangeTeam(team);
}

//! Switches the player's team.
extern "C" PLUGIN_API void SwitchClientTeam(int clientIndex, int team)
{
	auto client = static_cast<CCSPlayerController*>(utils::GetController(CPlayerSlot(clientIndex - 1)));
	if (!client)
	{
		return;
	}

	client->SwitchTeam(team);
}

//! Respawns a player.
extern "C" PLUGIN_API void RespawnClient(int clientIndex)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return;
	}

	if (client->GetPawn()->IsAlive())
	{
		// TODO: Fix players spawning under spawn positions
		static_cast<CCSPlayerPawn*>(client->GetPawn())->Respawn();
	}
	else
	{
		static_cast<CCSPlayerController*>(client)->Respawn();
	}
}

//! Forces a player to commit suicide.
extern "C" PLUGIN_API void ForcePlayerSuicide(int clientIndex, bool explode, bool force)
{
	auto client = utils::GetController(CPlayerSlot(clientIndex - 1));
	if (!client)
	{
		return;
	}

	client->GetPawn()->CommitSuicide(explode, force);
}

//! Disconnects a client from the server as soon as the next frame starts.
extern "C" PLUGIN_API void KickClient(int clientIndex)
{
	g_pEngineServer2->DisconnectClient(CPlayerSlot(clientIndex - 1), NETWORK_DISCONNECT_KICKED);
}

//! Bans a client.
extern "C" PLUGIN_API void BanClient(int clientIndex, float duration, bool kick)
{
	g_pEngineServer2->BanClient(CPlayerSlot(clientIndex - 1), duration, kick);
}

//! Bans an identity (either an IP address or auth string).
extern "C" PLUGIN_API void BanIdentity(uint64_t steamId, float duration, bool kick)
{
	g_pEngineServer2->BanClient(CSteamID(static_cast<uint64>(steamId)), duration, kick);
}

