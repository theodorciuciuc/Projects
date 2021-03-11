package main;

import common.Constants;
import fileio.FileSystem;

public final class GameInputLoader {
    private final String mInputPath;
    private final String mOutputPath;

    GameInputLoader(final String inputPath, final String outputPath) {
        mInputPath = inputPath;
        mOutputPath = outputPath;
    }

    public String getMOutputPath() {
        return mOutputPath;
    }

    public GameInput load() {
        int rows = 0;
        int columns = 0;
        char[][] map = null;
        int nrPlayers = 0;
        String[][] players = null;
        int nrRounds = 0;
        char[][] rounds = null;

        try {
            FileSystem fs = new FileSystem(mInputPath, mOutputPath);
            rows = fs.nextInt();
            columns = fs.nextInt();

            map = new char[rows][columns];
            for (int i = 0; i < rows; i++) {
                map[i] = fs.nextWord().toCharArray();
            }

            nrPlayers = fs.nextInt();

            players = new String[nrPlayers][Constants.NUMBER_OF_ARGUEMNTS];
            for (int i = 0; i < nrPlayers; i++) {
                for (int j = 0; j < Constants.NUMBER_OF_ARGUEMNTS; j++) {
                    players[i][j] = fs.nextWord();
                }
            }

            nrRounds = fs.nextInt();

            rounds = new char[nrRounds][nrPlayers];
            for (int i = 0; i < nrRounds; i++) {
                rounds[i] = fs.nextWord().toCharArray();
            }

            fs.close();

        } catch (Exception e1) {
            e1.printStackTrace();
        }

        return new GameInput(rows, columns, map, nrPlayers, players, nrRounds, rounds);
    }
}
