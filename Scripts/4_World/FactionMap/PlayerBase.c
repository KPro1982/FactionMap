modded class PlayerBase	
{
	
	
	override void OnJumpStart()    
    {
        super.OnJumpStart();         
		autoptr JoinMapWidget m_joinmapwidget= new JoinMapWidget;
		
		Print(string.Format("Hello world"));

		
    }
}
	
class JoinMapWidget
{

	autoptr ButtonWidget m_joinMapButton;
	
	void JoinMapWidget()
	{
	
		m_joinMapButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets( "FactionMap/gui/layouts/factionmap.layout" ));
		m_joinMapButton.Show(true);

	}
	
	
	
}