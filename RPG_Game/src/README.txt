All the logic of the program is in the class GameLogic. The function input
takes the input from files, the function logic manages the interactions between
players and the function end outputs the results of the battle in a file.
I created the abstract class Player which is inherited by the hero classes:
Knight, Pyromancer, Rogue, Wizard. The concept of double-dispatch is implemented
in the method getAttackedBy(Player enemy). This method calls enemy.attacks(this)
and each hero has his own methods to attack each particular class. The class
Constants has all the constants in order to calculate the damage each player
does to each class.
