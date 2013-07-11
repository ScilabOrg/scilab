package fr.cnes.jimsIntro.example3;

public class Door {

    private Color	color;

    public Door(Color color) {
        this.color = color;
    }

    public Color getColor() {
        return color;
    }

    public void repaint(Color newColor) {
        color = newColor;
    }
}