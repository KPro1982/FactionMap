modded class VPPMapMenu 
{    
	private ButtonWidget m_JoinMap;
	protected ref JoinMapDialog m_JoinMapDialog;
	private string defaultInfoText;
	
	override void MapDoubleClick(Widget w, int x, int y, int button) {
		string mapname = Config_Client.Get().m_mapname;
		string username = Config_Client.Get().m_username;
		string steamid = Config_Client.Get().m_steamid;
		
        if (button == MouseState.MIDDLE) {
		
			vector coords = GetMapClickPos();
            string text = string.Format("Cursor X/Y: %1 / %2", Math.Round(coords[0]), Math.Round(coords[2]));
			Print(string.Format("Cursor X/Y: %1 / %2", Math.Round(coords[0]), Math.Round(coords[2])));
			GetRPCManager().SendRPC("FactionMap", "RPC_SendPingToServer", new Param4<string,string,string,vector>(mapname, username, steamid, coords));
			DisplayInfo("Ping sent to `" + mapname + "` map.", false, 5);
		}
		super.MapDoubleClick(w, x, y, button);
	}
	
	void DisplayInfo(string newInfo, bool resetDefault = false, int seconds = 10)
	{
		seconds *= 1000;
		if(resetDefault) 
		{
			defaultInfoText = newInfo;
			return;
		}
		m_info_text.SetText(newInfo);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.ResetInfo, seconds, false);

	
	
	}
	void ResetInfo()
	{
		m_info_text.SetText(defaultInfoText);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.DisplayInfo);
	
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
    {
        super.OnClick(w, x, y, button);

        if (w == m_JoinMap)
		{
			Print("Join MapButton Pressed");
			ShowJoinMapDialog();
		
            return true;
        }

        return super.OnClick(w, x, y, button);
    }
	
	

	
	  override Widget Init() {
        g_Game.ShowAllHidden3dMarkers();
        if (!m_Initialized) {
			
			GetRPCManager().AddRPC("VanillaPPMap", "RPC_BroadcastPingToMembers", this, SingleplayerExecutionType.Both);
			
            m_AddressPort = g_Game.GetConnectAddressPort();

            layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FactionMap/scripts/GUI/Layouts/FactionMap.layout" );
            m_MapWidget = MapWidget.Cast( layoutRoot.FindAnyWidget( "Map_Widget" ) );
            WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_MapWidget, this, "MapDoubleClick" );

            Widget panelListFrame = layoutRoot.FindAnyWidget( "panel_list_frame" );
            m_Adapter = new MarkersListAdapter(this, panelListFrame);

            m_PanelEditDialog = layoutRoot.FindAnyWidget( "panel_editdialog" );
            m_MapFakeBg = ImageWidget.Cast(m_PanelEditDialog.FindAnyWidget("map_fakebg"));
            m_MapFakeBg.LoadImageFile(0, "VanillaPPMap\\GUI\\Textures\\map_blurred.paa");

            m_openNewsFeed    = ButtonWidget.Cast(layoutRoot.FindAnyWidget("openNewsFeed"));
			m_JoinMap    = ButtonWidget.Cast(layoutRoot.FindAnyWidget("joinFactionMap"));
            
			m_CoordinatesText = TextWidget.Cast(layoutRoot.FindAnyWidget("coordinates_text"));

            ImageWidget infoImage  = ImageWidget.Cast(layoutRoot.FindAnyWidget( "info_image" ));
            infoImage.LoadImageFile(0, "set:vpp_map_ui image:icon_info");

            m_info_text = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget( "info_text" ));
            string tooltip = "Double-click map to add a marker";
            if (!g_Game.CanUse3DMarkers())
            	tooltip += "\nThis server has disabled 3D markers feature!";

            DisplayInfo(tooltip, true);

            ReloadMarkers();
            m_Initialized = true;
        }
        return layoutRoot;
    }
	
	void ShowJoinMapDialog() {
        g_Game.HideAll3dMarkers();
        if (!m_JoinMapDialog) {
            m_JoinMapDialog = new JoinMapDialog(this, m_PanelEditDialog); // do I need to create a new panel?
        }
        m_MapWidget.Show(false);
        m_MapFakeBg.Show(true);
        m_PanelEditDialog.Show(true);
    /*
		ref array<ref MarkerInfo> markers;
        if (isCustomServer) {
            markers = m_CustomServerMarkers;
        } else {
            markers = m_ClientMarkers;
        }
        MarkerInfo marker = markers.Get(markerIndex);
	
        m_EditDialog.SetMarker(markerIndex, marker, isCustomServer, isNewMarker, markerSuffix);
	*/
    }

    void HideJoinMapDialog() {
        g_Game.ShowAllHidden3dMarkers();
        if (m_JoinMapDialog) {
            m_PanelEditDialog.Show(false);
            m_MapWidget.Show(true);
            m_MapFakeBg.Show(false);
			DisplayInfo("Double middle-click to send ping to map `" + Config_Client.Get().m_mapname + "`", false, 20);
        }
    }
	
	void RPC_BroadcastPingToMembers(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type == CallType.Client)
		{	
			Param2<string, vector> data;
			if (!ctx.Read(data)) return;
				
			// Move this logic into VVPMapMenu somehow
			Print(string.Format("received Ping: %1, %2", data.param1, data.param2));
			DisplayPing(data.param1, data.param2);
			
		}
	}
	
    void DisplayPing(string username, vector pingCoord) {
		
       m_MapWidget.AddUserMark(pingCoord, username, ARGB(255,255,255,0), "VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa");
 
    }
}

