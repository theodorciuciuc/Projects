package players;

import common.Constants;
import effects.Effect;

public final class Knight extends Player {

    public Knight(final char type, final int x, final int y, final int hp) {
        super(type, x, y, hp);
    }

    public void getAttackedBy(final Player enemy) {
        enemy.attack(this);
    }

    int execute(final float race) {
        float execute = Constants.KNIGHT_EXECUTE_BASE + getLevel() * Constants.KNIGHT_EXECUTE_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'L') {
            execute *= Constants.KNIGHT_MAP_BONUS;
            execute = Math.round(execute);
        }
        execute *= race;
        execute = Math.round(execute);
        return (int) execute;
    }

    int slam(final float race) {
        float slam = Constants.KNIGHT_SLAM_BASE + getLevel() * Constants.KNIGHT_SLAM_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'L') {
            slam *= Constants.KNIGHT_MAP_BONUS;
            slam = Math.round(slam);
        }
        slam *= race;
        slam = Math.round(slam);
        return (int) slam;
    }

    @Override
    public void attack(final Knight knight) {
        knight.changeHp(-execute(Constants.KNIGHT_EXECUTE_KNIGHT));
        knight.changeHp(-slam(Constants.KNIGHT_SLAM_KNIGHT));
        knight.setEffect(new Effect(0, 1, true));
    }

    @Override
    public void attack(final Pyromancer pyromancer) {
        pyromancer.changeHp(-execute(Constants.KNIGHT_EXECUTE_PYROMANCER));
        pyromancer.changeHp(-slam(Constants.KNIGHT_SLAM_PYROMANCER));
        pyromancer.setEffect(new Effect(0, 1, true));
    }

    @Override
    public void attack(final Rogue rogue) {
        rogue.changeHp(-execute(Constants.KNIGHT_EXECUTE_ROGUE));
        rogue.changeHp(-slam(Constants.KNIGHT_SLAM_ROGUE));
        rogue.setEffect(new Effect(0, 1, true));
    }

    @Override
    public void attack(final Wizard wizard) {
        wizard.changeHp(-execute(Constants.KNIGHT_EXECUTE_WIZARD));
        wizard.changeHp(-slam(Constants.KNIGHT_SLAM_WIZARD));
        wizard.setEffect(new Effect(0, 1, true));
    }

    @Override
    public void levelUp() {
        while (true) {
            if (getXp() >= Constants.XP_BASE + getLevel() * Constants.XP_SCALE) {
                level++;
                resetHp();
                changeHp(Constants.KNIGHT_BASE_HP + getLevel() * Constants.KNIGHT_LVL_HP);
            } else {
                break;
            }
        }
    }

}
