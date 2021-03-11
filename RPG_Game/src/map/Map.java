package map;

public final class Map {
    private static Map instance = null;
    private char[][] land;

    private Map() { }

    public static Map getInstance() {
        if (instance == null) {
            instance = new Map();
        }
        return instance;
    }

    public char getLandAtCoords(final int x, final int y) {
        return land[x][y];
    }

    public void setLand(final char[][] land) {
        this.land = land;
    }
}
