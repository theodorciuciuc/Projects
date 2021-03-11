package game;

import java.io.IOException;
import java.util.ArrayList;

import fileio.implementations.FileWriter;
import main.GameInput;
import map.Map;
import players.Player;
import players.PlayerFactory;

public final class GameLogic {
    private static GameLogic instance = null;
    private ArrayList<Player> players;
    private Map map;
    private PlayerFactory playerFactory;
    private char[][] rounds;
    private int nrRounds;

    private GameLogic() {
        playerFactory = PlayerFactory.getInstance();
        players = new ArrayList<Player>();
    }

    public static GameLogic getInstance() {
        if (instance == null) {
            instance = new GameLogic();
        }
        return instance;
    }

    public void input(final GameInput gameInput) {
        nrRounds = gameInput.getNrRounds();
        rounds = gameInput.getRounds();
        map = Map.getInstance();
        map.setLand(gameInput.getMap());
        for (int i = 0; i < gameInput.getNrPlayers(); i++) {
            players.add(playerFactory.createPlayer(gameInput.getPlayers()[i][0].toCharArray()[0],
                    Integer.parseInt(gameInput.getPlayers()[i][1]),
                    Integer.parseInt(gameInput.getPlayers()[i][2])));
        }
    }

    public void logic() {
        for (int k = 0; k < nrRounds; k++) {
            // movement and over-time effects
            for (int j = 0; j < players.size(); j++) {
                players.get(j).applyEffect();
                if (players.get(j).getHp() > 0 && !players.get(j).getIncapacite()) {
                    players.get(j).move(rounds[k][j]);
                }
            }
            // combat
            for (int i = 0; i < players.size(); i++) {
                for (int j = i + 1; j < players.size(); j++) {
                    if (players.get(i).sameCoords(players.get(j))
                            && players.get(i).getHp() > 0
                            && players.get(j).getHp() > 0) {
                        players.get(i).getAttackedBy(players.get(j));
                        players.get(j).getAttackedBy(players.get(i));
                        players.get(j).changeXp(players.get(i));
                        players.get(i).changeXp(players.get(j));
                    }
                    players.get(i).balanceHp();
                    players.get(j).balanceHp();
                }
            }
        }
    }

    public void end(final String path) throws IOException {
        FileWriter fw = new FileWriter(path);
        for (Player player : players) {
            if (player.getHp() <= 0) {
                fw.writeWord(player.getType() + " ");
                fw.writeWord("dead");
                fw.writeNewLine();
            } else {
                fw.writeWord(player.getType() + " ");
                fw.writeWord(player.getLevel() + " ");
                fw.writeWord(player.getXp() + " ");
                fw.writeWord(player.getHp() + " ");
                fw.writeWord(player.getX() + " ");
                fw.writeInt(player.getY());
                fw.writeNewLine();
            }
        }
        fw.close();
    }

}
