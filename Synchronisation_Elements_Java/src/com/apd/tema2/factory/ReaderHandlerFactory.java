package com.apd.tema2.factory;

import com.apd.tema2.Main;
import com.apd.tema2.entities.Car;
import com.apd.tema2.entities.Pedestrians;
import com.apd.tema2.entities.ReaderHandler;
import com.apd.tema2.intersections.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Returneaza sub forma unor clase anonime implementari pentru metoda de citire din fisier.
 */
public class ReaderHandlerFactory {

    public static ReaderHandler getHandler(String handlerType) {
        // simple semaphore intersection
        // max random N cars roundabout (s time to exit each of them)
        // roundabout with exactly one car from each lane simultaneously
        // roundabout with exactly X cars from each lane simultaneously
        // roundabout with at most X cars from each lane simultaneously
        // entering a road without any priority
        // crosswalk activated on at least a number of people (s time to finish all of them)
        // road in maintenance - 1 lane 2 ways, X cars at a time
        // road in maintenance - N lanes 2 ways, X cars at a time
        // railroad blockage for T seconds for all the cars
        // unmarked intersection
        // cars racing
        return switch (handlerType) {
            case "simple_semaphore" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) {
                    // Exemplu de utilizare:
                    // Main.intersection = IntersectionFactory.getIntersection("simpleIntersection");
                }
            };
            case "simple_n_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    // To parse input line use:
                    // String[] line = br.readLine().split(" ");
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).t = Integer.parseInt(line[1]);
                	((SimpleIntersection)Main.intersection).sem = new Semaphore(Integer.parseInt(line[0]));
                }
            };
            case "simple_strict_1_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).t = Integer.parseInt(line[1]);
                	((SimpleIntersection)Main.intersection).sem_array = new Semaphore[Integer.parseInt(line[0])];
                	for (int i = 0; i < Integer.parseInt(line[0]); i++) {
                		((SimpleIntersection)Main.intersection).sem_array[i] = new Semaphore(1);
                	}
                	((SimpleIntersection)Main.intersection).bar = new CyclicBarrier(Integer.parseInt(line[0]));
                }
            };
            case "simple_strict_x_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).t = Integer.parseInt(line[1]);
                	((SimpleIntersection)Main.intersection).x = Integer.parseInt(line[2]);
                	((SimpleIntersection)Main.intersection).sem_array = new Semaphore[Integer.parseInt(line[0])];
                	for (int i = 0; i < Integer.parseInt(line[0]); i++) {
                		((SimpleIntersection)Main.intersection).sem_array[i] = new Semaphore(Integer.parseInt(line[2]));
                	}
                	((SimpleIntersection)Main.intersection).bar = new CyclicBarrier(Integer.parseInt(line[0]) * Integer.parseInt(line[2]));
                	((SimpleIntersection)Main.intersection).bar_first = new CyclicBarrier(Main.carsNo);
                }
            };
            case "simple_max_x_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).t = Integer.parseInt(line[1]);
                	((SimpleIntersection)Main.intersection).x = Integer.parseInt(line[2]);
                	((SimpleIntersection)Main.intersection).sem_array = new Semaphore[Integer.parseInt(line[0])];
                	for (int i = 0; i < Integer.parseInt(line[0]); i++) {
                		((SimpleIntersection)Main.intersection).sem_array[i] = new Semaphore(Integer.parseInt(line[2]));
                	}
                	((SimpleIntersection)Main.intersection).bar = new CyclicBarrier(Integer.parseInt(line[0]) * Integer.parseInt(line[2]));
                	((SimpleIntersection)Main.intersection).bar_first = new CyclicBarrier(Main.carsNo);
                }
            };
            case "priority_intersection" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).t = Integer.parseInt(line[1]);
                	((SimpleIntersection)Main.intersection).queue = new ArrayBlockingQueue<Car>(Integer.parseInt(line[1]));
                	((SimpleIntersection)Main.intersection).volatil = new AtomicInteger(0);
                	((SimpleIntersection)Main.intersection).sync = new Object();
                }
            };
            case "crosswalk" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    
                }
            };
            case "simple_maintenance" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	String[] line = br.readLine().split(" ");
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).n = Integer.parseInt(line[0]);
                	((SimpleIntersection)Main.intersection).queue = new ArrayBlockingQueue<Car>(Main.carsNo);
                	((SimpleIntersection)Main.intersection).queue2 = new ArrayBlockingQueue<Car>(Main.carsNo);
                	((SimpleIntersection)Main.intersection).volatil = new AtomicInteger(0);
                	((SimpleIntersection)Main.intersection).sync = new Object();
                	((SimpleIntersection)Main.intersection).sync2 = new Object();
                }
            };
            case "complex_maintenance" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    
                }
            };
            case "railroad" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                	Main.intersection = IntersectionFactory.getIntersection(handlerType);
                	((SimpleIntersection)Main.intersection).queue = new ArrayBlockingQueue<Car>(Main.carsNo);
                	((SimpleIntersection)Main.intersection).bar_first = new CyclicBarrier(Main.carsNo);
                	((SimpleIntersection)Main.intersection).sync = new Object();
                }
            };
            default -> null;
        };
    }

}
