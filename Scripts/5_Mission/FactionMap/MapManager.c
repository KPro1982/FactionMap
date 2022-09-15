class MapRegistryEntry {
	string m_mapname;
	string m_password;
	ref array<ref MapMember> m_members; 
	
	void MapRegistryEntry(string mapname, string password, MapMember member = null)
	{
		
		m_members = new array<ref MapMember>;
		m_mapname = mapname;
		m_password = password;
		if(member)
		{
			m_members.Insert(member);
		}
	
	}
	
	void AddMember (MapMember member)
	{
		int pos = -1;
		if(!FindMember(member, pos))
		{
			m_members.Insert(member);
			Print("Member added");
		}
		else
		{
			Print("Member not added");
		}
		
	
	
	}
	
	void RemoveMember (MapMember member)
	{
		int pos = -1;
		if(FindMember(member, pos))
		{
			m_members.Remove(pos);
		}
	
	
	}
	void RemoveMemberByUsername (string username)
	{
		int pos = -1;
		if(FindMemberByUsername(username, pos))
		{
			m_members.Remove(pos);
		}
	
	
	}
	bool FindMember(MapMember member, out int pos = 0)
	{
		
		for(int i = 0; i < m_members.Count(); i++)
		{
			if(m_members.Get(i).IsMember(member))
			{
				pos = i;
				return true;
			}
		
		}
		
		return false;
	
	
	}
	
	bool FindMemberByUsername(string username, out int pos = 0)
	{
		
		for(int i = 0; i < m_members.Count(); i++)
		{
			if(m_members.Get(i).m_username == username)
			{
				pos = i;
				return true;
			}
		
		}
		
		return false;
	
	
	}
	int CountMembers()	{
		
		return m_members.Count();
	
	}
	
	bool HasMembers()
	{
		if(m_members.Count() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	
	
	void PrintEntry()
	{
		Print(string.Format("[%1]: %2", m_mapname, m_password));
		for (int i=0; i < m_members.Count(); i++)
		{
			
			Print(string.Format("- [%1]", m_members.Get(i).m_username));
		
		}
	
	}

}

class MapMember
{
	string m_username;
	string m_steamid;
	
	void MapMember(string username, string steamid = "")
	{
		m_username = username;
		m_steamid = steamid;
	}
	
	bool IsMember(MapMember member)
	{
		if(member.m_steamid == m_steamid)
		{
			return true;
		}
		return false;
	}
}

class MapManager
{
	private ref static MapManager m_instance = null;	
	static ref array<ref MapRegistryEntry> m_registeredMaps;
	
	static MapManager Get()
	{
		if(m_instance == null)
		{
			m_instance = new MapManager;
			Init(m_instance);
			
		}
		return m_instance;
		
	}
	
	
	static void Init(MapManager instance)
	{

		m_registeredMaps = Config_Server.Get().m_registeredMaps;
		GetRPCManager().AddRPC("FactionMap", "RPC_JoinMap", instance, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FactionMap", "RPC_LeaveMap", instance, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FactionMap", "RPC_SendPingToServer", instance, SingleplayerExecutionType.Both);
		
	}
	
	

	
	void RPC_JoinMap(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type == CallType.Server)
		{	
			Param4<string, string, string,string> data;
			if (!ctx.Read(data)) return;
	
			
			Print(string.Format("JoinMap called on Server: %1, %2, %3, %4", data.param1, data.param2, data.param3));
			RegisterMap(data.param1,data.param2,data.param3,data.param4);
			Config_Server.Save();
			PrintRegistry();
			
		}
	}
	
	void RPC_LeaveMap(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type == CallType.Server)
		{	
			Param3<string, string,string> data;
			if (!ctx.Read(data)) return;
	
			
			Print(string.Format("LeaveMap called on Server: %1, %2,%3", data.param1, data.param2,data.param3));
			UnregisterMap(data.param1,data.param2,data.param3);
			Config_Server.Save();
			PrintRegistry();
			
		}
	}
	void RPC_SendPingToServer(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{

		
		if(type == CallType.Server)
		{	
			Param4<string, string,string,vector> data;
			if (!ctx.Read(data)) return;
			
			string mapname = data.param1;
			string username = data.param2;
			
			string steamid = data.param3;
			vector coords = data.param4;
			Print(string.Format("Send Ping: %1, %2,%3, %4", mapname, username,steamid,coords));
	
			int mapPos =0;
			if(FindMapByName(mapname, mapPos))
			{
				for(int i = 0; i < m_registeredMaps.Get(mapPos).CountMembers(); i++)
				{
					string memberSteamid = m_registeredMaps.Get(mapPos).m_members.Get(i).m_steamid;
					PlayerIdentity playerid = GetPlayerIdentityFromSteamId(memberSteamid);
					GetRPCManager().SendRPC("VanillaPPMap", "RPC_BroadcastPingToMembers", 
					new Param2<string,vector>(username, coords), false, playerid);
				
				}
			}
			
			
			
		}
	}
	

	
	PlayerIdentity GetPlayerIdentityFromSteamId(string steamid)
	{
		array<PlayerIdentity> players = new array<PlayerIdentity>;
		GetGame().GetPlayerIndentities(players);
		
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetPlainId() == steamid)
			{
				return players.Get(i);
			}
		
		}
		
		return null;
	}
	
	void RegisterMap(string mapname, string password, string username, string steamid)
	{
		MapMember member = new MapMember(username, steamid);
		MapRegistryEntry entry = new MapRegistryEntry(mapname, password, member);
		
		int mapPos = -1;
	/*	
		if(FindMapByMember(member, mapPos)) // insure that player is only a member of one map
		{
			UnregisterMap(m_registeredMaps.Get(mapPos).m_mapname, username, steamid);
			Print("Leaving prior maps.");
		}
	*/
		
		int pos = -1;
		if(!FindMapByName(mapname, pos))  // this is the original entry
		{
			m_registeredMaps.Insert(entry);
			Print(string.Format("Successfully registered mapname: [%1]", entry.m_mapname));
		}
		else // map is already registered
		{
			if(m_registeredMaps.Get(pos).m_password == password)
			{
			
				Print(string.Format("[%1] is already registered. Adding new member.", entry.m_mapname));
				m_registeredMaps.Get(pos).AddMember(member);
			}
			else
			{
				Print("Password incorrect. Member not added.");
			
			}
		}
	}
	
	void UnregisterMap(string mapname, string username, string steamid)
	{
		MapMember member = new MapMember(username, steamid);
		
		int pos = -1;
		if(FindMapByName(mapname, pos))
		{
			m_registeredMaps.Get(pos).RemoveMember(member);
			Print(string.Format("Removed member from map [%1]", mapname));
			if(!m_registeredMaps.Get(pos).HasMembers())
			{
				m_registeredMaps.Remove(pos);
			
			}
		}
		else
		{
			Print(string.Format("MapManager-Map not found: [%1]", mapname));
		
		}
	}
	
	
	
	bool FindMapByName(string mapname, out int pos)
	{
		for(int i = 0; i < m_registeredMaps.Count(); i++)
		{
			if(m_registeredMaps.Get(i).m_mapname == mapname)
			{
				pos = i;
				return true;
			}
		
		}
		
		return false;
	
	}
	bool FindMapByMember(MapMember member, out int pos = 0)
	{
		int mapPos;
		for(int i = 0; i < m_registeredMaps.Count(); i++)
		{
			if(m_registeredMaps.Get(i).FindMember(member, mapPos))
			{
				pos = i;
				return true;
			}
		
		}
		
		return false;
	}
	
	void PrintRegistry()
	{
		int c = m_registeredMaps.Count();
		for(int i=0; i< c; i++)
		{
			Print(string.Format("Map [%1]",i));
			m_registeredMaps.Get(i).PrintEntry();
		
		
		}
	
	}
	
	void SaveMaps()
	{
	
	
	
	
	
	}

}