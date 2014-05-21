package com.example.crystalcrazegame;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;

enum WhichView{GAME_VIEW, MAIN_MENU_VIEW};
public class MainActivity extends Activity {

    private MainMenuView mmv;
    private GameView gv;
    public int width;
    public int height;
    public SharedPreferences sp;
    public final String SCORE_KEY = "SCORE";
    
    Handler hd = new Handler()
    {
    	public void handleMessage(Message msg)
    	{
    		switch(msg.what)
    		{
    		case 0:
    			gotoGameView();
    			break;
    		case 1:
    			gotoMainMenuView();
    			break;
    		}
    	}
    };
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // ����Ϊȫ��
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
        		WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        // ��ȡ��Ļ�ߴ�
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        if (dm.heightPixels > dm.widthPixels)
        {
        	width = dm.widthPixels;
        	height = dm.heightPixels;
        }
        else
        {
        	width = dm.heightPixels;
        	height = dm.widthPixels;
        }
        
        sp = getPreferences(MODE_PRIVATE);
       
        gotoMainMenuView();
    }
    // ���뿪ʼ����
    public void gotoMainMenuView()
    {
    	mmv = new MainMenuView(this);
    	setContentView(mmv);
    }
    // ������Ϸ����
    public void gotoGameView()
    {
    	gv = new GameView(this);
    	setContentView(gv);
    }
}
