package players;

import common.Constants;

public final class Wizard extends Player {

    public Wizard(final char type, final int x, final int y, final int hp) {
        super(type, x, y, hp);
    }

    int drain(final float race, final int maxHp, final int currentHp) {
        float procent = Constants.WIZARD_DRAIN_BASE + getLevel() * Constants.WIZARD_DRAIN_LVL;
        if (map.getLandAtCoords(getX(), getY()) == 'D') {
            procent *= Constants.WIZARD_MAP_BONUS;
        }
        procent *= race;
        float drain = Math.min(Constants.WIZARD_DRAIN_SCALE * maxHp, currentHp);
        drain = Math.round(drain);
        drain *= procent;
        drain = Math.round(drain);
        return (int) drain;
    }

    int deflect(final Knight knight) {
        float procent = Constants.WIZARD_DEFLECT_BASE + getLevel() * Constants.WIZARD_DEFLECT_LVL;
        float damage = Math.round(procent * (knight.execute(1) + knight.slam(1)));
        damage *= Constants.WIZARD_DEFLECT_KNIGHT;
        damage = Math.round(damage);
        if (map.getLandAtCoords(getX(), getY()) == 'D') {
            damage *= Constants.WIZARD_MAP_BONUS;
            damage = Math.round(damage);
        }
        return (int) damage;
    }

    int deflect(final Pyromancer pyromancer) {
        float procent = Constants.WIZARD_DEFLECT_BASE + getLevel() * Constants.WIZARD_DEFLECT_LVL;
        float damage = Math.round(procent * (pyromancer.fireblast(1) + pyromancer.ignite(1)));
        damage *= Constants.WIZARD_DEFLECT_PYROMANCER;
        damage = Math.round(damage);
        if (map.getLandAtCoords(getX(), getY()) == 'D') {
            damage *= Constants.WIZARD_MAP_BONUS;
            damage = Math.round(damage);
        }
        return (int) damage;
    }

    int deflect(final Rogue rogue) {
        float procent = Constants.WIZARD_DEFLECT_BASE + getLevel() * Constants.WIZARD_DEFLECT_LVL;
        float damage = Math.round(procent * (rogue.backstab(1) + rogue.paralysis(1)));
        damage *= Constants.WIZARD_DEFLECT_ROGUE;
        damage = Math.round(damage);
        if (map.getLandAtCoords(getX(), getY()) == 'D') {
            damage *= Constants.WIZARD_MAP_BONUS;
            damage = Math.round(damage);
        }
        return (int) damage;
    }

    @Override
    public void attack(final Knight knight) {
        int maxHp = Constants.KNIGHT_BASE_HP + knight.getLevel() * Constants.KNIGHT_LVL_HP;
        int currentHp = knight.getInitHp();
        knight.changeHp(-drain(Constants.WIZARD_DRAIN_KNIGHT, maxHp, currentHp));
        knight.changeHp(-deflect(knight));
    }

    @Override
    public void attack(final Pyromancer pyromancer) {
        int maxHp = Constants.PYROMANCER_BASE_HP
               + pyromancer.getLevel() * Constants.PYROMANCER_LVL_HP;
        int currentHp = pyromancer.getInitHp();
        pyromancer.changeHp(-drain(Constants.WIZARD_DRAIN_PYROMANCER, maxHp, currentHp));
        pyromancer.changeHp(-deflect(pyromancer));
    }

    @Override
    public void attack(final Rogue rogue) {
        int maxHp = Constants.ROGUE_BASE_HP + rogue.getLevel() * Constants.ROGUE_LVL_HP;
        int currentHp = rogue.getInitHp();
        rogue.changeHp(-drain(Constants.WIZARD_DRAIN_ROGUE, maxHp, currentHp));
        rogue.changeHp(-deflect(rogue));
    }

    @Override
    public void attack(final Wizard wizard) {
        int maxHp = Constants.WIZARD_BASE_HP + wizard.getLevel() * Constants.WIZARD_LVL_HP;
        int currentHp = wizard.getInitHp();
        wizard.changeHp(-drain(Constants.WIZARD_DRAIN_WIZARD, maxHp, currentHp));
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
                changeHp(Constants.WIZARD_BASE_HP + getLevel() * Constants.WIZARD_LVL_HP);
            } else {
                break;
            }
        }
    }

}
