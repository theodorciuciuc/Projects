package effects;

import players.Player;

public class Effect {
    private int duration;
    private int damage;
    private boolean incapacitate;

    public Effect(final int damage, final int duration, final boolean incapacitate) {
        this.damage = damage;
        this.duration = duration;
        this.incapacitate = incapacitate;
    }

    public final boolean getIncapacitate() {
        return incapacitate;
    }

    public final void tick(final Player player) {
        if (duration > 0) {
            player.changeHp(-damage);
            duration--;
        } else {
            player.setEffect(null);
        }
    }
}
