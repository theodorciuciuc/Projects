package players;

import common.Constants;

public final class PlayerFactory {
    private static PlayerFactory instance = null;

    private PlayerFactory() { }

    public static PlayerFactory getInstance() {
        if (instance == null) {
            instance = new PlayerFactory();
        }
        return instance;
    }

    public Player createPlayer(final char type, final int x, final int y) {
        if (type == 'W') {
            return new Wizard('W', x, y, Constants.WIZARD_BASE_HP);
        }
        if (type == 'K') {
            return new Knight('K', x, y, Constants.KNIGHT_BASE_HP);
        }
        if (type == 'P') {
            return new Pyromancer('P', x, y, Constants.PYROMANCER_BASE_HP);
        }
        if (type == 'R') {
            return new Rogue('R', x, y, Constants.ROGUE_BASE_HP);
        }
        return null;
    }

}
