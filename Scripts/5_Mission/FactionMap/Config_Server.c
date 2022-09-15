 class Config_Server_Data
{
   	  ref array<ref MapRegistryEntry> m_registeredMaps;
}

class Config_Server
{
	 private static ref Config_Server_Data m_Config_Server_Data = NULL;
	 private static const string configPath = "$profile:\\FactionMap\\Config_Server_Data.json";
	 private static const string configRoot = "$profile:\\FactionMap\\";	
	
	
	
	static Config_Server_Data Get()
	{
		
		if (m_Config_Server_Data == NULL)
		{
			m_Config_Server_Data = LoadConfig();
		} 
		
 		return m_Config_Server_Data;
	}

	static Config_Server_Data Set()
	{
		return Get();
	}
	
	
	private static Config_Server_Data LoadConfig()
	{
		Print("==================================================");
		Config_Server_Data config = new Config_Server_Data;

		if (FileExist(configPath)==0)
        {
            Print("[FactionMap] 'Config does not exist' does not exist");
            CreateDefaultConfig(config);
        }
		else {

			JsonFileLoader<Config_Server_Data>.JsonLoadFile(configPath, config);
			Print("[FactionMap] 'Config' found, loaded config");
		}
			

		return config;
	}

	private static void CreateDefaultConfig(Config_Server_Data config)
	{
		
	
		config.m_registeredMaps  = new array<ref MapRegistryEntry>;
	
		
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
		Save(configPath, Get());
	}
	static void Save( string k_Path, Config_Server_Data k_config )
	{
		if(GetGame().IsDedicatedServer())
    	{
              JsonFileLoader<Config_Server_Data>.JsonSaveFile(k_Path, k_config);
    	}
		else
		{
			Print("Error: Server Config Data attempting to save on client.");
		}
  
	
	}
}
