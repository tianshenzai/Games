package com.example.crystalcrazegame;

import java.util.Random;

public class RandomClass {
	Random r;
	private static RandomClass random;
	private RandomClass()
	{
		r = new Random();
	}
	public static RandomClass getInstance()
	{
		if (random == null)
			random = new RandomClass();
		return random;
	}
	public int[] getInt()
	{
		int[] temp = new int[80];
		for (int i = 0; i < 80; i++)
		{
			temp[i] = Math.abs(r.nextInt()) % 5;
		}
		return temp;
	}
	
	public int[] getEight()
	{
		int[] temp = new int[8];
		for (int i = 0; i < 8; i++)
		{
			temp[i] = Math.abs(r.nextInt()) % 5;
		}
		return temp;
	}

}
