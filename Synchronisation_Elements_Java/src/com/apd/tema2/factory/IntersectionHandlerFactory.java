package com.apd.tema2.factory;

import com.apd.tema2.Main;
import com.apd.tema2.entities.*;
import com.apd.tema2.intersections.*;
import com.apd.tema2.utils.Constants;

import static java.lang.Thread.sleep;

import java.util.concurrent.BrokenBarrierException;

/**
 * Clasa Factory ce returneaza implementari ale InterfaceHandler sub forma unor
 * clase anonime.
 */
public class IntersectionHandlerFactory {

    public static IntersectionHandler getHandler(String handlerType) {
        // simple semaphore intersection
        // max random N cars roundabout (s time to exit each of them)
        // roundabout with exactly one car from each lane simultaneously
        // roundabout with exactly X cars from each lane simultaneously
        // roundabout with at most X cars from each lane simultaneously
        // entering a road without any priority
        // crosswalk activated on at least a number of people (s time to finish all of
        // them)
        // road in maintenance - 2 ways 1 lane each, X cars at a time
        // road in maintenance - 1 way, M out of N lanes are blocked, X cars at a time
        // railroad blockage for s seconds for all the cars
        // unmarked intersection
        // cars racing
        return switch (handlerType) {
            case "simple_semaphore" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	System.out.println("Car " + car.getId() + " has reached the semaphore, now waiting...");
                	try {
						sleep(car.getWaitingTime());
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has waited enough, now driving...");
                }
            };
            case "simple_n_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	System.out.println("Car " + car.getId() + " has reached the roundabout, now waiting...");
                	try {
						sleep(car.getWaitingTime());
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	try {
						((SimpleIntersection)Main.intersection).sem.acquire();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has entered the roundabout");
                	try {
						sleep(((SimpleIntersection)Main.intersection).t);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has exited the roundabout after " + ((SimpleIntersection)Main.intersection).t / 1000 + " seconds");
                	((SimpleIntersection)Main.intersection).sem.release();
                }
            };
            case "simple_strict_1_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	System.out.println("Car " + car.getId() + " has reached the roundabout");
                	try {
						sleep(car.getWaitingTime());
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	try {
						((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].acquire();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	try {
						((SimpleIntersection)Main.intersection).bar.await();
					} catch (InterruptedException | BrokenBarrierException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());
                	try {
						sleep(((SimpleIntersection)Main.intersection).t);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has exited the roundabout after " + ((SimpleIntersection)Main.intersection).t / 1000 + " seconds");
                	((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].release();
                }
            };
            case "simple_strict_x_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	System.out.println("Car " + car.getId() + " has reached the roundabout, now waiting...");
                	try {
						sleep(car.getWaitingTime());
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	try {
						((SimpleIntersection)Main.intersection).bar_first.await();
					} catch (InterruptedException | BrokenBarrierException e1) {
						e1.printStackTrace();
					}
                	try {
						((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].acquire();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " was selected to enter the roundabout from lane " + car.getStartDirection());
                	try {
						((SimpleIntersection)Main.intersection).bar.await();
					} catch (InterruptedException | BrokenBarrierException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());
                	try {
						sleep(((SimpleIntersection)Main.intersection).t);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has exited the roundabout after " + ((SimpleIntersection)Main.intersection).t / 1000 + " seconds");
                	((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].release();
                }
            };
            case "simple_max_x_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    // Get your Intersection instance
                	
                    try {
                        sleep(car.getWaitingTime());
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } // NU MODIFICATI

                    // Continuati de aici
                	System.out.println("Car " + car.getId() + " has reached the roundabout from lane " + car.getStartDirection());
                	try {
						((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].acquire();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());
                	try {
						sleep(((SimpleIntersection)Main.intersection).t);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
                	System.out.println("Car " + car.getId() + " has exited the roundabout after " + ((SimpleIntersection)Main.intersection).t / 1000 + " seconds");
                	((SimpleIntersection)Main.intersection).sem_array[car.getStartDirection()].release();
                }
            };
            case "priority_intersection" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    // Get your Intersection instance   
                	
                    try {
                        sleep(car.getWaitingTime());
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } // NU MODIFICATI
                    
                    // Continuati de aici
                	if (car.getPriority() == 1) {
                		System.out.println("Car " + car.getId() + " with low priority is trying to enter the intersection...");
                		try {
							((SimpleIntersection)Main.intersection).queue.put(car);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
                	}
                	
                    if (car.getPriority() > 1) {
                    	System.out.println("Car " + car.getId() + " with high priority has entered the intersection");
                    	((SimpleIntersection)Main.intersection).volatil.incrementAndGet();
                    	try {
							sleep(2000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
                    	System.out.println("Car " + car.getId() + " with high priority has exited the intersection");
                    	if (((SimpleIntersection)Main.intersection).volatil.decrementAndGet() == 0) {
                        	synchronized(((SimpleIntersection)Main.intersection).sync) {
                        		((SimpleIntersection)Main.intersection).sync.notifyAll();
                        	}
                    	}
                    }
                    else {
                    	synchronized(((SimpleIntersection)Main.intersection).sync){
                    		while (((SimpleIntersection)Main.intersection).volatil.get() != 0) {
                    			try {
                    				((SimpleIntersection)Main.intersection).sync.wait();
								} catch (InterruptedException e) {
									e.printStackTrace();
								}
                    		}
                    		try {
                    			Car aux = ((SimpleIntersection)Main.intersection).queue.take();
                    			System.out.println("Car " + aux.getId() + " with low priority has entered the intersection");
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
                    	}
                    }
                }
            };
            case "crosswalk" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    
                }
            };
            case "simple_maintenance" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	synchronized(((SimpleIntersection)Main.intersection).sync) {
                		if (car.getStartDirection() == 0) {
                			try {
								((SimpleIntersection)Main.intersection).queue.put(car);
								System.out.println("Car " + car.getId() + " from side number 0 has reached the bottleneck");
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
                		}
                		else {
                			try {
								((SimpleIntersection)Main.intersection).queue2.put(car);
								System.out.println("Car " + car.getId() + " from side number 1 has reached the bottleneck");
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
                		}
                	}
                	while (true) {
                		synchronized(((SimpleIntersection)Main.intersection).sync2) {
                			if (((SimpleIntersection)Main.intersection).queue.size() >= ((SimpleIntersection)Main.intersection).n
                					&& ((SimpleIntersection)Main.intersection).volatil.get() == 0) {
                				for (int i = 0; i < ((SimpleIntersection)Main.intersection).n; i++) {
                					try {
										Car aux = ((SimpleIntersection)Main.intersection).queue.take();
										System.out.println("Car " + aux.getId() + " from side number 0 has passed the bottleneck");
										Main.carsNo--;
									} catch (InterruptedException e) {
										e.printStackTrace();
									}
                				}
                				((SimpleIntersection)Main.intersection).volatil.set(1);
                			}
                			if (((SimpleIntersection)Main.intersection).queue2.size() >= ((SimpleIntersection)Main.intersection).n
                					&& ((SimpleIntersection)Main.intersection).volatil.get() == 1) {
                				for (int i = 0; i < ((SimpleIntersection)Main.intersection).n; i++) {
                					try {
										Car aux = ((SimpleIntersection)Main.intersection).queue2.take();
										System.out.println("Car " + aux.getId() + " from side number 1 has passed the bottleneck");
										Main.carsNo--;
									} catch (InterruptedException e) {
										e.printStackTrace();
									}
                				}
                				((SimpleIntersection)Main.intersection).volatil.set(0);
                			}
                			if (Main.carsNo == 0) {
                				break;
                			}
                		}
                	}
                }
            };
            case "complex_maintenance" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    
                }
            };
            case "railroad" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                	synchronized(((SimpleIntersection)Main.intersection).sync) {
                		try {
                			((SimpleIntersection)Main.intersection).queue.put(car);
                			System.out.println("Car " + car.getId() + " from side number " + car.getStartDirection() + " has stopped by the railroad");
                		} catch (InterruptedException e) {
                			e.printStackTrace();
                		}
                    }
                    try {
						((SimpleIntersection)Main.intersection).bar_first.await();
					} catch (InterruptedException | BrokenBarrierException e) {
						e.printStackTrace();
					}
                    if (car.getId() == 0) {
                    	System.out.println("The train has passed, cars can now proceed");
                    }
                    try {
						((SimpleIntersection)Main.intersection).bar_first.await();
					} catch (InterruptedException | BrokenBarrierException e) {
						e.printStackTrace();
					}
                    synchronized(((SimpleIntersection)Main.intersection).sync) {
                    	try {
            				Car aux = ((SimpleIntersection)Main.intersection).queue.take();
            				System.out.println("Car " + aux.getId() + " from side number " + aux.getStartDirection() + " has started driving");
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
            		}
                }
            };
            default -> null;
        };
    }
}
