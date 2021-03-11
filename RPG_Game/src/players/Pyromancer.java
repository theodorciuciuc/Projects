package players;

import common.Constants;
import effects.Effect;

public final class Pyromancer extends Player {

    public Pyromancer(final char type, final int x, final int y, final int hp) {
        super(type, x, y, hp);
    }

    int fireblast(final float race) {
        float fireblast = Constants.PYROMANCER_FIREBLAST_BASE + getLevel()
            * Constants.PYROMANCER_FIREBLAST_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'V') {
            fireblast *= Constants.PYROMANCER_MAP_BONUS;
            fireblast = Math.round(fireblast);
        }
        fireblast *= race;
        fireblast = Math.round(fireblast);
        return (int) fireblast;
    }

    int ignite(final float race) {
        float ignite = Constants.PYROMANCER_IGNITE_BASE + getLevel()
            * Constants.PYROMANCER_IGNITE_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'V') {
            ignite *= Constants.PYROMANCER_MAP_BONUS;
            ignite = Math.round(ignite);
        }
        ignite *= race;
        ignite = Math.round(ignite);
        return (int) ignite;
    }

    int overTime(final float race) {
        float overTime = Constants.PYROMANCER_TIME_BASE + getLevel()
            * Constants.PYROMANCER_TIME_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'V') {
            overTime *= Constants.PYROMANCER_MAP_BONUS;
            overTime = Math.round(overTime);
        }
        overTime *= race;
        overTime = Math.round(overTime);
        return (int) overTime;
    }

    @Override
    public void attack(final Knight knight) {
        knight.changeHp(-fireblast(Constants.PYROMANCER_FIREBLAST_KNIGHT));
        knight.changeHp(-ignite(Constants.PYROMANCER_IGNITE_KNIGHT));
        knight.setEffect(new Effect(overTime(Constants.PYROMANCER_IGNITE_KNIGHT), 2, false));
    }

    @Override
    public void attack(final Pyromancer pyromancer) {
        pyromancer.changeHp(-fireblast(Constants.PYROMANCER_FIREBLAST_PYROMANCER));
        pyromancer.changeHp(-ignite(Constants.PYROMANCER_IGNITE_PYROMANCER));
        pyromancer.setEffect(new Effect(overTime(
                Constants.PYROMANCER_IGNITE_PYROMANCER), 2, false));
    }

    @Override
    public void attack(final Rogue rogue) {
        rogue.changeHp(-fireblast(Constants.PYROMANCER_FIREBLAST_ROGUE));
        rogue.changeHp(-ignite(Constants.PYROMANCER_IGNITE_ROGUE));
        rogue.setEffect(new Effect(overTime(Constants.PYROMANCER_IGNITE_ROGUE), 2, false));
    }

    @Override
    public void attack(final Wizard wizard) {
        wizard.changeHp(-fireblast(Constants.PYROMANCER_FIREBLAST_WIZARD));
        wizard.changeHp(-ignite(Constants.PYROMANCER_IGNITE_WIZARD));
        wizard.setEffect(new Effect(overTime(Constants.PYROMANCER_IGNITE_WIZARD), 2, false));
    }

    @Override
    public void getAttackedBy(final Player enemy) {
        enemy.attack(this);
    }

    @Override
    public void levelUp() {
        while (true) {
            if (getXp() >= Constants.XP_BASE + getLevel() * Constants.XP_SCALE) {
                level++;
                resetHp();
                changeHp(Constants.PYROMANCER_BASE_HP + getLevel() * Constants.PYROMANCER_LVL_HP);
            } else {
                break;
            }
        }
    }

}
