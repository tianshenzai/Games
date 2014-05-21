package com.example.crystalcrazegame;

import java.util.Random;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.media.MediaPlayer;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * @author AllenIverson
 *
 */
public class MainMenuView extends SurfaceView implements SurfaceHolder.Callback, Runnable {

	private Bitmap background;
	private Bitmap startbt;
	private Bitmap startbtdown;
	private Bitmap aboutbt;
	private Bitmap aboutbtdown;
	private Bitmap text;
	private Bitmap scorefont;
	private int scorenum = 0; // 分数长度
	private int scoreArr[] = new int[9]; // 存储分数
	private Bitmap[] baoshi = new Bitmap[5]; 
	private boolean sbdown = false; // 按下开始
	private boolean abdown = false; // 按下关于
	private boolean thflag = false; // 线程标志
	private Paint paint;
	private MainActivity act;
	private Thread th;
	private Rect srcb; // 背景
	private Rect dstb;
	private Rect srct; // 文字
	private Rect dstt;
	private Rect srcs; // 开始
	private Rect dsts;
	private Rect srca; // 关于
	private Rect dsta;
	private Rect srcsc; // 分数
	private Rect dstsc;
	private int danweix; // x轴单位
//	private int danweiy; // y轴单位
	private int  count = 0;
	private long starttime = System.nanoTime();
	private Random r = new Random();
	public String result; // 最高分
	MediaPlayer mp;
	public class status
	{
		public int x;
		public int y;
		public int num;
	}
	private status[] star = new status[5]; 
	
	public MainMenuView(MainActivity act) {
		super(act);
		// TODO Auto-generated constructor stub

		this.act = act;
		this.getHolder().addCallback(this);
		paint = new Paint();
		paint.setAntiAlias(true);
		result = act.sp.getString(act.SCORE_KEY, null);
		mp = MediaPlayer.create(this.act, R.raw.loop);
		mp.setLooping(true);
		mp.start();
	}
	
	// 初始化星星
	public void initstar(int i)
	{
		if (star[i] == null)
			star[i] = new status();
		star[i].x = 0;
		star[i].y = r.nextInt(act.height * 2 / 3);
		int j = 0;
		while (j < i)
		{
			if (Math.abs(star[j].y - star[i].y) <= 20)
				star[i].y = r.nextInt(act.height * 2 / 3);
			else
				j++;
		}
		star[i].num = r.nextInt(5);
	}
	
	// 加载图片，定义位置
	public void init()
	{
		// 加载图片
		background = BitmapFactory.decodeResource(getResources(), R.drawable.background);
		startbt = BitmapFactory.decodeResource(getResources(), R.drawable.play);
		startbtdown = BitmapFactory.decodeResource(getResources(), R.drawable.playdown);
		aboutbt = BitmapFactory.decodeResource(getResources(), R.drawable.about);
		aboutbtdown = BitmapFactory.decodeResource(getResources(), R.drawable.aboutdown);
		text = BitmapFactory.decodeResource(getResources(), R.drawable.logo);
		scorefont = BitmapFactory.decodeResource(getResources(), R.drawable.scorefont);
		baoshi[0] = BitmapFactory.decodeResource(getResources(), R.drawable.p0);
		baoshi[1] = BitmapFactory.decodeResource(getResources(), R.drawable.p1);
		baoshi[2] = BitmapFactory.decodeResource(getResources(), R.drawable.p2);
		baoshi[3] = BitmapFactory.decodeResource(getResources(), R.drawable.p3);
		baoshi[4] = BitmapFactory.decodeResource(getResources(), R.drawable.p4);
		// 计算位置
		srcb = new Rect(0, 0, background.getWidth(), background.getHeight());
		dstb = new Rect(0, 0, act.width, act.height);
		srct = new Rect(0, 0, text.getWidth(), text.getHeight());
		dstt = new Rect(act.width / 10, act.height / 10, act.width * 9 / 10, act.height * 5 / 10);
		srcs = new Rect(0, 0, startbt.getWidth(), startbt.getHeight());
		dsts = new Rect(act.width * 2 / 10, act.height * 5 / 10, act.width * 8 / 10, act.height * 7 / 10);
		srca = new Rect(0, 0, aboutbt.getWidth(), aboutbt.getHeight());
		dsta = new Rect(act.width * 2 / 10, act.height * 7 / 10, act.width * 8 / 10, act.height * 9 / 10);
		srcsc = new Rect(0, 0, scorefont.getWidth() / 10, scorefont.getHeight());
		dstsc = new Rect(act.width * 9 / 20, act.height * 9 / 10 , act.width * 11 / 20, act.height);
		
		// 初始化
		danweix = act.width / 8;
//		danweiy = act.height / 12;
		
		for (int i = 0; i < 9; i++)
			scoreArr[i] = -1;
		
		if (result != null)
			scorenum = result.length();
		
		for (int i = 0; i < scorenum; i++)
		{
			scoreArr[i] = result.charAt(i) - '0';
		}
		
		for (int i = 0; i < 5; i++)
			initstar(i);
	}
	
