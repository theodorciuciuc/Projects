package players;

import common.Constants;
import effects.Effect;

public final class Rogue extends Player {

    private int stabs = Constants.STABS;
    private int initStabs = Constants.STABS;

    public Rogue(final char type, final int x, final int y, final int hp) {
        super(type, x, y, hp);
    }

    @Override
    public void balanceHp() {
        super.balanceHp();
        initStabs = stabs;
    }

    int backstab(final float race) {
        float backstab = Constants.ROGUE_BACKSTAB_BASE + getLevel() * Constants.ROGUE_BACKSTAB_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            if (initStabs % Constants.STABS == 0) {
                backstab *= Constants.ROGUE_CRITICAL;
                backstab = Math.round(backstab);
            }
            backstab *= Constants.ROGUE_MAP_BONUS;
            backstab = Math.round(backstab);
        }
        stabs++;
        // backstab este chemata de fapt de un Deflect
        if (race == 1) {
            stabs--;
        }
        backstab *= race;
        backstab = Math.round(backstab);
        return (int) backstab;
    }

    int paralysis(final float race) {
        float paralysis = Constants.ROGUE_PARALYSIS_BASE + getLevel()
            * Constants.ROGUE_PARALYSIS_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            paralysis *= Constants.ROGUE_MAP_BONUS;
            paralysis = Math.round(paralysis);
        }
        paralysis *= race;
        paralysis = Math.round(paralysis);
        return (int) paralysis;
    }

    @Override
    public void attack(final Knight knight) {
        knight.changeHp(-backstab(Constants.ROGUE_BACKSTAB_KNIGHT));
        knight.changeHp(-paralysis(Constants.ROGUE_PARALYSIS_KNIGHT));
        int effectDuration;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            effectDuration = Constants.ROGUE_PARALYSIS_MAX_DURATION;
        } else {
            effectDuration = Constants.ROGUE_PARALYSYS_BASE_DURATION;
        }
        knight.setEffect(new Effect(paralysis(Constants.ROGUE_PARALYSIS_KNIGHT),
                effectDuration, true));
    }

    @Override
    public void attack(final Pyromancer pyromancer) {
        pyromancer.changeHp(-backstab(Constants.ROGUE_BACKSTAB_PYROMANCER));
        pyromancer.changeHp(-paralysis(Constants.ROGUE_PARALYSIS_PYROMANCER));
        int effectDuration;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            effectDuration = Constants.ROGUE_PARALYSIS_MAX_DURATION;
        } else {
            effectDuration = Constants.ROGUE_PARALYSYS_BASE_DURATION;
        }
        pyromancer.setEffect(new Effect(paralysis(
                Constants.ROGUE_PARALYSIS_PYROMANCER), effectDuration, true));
    }

    @Override
    public void attack(final Rogue rogue) {
        rogue.changeHp(-backstab(Constants.ROGUE_BACKSTAB_ROGUE));
        rogue.changeHp(-paralysis(Constants.ROGUE_PARALYSIS_ROGUE));
        int effectDuration;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            effectDuration = Constants.ROGUE_PARALYSIS_MAX_DURATION;
        } else {
            effectDuration = Constants.ROGUE_PARALYSYS_BASE_DURATION;
        }
        rogue.setEffect(new Effect(paralysis(
                Constants.ROGUE_PARALYSIS_ROGUE), effectDuration, true));
    }

    @Override
    public void attack(final Wizard wizard) {
        wizard.changeHp(-backstab(Constants.ROGUE_BACKSTAB_WIZARD));
        wizard.changeHp(-paralysis(Constants.ROGUE_PARALYSIS_WIZARD));
        int effectDuration;
        if (map.getLandAtCoords(getX(), getY()) == 'W') {
            effectDuration = Constants.ROGUE_PARALYSIS_MAX_DURATION;
        } else {
            effectDuration = Constants.ROGUE_PARALYSYS_BASE_DURATION;
        }
        wizard.setEffect(new Effect(paralysis(
                Constants.ROGUE_PARALYSIS_WIZARD), effectDuration, true));
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
                changeHp(Constants.ROGUE_BASE_HP + getLevel() * Constants.ROGUE_LVL_HP);
            } else {
                break;
            }
        }
    }

}
