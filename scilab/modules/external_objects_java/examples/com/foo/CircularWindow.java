package fr.cnes.jimsIntro.example3;

public class CircularWindow implements IWindow {

    double	radius;

    public CircularWindow(double radius) {
        this.radius = radius;
    }

    public double getWindowArea() {
        return Math.PI * radius * radius;
    }
}