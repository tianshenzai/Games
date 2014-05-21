package com.example.crystalcrazegame;

import java.util.Arrays;
import java.util.HashMap;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Scroller;

public class GameView extends View {

	private Bitmap background;
	private Bitmap[] baoshi = new Bitmap[5];
	private Bitmap pause;
	private Bitmap header;
	private Bitmap timer;
	private Bitmap hint;
	private Bitmap scorefont;
	private Bitmap go;
	private MainActivity act;
	private Paint paint;
	private int  count = 180;
	private Rect srcb; // ����
	private Rect dstb;
	private Rect srch; // ͷ��
	private Rect dsth;
	private Rect srct; // ʱ��
	private Rect dstt;
	private Rect srcp; // ��ͣ
	private Rect dstp;
	private Rect srcsc; // ��ʼ����
	private Rect dstsc;
	private Rect srco;  // ��������
	private Rect dsto;
	private Rect srcst; // ��ʼ
	private Rect dstst;
	private int scorenum = 0; // ��������-1
	private int scoreArr[] = new int[9];
	private int score = 0; // �ܷ���
	private int number = 0; // ÿ�εķ���
	private Scroller scroll;
	private int[] gameMatrix = new int[80]; // ��������
	private int danweix; // x�ᵥλ
	private int danweiy; // y�ᵥλ
	private int controll = 0; // �ڶ���ѡ�еĸ���
	private int change = 0;   // ѡ��Ķ��� 1�����ҽ��� 2�����½��� 3���������Ϸ���������
	private boolean eliminate = false; // �������
	private boolean changeback = false; // �ع����
	private float pointX;
	private float pointY;
	private int choose = 100; // ѡ�еĸ���
	private int[] xiaochu = new int[80]; // ��������
	private boolean[] tianbu = new boolean[80]; // �����
	private int[] huanChongGeZi = new int[8]; // �����������
	private int timespan;
	private boolean isover = false; // ������־
	private boolean isstart = true; // ��ʼ��־
	private String result; // ��ʷ��߷�
	private MediaPlayer mp1; // ��������
	private MediaPlayer mp2; // ʱ������
	private SoundPool sp; // ��������
	private HashMap<Integer, Integer> spMap;
	private boolean endsp = true;
	
	public GameView(MainActivity act) {
		super(act);
		// TODO Auto-generated constructor stub
		
		this.act = act;
		paint = new Paint();
		paint.setAntiAlias(true);
		init();
	}
	
	// ��ʼ��
	public void init()
	{
		// ����ͼƬ
		background = BitmapFactory.decodeResource(getResources(), R.drawable.background);
		pause = BitmapFactory.decodeResource(getResources(), R.drawable.pause);
		header = BitmapFactory.decodeResource(getResources(), R.drawable.header);
		timer = BitmapFactory.decodeResource(getResources(), R.drawable.timer);
		hint = BitmapFactory.decodeResource(getResources(), R.drawable.hint);
		scorefont = BitmapFactory.decodeResource(getResources(), R.drawable.scorefont);
		go = BitmapFactory.decodeResource(getResources(), R.drawable.go);
		baoshi[0] = BitmapFactory.decodeResource(getResources(), R.drawable.p0);
		baoshi[1] = BitmapFactory.decodeResource(getResources(), R.drawable.p1);
		baoshi[2] = BitmapFactory.decodeResource(getResources(), R.drawable.p2);
		baoshi[3] = BitmapFactory.decodeResource(getResources(), R.drawable.p3);
		baoshi[4] = BitmapFactory.decodeResource(getResources(), R.drawable.p4);
		for (int i = 0; i < 5; i++)
			baoshi[i] = Bitmap.createScaledBitmap(baoshi[i], act.width / 8, act.height / 12, false);
		// ����λ��
		srcb = new Rect(0, 0, background.getWidth(), background.getHeight());
		dstb = new Rect(0, 0, act.width, act.height);
		srch = new Rect(0, 0, header.getWidth(), header.getHeight());
		dsth = new Rect(0, 0, act.width, act.height * 2 / 12);
		srct = new Rect(0, 0, timer.getWidth(), timer.getHeight());
		dstt = new Rect(act.width / 32 + 1, act.height / 11 + 5, act.width * 31 / 32 - 1, act.height * 7 / 50 + 12);
		srcp = new Rect(0, 0, pause.getWidth(), pause.getHeight());
		dstp = new Rect(0, 0, act.width / 8, act.height / 12);
		srcsc = new Rect(0, 0, scorefont.getWidth() / 10, scorefont.getHeight());
		dstsc = new Rect(act.width * 19 / 20, 0 , act.width * 20 / 20, act.height / 10);
		srco = new Rect(0, 0, 0, scorefont.getHeight());
		dsto = new Rect(0, 0 , 0, 0);
		srcst = new Rect(0, 0, go.getWidth(), go.getHeight());
		dstst = new Rect(act.width*4/10, act.height*4/10, act.width*6/10, act.height*6/10);
		
		// ��ʼ��
		danweix = act.width / 8;
		danweiy = act.height / 12;
		timespan = 60 * 30 / act.width;
		scroll = new Scroller(getContext());
		gameMatrix = RandomClass.getInstance().getInt();
		huanChongGeZi = RandomClass.getInstance().getEight();
		
		for (int i = 0; i < 9; i++)
			scoreArr[i] = 0;
		
		// ��ʼ������
		mp1 = MediaPlayer.create(this.act, R.raw.loop);
		mp1.setLooping(true);
		
		mp2 = MediaPlayer.create(this.act, R.raw.timer);
		mp2.setLooping(true);
		
		sp = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
		spMap = new HashMap<Integer, Integer>();
		spMap.put(1, sp.load(this.act, R.raw.endgame, 1));
		
	}
	
