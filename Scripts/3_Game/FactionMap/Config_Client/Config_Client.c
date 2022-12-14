 class Config_Client_Data
{
   	  bool m_isdirty = false;
      string m_mapname;
      string m_password;
      string m_username; 
	  string m_steamid;

}

class Config_Client
{
	private static ref Config_Client_Data m_Config_Client_Data = NULL;
	private static const string configPath = "$profile:\\FactionMap\\Config_Client_Data.json";
	private static const string configRoot = "$profile:\\FactionMap\\";	
	
	
	
	static Config_Client_Data Get()
	{
		
		if (m_Config_Client_Data == NULL)
		{
			m_Config_Client_Data = LoadConfig();
		} 
		
 		return m_Config_Client_Data;
	}

	static Config_Client_Data Set()
	{
		return Get();
	}
	
	
	private static Config_Client_Data LoadConfig()
	{
		Print("==================================================");
		Config_Client_Data config = new Config_Client_Data;

		if (FileExist(configPath)==0)
        {
            Print("[FactionMap] 'Config does not exist' does not exist");
            CreateDefaultConfig(config);
        }
		else {

			JsonFileLoader<Config_Client_Data>.JsonLoadFile(configPath, config);
			Print("[FactionMap] 'Config' found, loaded config");
		}			

		return config;
	}

	private static void CreateDefaultConfig(Config_Client_Data config)
	{
		config.m_mapname  = "OWG";
		config.m_password = "1234";
		config.m_username = "Your user name";
		config.m_steamid  = GetGame().GetUserManager().GetTitleInitiator().GetUid();

	
		
        if (FileExist(configRoot)==0)
        {
            Print("[FactionMap] '" + configRoot + "' does not exist, creating directory");
            MakeDirectory(configRoot);
        }
		
		Print("[FactionMap] Saving default config file!");			
        Save(configPath, config);
		
	}
	static void Save()
	{
		Get().m_steamid = GetGame().GetUserManager().GetTitleInitiator().GetUid();
		Save(configPath, Get());
	}
	static void Save( string k_Path, Config_Client_Data k_config )
	{
				
        JsonFileLoader<Config_Client_Data>.JsonSaveFile(k_Path, k_config);
	
	}
}
