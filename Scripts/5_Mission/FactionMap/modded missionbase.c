modded class MissionBase
{
	ref MapManager m_MapManager;

	void MissionBase()
	{
		m_MapManager = MapManager.Cast(MapManager.Get());
	}

	override void OnKeyPress(int key)
	{
		Print("Key Press Detected");
		super.OnKeyPress(key);

		if (key == KeyCode.KC_K)
		{
			
		}
	}
}
