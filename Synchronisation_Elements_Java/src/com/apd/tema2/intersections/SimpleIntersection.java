package com.apd.tema2.intersections;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

import com.apd.tema2.entities.Car;
import com.apd.tema2.entities.Intersection;

public class SimpleIntersection implements Intersection {
    // Define your variables here.
	public int n;
	public int t;
	public int x;
	public Semaphore sem;
	public Semaphore[] sem_array;
	public CyclicBarrier bar;
	public CyclicBarrier bar_first;
	public ArrayBlockingQueue<Car> queue;
	public volatile AtomicInteger volatil;
	public Object sync;
	public ArrayBlockingQueue<Car> queue2;
	public Object sync2;
}
