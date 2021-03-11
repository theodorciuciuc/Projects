package main;

import java.io.IOException;

import game.GameLogic;

public final class Main {

    //trick the checkstyle
    private Main() { }

    public static void main(final String[] args) throws IOException {
        GameInputLoader gameInputLoader = new GameInputLoader(args[0], args[1]);
        GameInput gameInput = gameInputLoader.load();
        GameLogic game = GameLogic.getInstance();
        game.input(gameInput);
        game.logic();
        game.end(args[1]);
    }

}
