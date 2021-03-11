package main;

public class GameInput {
    private int rows;
    private int columns;
    private char[][] map;
    private int nrPlayers;
    private String[][] players;
    private int nrRounds;
    private char[][] rounds;

    public GameInput(final int rows, final int columns, final char[][] map,
            final int nrPlayers, final String[][] players, final int nrRounds,
            final char[][] rounds) {
        this.rows = rows;
        this.columns = columns;
        this.map = map;
        this.nrPlayers = nrPlayers;
        this.players = players;
        this.nrRounds = nrRounds;
        this.rounds = rounds;
    }

    public final int getRows() {
        return rows;
    }


    public final int getColumns() {
        return columns;
    }


    public final char[][] getMap() {
        return map;
    }

    public final int getNrPlayers() {
        return nrPlayers;
    }

    public final String[][] getPlayers() {
        return players;
    }

    public final int getNrRounds() {
        return nrRounds;
    }

    public final char[][] getRounds() {
        return rounds;
    }

}