	// 绘制
	public void ondraw(Canvas canvas)
	{
		// 画背景
		canvas.drawBitmap(background, srcb, dstb, paint);
		// 画星星
		for (int i = 0; i < 5; i++)
		{
			if (star[i].x >= 0 && star[i].x <= act.width && star[i].y >= 0 && star[i].y <= act.height)
			{
				star[i].x += 5;
				star[i].y -= 5;
				canvas.drawBitmap(baoshi[star[i].num], star[i].x, star[i].y, paint);
			}
			else
			{
				initstar(i);
				canvas.drawBitmap(baoshi[star[i].num], star[i].x, star[i].y, paint);
			}
		}
		// 画文字
		canvas.drawBitmap(text, srct, dstt, paint);
		// 画开始按钮
		if (sbdown)
			canvas.drawBitmap(startbtdown, srcs, dsts, paint);
		else
			canvas.drawBitmap(startbt, srcs, dsts, paint);
		// 画关于按钮
		if (abdown)
			canvas.drawBitmap(aboutbtdown, srca, dsta, paint);
		else
			canvas.drawBitmap(aboutbt, srca, dsta, paint);
		// 画分数
		if (result != null)
		{
			for (int i = 0; i < scorenum; i++)
			{
//				System.out.println(scoreArr[i]);
				srcsc.left = scorefont.getWidth() * scoreArr[i] / 10;
				srcsc.right = scorefont.getWidth() * (scoreArr[i]+1) / 10;
				dstsc.left = act.width / 2 + (i - scorenum/2) * danweix / 2;
				dstsc.right = dstsc.left + danweix/2;
				canvas.drawBitmap(scorefont, srcsc, dstsc, paint);
			}
		}
		else 
		{
			canvas.drawBitmap(scorefont, srcsc, dstsc, paint);
		}
	}
	
	// 重画
	public void repaint()
	{
		SurfaceHolder holder = this.getHolder();
		Canvas canvas = holder.lockCanvas();
		try
		{
			synchronized(holder)
			{
				ondraw(canvas);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			if (canvas != null)
				holder.unlockCanvasAndPost(canvas);
		}
	}
	

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		int currentNum = event.getAction();
		float x = event.getX();
		float y = event.getY();
		
		switch (currentNum)
		{
		case MotionEvent.ACTION_DOWN:
			if (dsts.left < x && dsts.right > x && dsts.top < y && dsts.bottom > y)
			{
				sbdown = true;
			}
			else if (dsta.left < x && dsta.right > x && dsta.top < y && dsta.bottom > y)
			{
				abdown = true;
			}
			break;
		case MotionEvent.ACTION_UP:
			if (sbdown)
			{
				sbdown = false;
				if (dsts.left < x && dsts.right > x && dsts.top < y && dsts.bottom > y)
				{
					act.hd.sendEmptyMessage(0);
				}
			}
			if (abdown)
			{
				abdown = false;
			}
			break;
		}

		return true;
	}

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		// TODO Auto-generated method stub
		
		
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
		init();
		th = new Thread(this);
		th.start();
		thflag = true;
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
		thflag = false;
		mp.release();
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		while (thflag)
		{
			try
			{
				repaint();
				if (!mp.isLooping())
				count++;
				if (count > 60)
				{
					count = 0;
					long temptime = System.nanoTime();
					long span = temptime - starttime;
					starttime = temptime;
					Thread.sleep(1000 - span / 1000);
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
	}

}
