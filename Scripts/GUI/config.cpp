class CfgPatches
{
	class DZ_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"VanillaPlusPlusMap"}; 
	};
};

class CfgMods 
{
	class DZ_FactionMap
	{
		name = "Kpro1982";
		dir = "FactionMap";
		credits = "Kpro1982";
		author = "Kpro1982";
		overview = "A new mod";
		creditsJson = "FactionMap/Scripts/Data/Credits.json";
		versionPath = "FactionMap/Scripts/Data/Version.hpp";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				/* files[]=
				{
					"FactionMap/gui/imagesets/prefabs.imageset"
				}; */
			};

			class widgetStyles
			{
				/*
				files[]=
				{
					"FactionMap/gui/looknfeel/prefabs.styles"
				};
				*/
			};

			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"FactionMap/scripts/common",
					"FactionMap/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"FactionMap/scripts/common",
					"FactionMap/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"FactionMap/scripts/common",
					"FactionMap/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"FactionMap/scripts/common",
					"FactionMap/scripts/5_Mission"
				};
			};
		};
	};
};