	// ����soundpool����
	public void playSound(int sound, int loop)
	{
		AudioManager mgr = (AudioManager)this.act.getSystemService(Context.AUDIO_SERVICE);
		float streamVolumeCurrent = mgr.getStreamVolume(AudioManager.STREAM_MUSIC);
		float streamVolumeMax = mgr.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		float volume = streamVolumeCurrent / streamVolumeMax;
		sp.play(spMap.get(sound), volume, volume, 1, loop, 1f);
	}

	@Override
	public void onDraw(Canvas canvas)
	{
		// ������
		canvas.drawBitmap(background, srcb, dstb, paint);
		// ��ͷ��
		canvas.drawBitmap(header, srch, dsth, paint);
		// ��ʱ��
		canvas.drawBitmap(timer, srct, dstt, paint);
		// ����ͣ��ť
		canvas.drawBitmap(pause, srcp, dstp, paint);
		// ������
		for (int i = 0; i <= scorenum; i++)
		{
			srcsc.left = scorefont.getWidth() * scoreArr[i] / 10;
			srcsc.right = scorefont.getWidth() * (scoreArr[i]+1) / 10;
			dstsc.left = act.width * (19-i) / 20;
			dstsc.right = act.width * (20-i) / 20;
			canvas.drawBitmap(scorefont, srcsc, dstsc, paint);
		}

		// ����ʼ����
		if (isstart)
		{
			// ������
			for (int i = 0; i < 80; i++)
				canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix, i/8*danweiy + 2*danweiy, paint);
			count--;
			//��������
			if (count >= 0)
			{
				srcst.left = scorefont.getWidth() * count / 60 / 10;
				srcst.right = srcst.left + scorefont.getWidth() / 10;
				srcst.top = 0;
				srcst.bottom = scorefont.getHeight();
				canvas.drawBitmap(scorefont, srcst, dstst, paint);
			}
			// ��go
			else if (count > -30)
			{
				srcst.right = go.getWidth();
				srcst.bottom = go.getHeight();	
				canvas.drawBitmap(go, srcst, dstst, paint);
			}
			// ��ʼ��Ϸ
			else
			{
				isstart = false;
				count = 0;		
				doMatch();
			}
		}
		// ��ʼ��Ϸ
		else if (!isover)
		{
			if (!mp1.isPlaying())
			{
				
				mp1.start();
			}
			if (!mp2.isPlaying())
			{
				
				mp2.start();
			}
			count++;
			if (count > timespan)
			{
				dstt.right--;
				if (dstt.right == 0)
					isover = true;
				count = 0;
			}
			
			// ����Ч��
			if (!eliminate)
			{
				for (int i = 0; i < 80; i++)
				{
					if (change != 0 && controll == i)
					{
						canvas.drawBitmap(baoshi[gameMatrix[i]], pointX, pointY, paint);
					}
					else if (change == 1 && controll == i-1)
					{
						canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix-(pointX-(i-1)%8*danweix), i/8*danweiy + 2 * danweiy - (pointY-((i-1)/8*danweiy + 2 * danweiy)), paint);
					}
					else if (change == 2 && controll == i-8)
					{
						canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix-(pointX-(i-8)%8*danweix), i/8*danweiy + 2 * danweiy - (pointY-((i-1)/8*danweiy + 2 * danweiy)), paint);
					}
					else
					{
						canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix, i/8*danweiy + 2*danweiy, paint);
					}
				}
			}
			// �����Ч��
			else
			{
				for (int i = 79; i >= 0; i--)
				{
					if (tianbu[i])
					{
						if ((i/8-1)*8 + i%8 >= 0)
						{
							tianbu[(i/8-1)*8 + i%8] = true;
							if (gameMatrix[(i/8-1)*8 + i%8] != 100)
								canvas.drawBitmap(baoshi[gameMatrix[(i/8-1)*8 + i%8]], i%8*danweix, (i/8-1)*danweiy + 2 * danweiy + pointY, paint);
						}
						else
						{
							canvas.drawBitmap(baoshi[huanChongGeZi[i%8]], i%8*danweix, (i/8-1)*danweiy + 2 * danweiy + pointY, paint);
						}
					}
					else
					{
						canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix, i/8*danweiy + 2*danweiy, paint);
					}
				}
			}
			
			// ѡ���
			if (choose >= 0 && choose < 80)
			{
				canvas.drawBitmap(hint, choose%8*danweix, choose/8*danweiy + 2*danweiy + 5, paint);
			}
		}
		// ��������
		else
		{
			// ������
			for (int i = 0; i < 80; i++)
				if (gameMatrix[i] != 100)
					canvas.drawBitmap(baoshi[gameMatrix[i]], i%8*danweix, i/8*danweiy + 2*danweiy, paint);
			
			// ֹͣ����
			scroll.abortAnimation();
			
			// ������
			for (int i = 0; i <= scorenum; i++)
			{
				srco.left = scorefont.getWidth() * scoreArr[i] / 10;
				srco.right = scorefont.getWidth() * (scoreArr[i]+1) / 10;
				dsto.left = act.width / 2 + (scorenum/2 - i) * danweix / 2;
				dsto.right = dsto.left + danweix/2;
				dsto.top = act.height * 6 / 12;
				dsto.bottom = act.height * 8 / 12;
				canvas.drawBitmap(scorefont, srco, dsto, paint);
			}
			
			// ��ͣ�������ֺ�ʱ������
			if (mp1.isPlaying())
			{
				mp1.pause();
			}
			if (mp2.isPlaying())
			{
				mp2.pause();
			}
			// ���Ž�������
			if (endsp)
			{
				this.playSound(1, 0);
				endsp = false;
			}
			
			// ������߷�
			result = act.sp.getString(act.SCORE_KEY, null);
			if (result == null)
			{
				SharedPreferences.Editor editor = act.sp.edit();
				editor.putString(act.SCORE_KEY, String.valueOf(score));
				editor.commit();
			}
			else
			{
				int temp = Integer.parseInt(result);
				if (score > temp)
				{
					SharedPreferences.Editor editor = act.sp.edit();
					editor.putString(act.SCORE_KEY, String.valueOf(score));
					editor.commit();
				}
			}
		}
		
	}

	// �ж��Ƿ��и�������
	public boolean doMatch()
	{
		if (xiaochu[0] != 100)
			clearArray();
		int count = 0;
		int arrayPoint = 0;
		number = 0;
		// ���ѯ
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 6;)
			{
				while (j+1 < 8 && gameMatrix[i*8+j] == gameMatrix[i*8+j+1])
				{
					j++;
					count++;
				}
				if (count >= 2)
				{
					for (; count >= 0; count--)
					{
						if (isover)
							return false;
						xiaochu[arrayPoint] = i * 8 + j - count;
						arrayPoint++;
						number++;
					}
				}
				count = 0;
				j++;
			}
		}
		
		// ����ѯ
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; )
			{
				while(j + 1 < 10 && gameMatrix[j * 8 + i] == gameMatrix[(j+1) * 8 + i])
				{
					j++;
					count++;
				}
				if (count >= 2)
				{
					for (; count >= 0; count--)
					{
						if (isover)
							return false;
						xiaochu[arrayPoint] = (j-count) * 8 + i;
						arrayPoint++;						
						number++;
					}
				}
				count = 0;
				j++;
			}
		}
		
		serialArray(); // ���л�����
		if (xiaochu[0] == 100)
			return false;
		else
		{
			doRemove();
			return true;
		}
	}
	
	// ���xiaochu�����tianbu����
	private void clearArray()
	{
		for (int i = 0; i < 80; i++)
		{
			xiaochu[i] = 100;
			tianbu[i] = false;
		}
	}
	
	// ���л�����
	private void serialArray()
	{
		Arrays.sort(xiaochu);
		for (int i = 0; i < 79; i++)
		{
			if (xiaochu[i]==xiaochu[i+1]&&xiaochu[i]!=100) // ȥ���ظ��ĸ���
			{
				xiaochu[i] = 100;
				number--;
			}
		}
		score += number * 2;
		// ���·�������
		int j = 0;
		number *= 2;
		while(true)
		{
			scoreArr[j] += number;
			if (scoreArr[j] >= 10)
			{
				number = scoreArr[j] / 10;
				scoreArr[j] %= 10;
				j++;
			}
			else
			{
				if (j > scorenum)
					scorenum = j;
				break;
			}
		}
		Arrays.sort(xiaochu);
		for (int i = 0; xiaochu[i]!=100; i++)
		{
			tianbu[xiaochu[i]] = true;
			gameMatrix[xiaochu[i]] = 100;
		}
	}
	
	// ���Ƹ���
	private void doRemove()
	{
		eliminate = true;
		scroll.startScroll(0,  danweiy * 2, 0, danweiy * 12, 8000);
	}
	
	// ����
	public void computeScroll()
	{
		super.computeScroll();
		if (scroll.computeScrollOffset())
		{
			// ��������
			if (eliminate)
			{
				pointX = (scroll.getCurrX() - 2*danweix) % danweix;
				pointY = (scroll.getCurrY() - 2*danweiy) % danweiy;
				if (doSth())
				{
					eliminate = true;
				}
				else
				{
					scroll.abortAnimation();
				}
			}
			else
			{
				pointX = scroll.getCurrX();
				pointY = scroll.getCurrY();
			}
			postInvalidate();
		}
		else
		{
			// ����δ��ɣ���������
			if (eliminate)
			{
				if (doSth())
				{
					eliminate = true;
					scroll.startScroll(0,  danweiy*2, 0, danweiy*12, 8000);
				}
				else
				{
					eliminate = false;
					doMatch();
				}
			}
			// ����
			else if (controll >= 0 && change != 0)
			{
				if (change == 1)
				{
					int temp;
					temp=gameMatrix[controll+1];
					gameMatrix[controll+1]=gameMatrix[controll];
					gameMatrix[controll]=temp;
				}
				else if (change == 2)
				{
					int temp;
					temp=gameMatrix[controll+8];
					gameMatrix[controll+8]=gameMatrix[controll];
					gameMatrix[controll]=temp;
				}
				// ������ƥ�䲻�ɹ��ͻع�
				if (!doMatch() && !changeback)
				{
					if (change == 1)
						scroll.startScroll(controll%8*danweix, controll/8*danweix + 2 * danweiy, danweix, 0);
					else if (change == 2)
						scroll.startScroll(controll%8*danweiy, controll/8*danweiy + 2 * danweiy, 0, danweiy);
					
					changeback = true;
				}
				else
				{
					change = 0;
					controll = -1;
				}
			}
			postInvalidate();
		}
	}
	
	// �����
	private boolean doSth()
	{
		for (int i = 79; i >= 0; i--)
		{
			if (tianbu[i])
			{
				if ((i/8-1)*8 + i%8 >= 0)
				{
					gameMatrix[i] = gameMatrix[(i/8-1)*8 + i%8];
				}
				else
				{
					gameMatrix[i] = huanChongGeZi[i%8];
				}
			}
		}
		huanChongGeZi = RandomClass.getInstance().getEight();
		return clearTianbu();
	}
	
	// �ж��Ƿ���δ�����
	private boolean clearTianbu()
	{
		boolean flag = false;
		for (int i = 0; i < 80; i++)
		{
			if (gameMatrix[i] == 100)
			{
				flag = true;
				tianbu[i] = true;
			}
			else
			{
				tianbu[i] = false;
			}
		}
		return flag;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub

		int x = (int)event.getX();
		int y = (int)event.getY();
		if (x <= act.width / 8 && y <= act.height / 12)
		{
			System.exit(0);
		}
		
		if (!isstart && !isover)
		{
			int tempChoose = 64;
			// δƥ����û����
			if (change == 0 && !eliminate)
			{
				if (y > danweiy * 2)
				{
					tempChoose = x / danweix + (y - 2*danweiy) / danweiy * 8;
				}
				if (Math.abs(tempChoose - choose) == 1 && tempChoose/8 == choose/8)
				{
					controll = tempChoose > choose ? choose : tempChoose;
					change = 1;
					changeback = false;
					scroll.startScroll(controll%8*danweix, controll/8*danweiy + 2*danweiy, danweix, 0);
					choose = 100;
				}
				else if (Math.abs(tempChoose/8 - choose/8) == 1 && tempChoose%8 == choose%8)
				{
					controll = tempChoose > choose ? choose : tempChoose;
					change = 2;
					changeback = false;
					scroll.startScroll(controll%8*danweix, controll/8*danweiy + 2*danweiy, 0, danweiy);
					choose = 100;
				}
				else
				{
					choose = tempChoose;
				}
			}
		}
		
		return true;
	}

}
