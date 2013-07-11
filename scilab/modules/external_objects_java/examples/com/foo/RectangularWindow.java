package fr.cnes.jimsIntro.example3;

public class RectangularWindow implements IWindow {

    double	length;
    double	width;

    public RectangularWindow(double length, double width) {
        this.length = length;
        this.width = width;
    }

    public double getWindowArea() {
        return length * width;
    }
}