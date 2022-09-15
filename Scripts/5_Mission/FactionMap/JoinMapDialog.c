class JoinMapDialog extends ScriptedWidgetEventHandler {

    VPPMapMenu m_Menu;


    Widget m_Root;
    EditBoxWidget m_EditBoxMapName;
    Widget m_EditBoxMapNameOverlay;
	
    Widget m_PanelPassword;
	EditBoxWidget m_EditBoxPassword;
	Widget m_EditBoxPasswordOverlay;
	
	Widget m_PanelUserName;
	EditBoxWidget m_EditBoxUserName;
	Widget m_EditBoxUserNameOverlay;

    ButtonWidget m_ButtonCancel;
    ButtonWidget m_ButtonOK;
    Widget m_PanelClose;
    ButtonWidget m_ButtonClose;


    void JoinMapDialog(VPPMapMenu menu, Widget root) {
        m_Menu = menu;
        m_Root = GetGame().GetWorkspace().CreateWidgets( "FactionMap/scripts/GUI/Layouts/JoinMapDialog.layout", root );

		m_EditBoxMapName = EditBoxWidget.Cast(m_Root.FindAnyWidget( "editbox_mapname" ));
		m_EditBoxMapName.SetText(Config_Client.Get().m_mapname);
        m_EditBoxMapNameOverlay = m_Root.FindAnyWidget( "editbox_mapname_overlay" );
        m_EditBoxMapNameOverlay.Show(false);
		
        m_PanelPassword = m_Root.FindAnyWidget( "panel_password" );
		m_EditBoxPassword = EditBoxWidget.Cast(m_Root.FindAnyWidget( "editbox_password" ));
		m_EditBoxPassword.SetText(Config_Client.Get().m_password);
		m_EditBoxPasswordOverlay = m_Root.FindAnyWidget( "editbox_password_overlay" );
		m_EditBoxPasswordOverlay.Show(false);
		
        m_PanelUserName = m_Root.FindAnyWidget( "panel_username" );
		m_EditBoxUserName = EditBoxWidget.Cast(m_Root.FindAnyWidget( "editbox_username" ));
		m_EditBoxUserName.SetText(Config_Client.Get().m_username);
		m_EditBoxUserNameOverlay = m_Root.FindAnyWidget( "editbox_username_overlay" );
		m_EditBoxUserNameOverlay.Show(false);
				
        m_ButtonCancel = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_cancel" ));
        m_ButtonOK = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_ok" ));
        m_PanelClose = m_Root.FindAnyWidget( "panel_close" );
        m_ButtonClose = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_close" ));


        m_Root.SetHandler(this);
    }

    override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
		string steamid = GetGame().GetUserManager().GetTitleInitiator().GetUid();
        if (button == MouseState.LEFT) {
            if (w == m_PanelClose || w == m_ButtonClose) {
                m_Menu.HideJoinMapDialog();
                return true;
            } else if (w == m_ButtonCancel) {

				Config_Client.Save();
				

				GetRPCManager().SendRPC("FactionMap", "RPC_LeaveMap", new Param3<string,string,string>(m_EditBoxMapName.GetText(),
					m_EditBoxUserName.GetText(),steamid));

				m_EditBoxMapName.SetText("");
                m_EditBoxPassword.SetText("");
                m_Menu.HideJoinMapDialog();
                return true;
            } else if (w == m_ButtonOK) {
                string mapname = m_EditBoxMapName.GetText();
                string password = m_EditBoxPassword.GetText();
				string username = m_EditBoxUserName.GetText();
				Config_Client.Get().m_mapname = mapname;
				Config_Client.Get().m_password = password;
				Config_Client.Get().m_username = username; 
				Config_Client.Save();
				GetRPCManager().SendRPC("FactionMap", "RPC_JoinMap", new Param4<string,string,string,string>(mapname, password, username, steamid));  
				
				
                m_Menu.HideJoinMapDialog();
                return true;
            } else if (w == m_EditBoxMapNameOverlay) {
                m_EditBoxMapName.SetText("");
                m_EditBoxMapNameOverlay.Show(false);
                return false;
            }
        }
        return false;
    }
}