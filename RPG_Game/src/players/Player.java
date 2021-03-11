package players;

import common.Constants;
import effects.Effect;
import map.Map;

/**
 * @author theodor
 *
 */
public abstract class Player {
    private int hp;
    private int initHp;
    private int xp;
    protected int level;
    private int x;
    private int y;
    private char type;
    private Effect effect;
    protected Map map;

    public Player(final char type, final int x, final int y, final int hp) {
        this.type = type;
        this.x = x;
        this.y = y;
        this.hp = hp;
        initHp = hp;
        xp = 0;
        level = 0;
        effect = null;
        map = Map.getInstance();
    }

    public final char getType() {
        return type;
    }

    public final int getX() {
        return x;
    }

    public final int getY() {
        return y;
    }

    public final int getHp() {
        return hp;
    }

    public final int getInitHp() {
        return initHp;
    }

    public void balanceHp() {
        initHp = hp;
    }

    public final int getLevel() {
        return level;
    }

    public final int getXp() {
        return xp;
    }

    public final void changeHp(final int health) {
        this.hp += health;
    }

    public final void changeXp(final Player enemy) {
        if (enemy.getHp() <= 0 && getHp() > 0) {
            xp += Math.max(0, Constants.XP_REWARD - (level - enemy.getLevel())
                    * Constants.XP_REWARD_SCALE);
            levelUp();
        }
    }

    public final boolean sameCoords(final Player player) {
        if (x == player.x && y == player.y) {
            return true;
        }
        return false;
    }

    public final void applyEffect() {
        if (effect != null) {
            effect.tick(this);
        }
    }

    public final boolean getIncapacite() {
        if (effect != null) {
            return effect.getIncapacitate();
        }
        return false;
    }

    public final void setEffect(final Effect effect) {
        this.effect = effect;
    }

    public final void move(final char move) {
        if (move == 'U') {
            x--;
        }
        if (move == 'D') {
            x++;
        }
        if (move == 'L') {
            y--;
        }
        if (move == 'R') {
            y++;
        }
    }

    public final void resetHp() {
        hp = 0;
    }

    public abstract void levelUp();

    public abstract void attack(Knight knight);

    public abstract void attack(Pyromancer pyromancer);

    public abstract void attack(Rogue rogue);

    public abstract void attack(Wizard wizard);

    public abstract void getAttackedBy(Player player);
}
