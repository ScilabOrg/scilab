/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.Toolkit;
import java.awt.color.ColorSpace;
import java.awt.image.BufferedImage;
import java.awt.image.ColorConvertOp;
import java.awt.image.RGBImageFilter;
import java.awt.image.FilteredImageSource;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

/**
 * Tools for images
 */
public class ImagesTools {

    public enum INTERPOLATION {BILINEAR, NEAREST_NEIGHBOR, BICUBIC};

    public static void scale(final File file, final double widthFactor, final double heightFactor, final INTERPOLATION interpolation) {
        try {
            BufferedImage img = scale(ImageIO.read(file), widthFactor, heightFactor, interpolation);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot scale: IOException");
        }
    }

    public static void scale(final File file, final int newWidth, final int newHeight, final INTERPOLATION interpolation) {
        try {
            BufferedImage img = scale(ImageIO.read(file), newWidth, newHeight, interpolation);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot scale: IOException");
        }
    }

    public static void scale(final File file, final double factor, final INTERPOLATION interpolation) {
        try {
            BufferedImage img = scale(ImageIO.read(file), factor, interpolation);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot scale: IOException");
        }
    }

    public static BufferedImage scale(final BufferedImage image, final double widthFactor, final double heightFactor, final INTERPOLATION interpolation) {
        final int w = image.getWidth();
        final int h = image.getHeight();
        final BufferedImage newImage = new BufferedImage((int) Math.round(Math.abs(w * widthFactor)), (int) Math.round(Math.abs(h * heightFactor)), BufferedImage.TYPE_INT_ARGB);
        final Graphics2D g2d = newImage.createGraphics();

        switch (interpolation) {
        case BILINEAR :
            g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
            break;
        case NEAREST_NEIGHBOR :
            g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
            break;
        case BICUBIC :
            g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BICUBIC);
        }

        g2d.scale(widthFactor, heightFactor);
        g2d.drawImage(image, 0, 0, null);
        g2d.dispose();

        return newImage;
    }

    public static BufferedImage scale(final BufferedImage image, final int newWidth, final int newHeight, final INTERPOLATION interpolation) {
        final int w = image.getWidth();
        final int h = image.getHeight();
        return scale(image, ((double) newWidth) / ((double) w), ((double) newWidth) / ((double) w), interpolation);
    }

    public static BufferedImage scale(final BufferedImage image, final double factor, final INTERPOLATION interpolation) {
        return scale(image, factor, factor, interpolation);
    }

    public static void rotateDegre(final File file, final double angle, final Color bgColor) {
        try {
            BufferedImage img = rotateDegre(ImageIO.read(file), angle, bgColor);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot rotate: IOException");
        }
    }

    public static void rotateDegre(final File file, final double angle) {
        try {
            BufferedImage img = rotateDegre(ImageIO.read(file), angle);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot rotate: IOException");
        }
    }

    public static void rotateRadian(final File file, final double angle) {
        try {
            BufferedImage img = rotateRadian(ImageIO.read(file), angle);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot rotate: IOException");
        }
    }

    public static void rotateRadian(final File file, final double angle, final Color bgColor) {
        try {
            BufferedImage img = rotateRadian(ImageIO.read(file), angle, bgColor);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot rotate: IOException");
        }
    }

    public static BufferedImage rotateDegre(final BufferedImage image, final double angle, final Color bgColor) {
        return rotateRadian(image, angle * Math.PI / 180, bgColor);
    }

    public static BufferedImage rotateDegre(final BufferedImage image, final double angle) {
        return rotateDegre(image, angle, new Color(0, 0, 0, 0));
    }

    public static BufferedImage rotateRadian(final BufferedImage image, final double angle) {
        return rotateRadian(image, angle, new Color(0, 0, 0, 0));
    }

    public static BufferedImage rotateRadian(final BufferedImage image, final double angle, final Color bgColor) {
        final double w = (double) image.getWidth();
        final double h = (double) image.getHeight();

        final double s = Math.sin(-angle);
        final double c = Math.cos(-angle);
        final double xmax = Math.max(-h * s, Math.max(0, Math.max(w * c, w * c - h * s)));
        final double xmin = Math.min(-h * s, Math.min(0, Math.min(w * c, w * c - h * s)));
        final double ymax = Math.max(h * c, Math.max(0, Math.max(w * s, w * s + h * c)));
        final double ymin = Math.min(h * c, Math.min(0, Math.min(w * s, w * s + h * c)));
        final int width = (int) (xmax - xmin);
        final int height = (int) (ymax - ymin);

        final BufferedImage newImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        final Graphics2D g2d = newImage.createGraphics();
        g2d.setBackground(bgColor);
        g2d.clearRect(0, 0, width, height);
        g2d.translate(-xmin, -ymin);
        g2d.rotate(-angle, 0, 0);
        g2d.drawImage(image, 0, 0, null);
        g2d.dispose();

        return newImage;
    }

    public static void grayscale(final File file) {
        try {
            BufferedImage img = grayscale(ImageIO.read(file));
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot grayscale: IOException");
        }
    }

    public static BufferedImage grayscale(final BufferedImage image) {
        ColorSpace cs = ColorSpace.getInstance(ColorSpace.CS_GRAY);
        ColorConvertOp op = new ColorConvertOp(cs, null);
        return op.filter(image, null);
    }

    public static void red(final File file) {
        try {
            BufferedImage img = red(ImageIO.read(file));
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot redify: IOException");
        }
    }

    public static void green(final File file) {
        try {
            BufferedImage img = green(ImageIO.read(file));
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot greenify: IOException");
        }
    }

    public static void blue(final File file) {
        try {
            BufferedImage img = blue(ImageIO.read(file));
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot blueify: IOException");
        }
    }

    public static BufferedImage red(final BufferedImage image) {
        return filter(image, 0xFFFF0000);
    }

    public static BufferedImage green(final BufferedImage image) {
        return filter(image, 0xFF00FF00);
    }

    public static BufferedImage blue(final BufferedImage image) {
        return filter(image, 0xFF0000FF);
    }

    public static BufferedImage filter(final BufferedImage image, final int bitfilter) {
        RGBImageFilter filter = new RGBImageFilter() {
                public int filterRGB(int x, int y, int rgb) {
                    return rgb & bitfilter;
                }
            };

        FilteredImageSource source = new FilteredImageSource(image.getSource(), filter);
        Image img = Toolkit.getDefaultToolkit().createImage(source);
        BufferedImage bufimg = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = bufimg.createGraphics();
        g2d.drawImage(img, 0, 0, null);
        g2d.dispose();

        return bufimg;
    }

    public static void replace(final File file, final Color src, final Color dest) {
        try {
            BufferedImage img = replace(ImageIO.read(file), src, dest);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot replace color: IOException");
        }
    }

    public static BufferedImage replace(final BufferedImage image, final Color src, final Color dest) {
        final int sRGB = src.getRGB();
        final int dRGB = dest.getRGB();
        RGBImageFilter filter = new RGBImageFilter() {
                public int filterRGB(int x, int y, int rgb) {
                    return rgb == sRGB ? dRGB : rgb;
                }
            };

        FilteredImageSource source = new FilteredImageSource(image.getSource(), filter);
        Image img = Toolkit.getDefaultToolkit().createImage(source);
        BufferedImage bufimg = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = bufimg.createGraphics();
        g2d.drawImage(img, 0, 0, null);
        g2d.dispose();

        return bufimg;
    }

    public static void crop(final File file) {
        try {
            BufferedImage img = crop(ImageIO.read(file));
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot crop: IOException");
        }
    }

    public static void crop(final File file, final int bgColor) {
        try {
            BufferedImage img = crop(ImageIO.read(file), bgColor);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot crop: IOException");
        }
    }

    public static void crop(final File file, final Color color) {
        try {
            BufferedImage img = crop(ImageIO.read(file), color);
            ImageIO.write(img, getExtension(file), file.getAbsoluteFile());
        } catch (IOException e) {
            System.err.println("Cannot crop: IOException");
        }
    }

    public static BufferedImage crop(final BufferedImage image) {
        return crop(image, 0, true);
    }

    public static BufferedImage crop(final BufferedImage image, final int bgColor) {
        return crop(image, bgColor, false);
    }

    public static BufferedImage crop(final BufferedImage image, final Color color) {
        return crop(image, color.getRGB(), false);
    }

    private static BufferedImage crop(final BufferedImage image, final int bgColor, final boolean firstPixelAsBg) {
        final int w = image.getWidth();
        final int h = image.getHeight();
        final int[] data = new int[w * h];
        image.getRGB(0, 0, w, h, data, 0, w);

        boolean init = false;
        final int empty = firstPixelAsBg ? data[0] : bgColor;
        int minW = -1;
        int maxW = -1;
        int minH = -1;
        int maxH = -1;
        int wi = 0;

        for (int i = 0; i < h; i++) {
            boolean changed = false;
            for (int j = 0; j < w; j++) {
                final int b = data[j + wi];
                if (b != empty) {
                    if (!init) {
                        minW = maxW = j;
                        minH = maxH = i;
                        init = true;
                    } else {
                        if (j < minW) {
                            minW = j;
                        } else if (j > maxW) {
                            maxW = j;
                        }

                        if (!changed) {
                            if (i < minH) {
                                minH = i;
                                changed = true;
                            } else if (i > maxH) {
                                maxH = i;
                                changed = true;
                            }
                        }
                    }
                }
            }
            wi += w;
        }

        if (minW == -1 || maxW == -1 || minH == -1 || maxH == -1) {
            return image;
        }

        return image.getSubimage(minW, minH, maxW - minW + 1, maxH - minH + 1);
    }

    private static String getExtension(File file) {
        String name = file.getName();
        int dotPos = name.indexOf('.');
        if (dotPos == -1) {
            return "png";
        }

        return name.substring(dotPos + 1, name.length()).toLowerCase();
    }

    public static void main(String[] args) {
        boolean crop = false;
        if (args[0].startsWith("--")) {
            String opt = args[0].substring(2);
            if (opt.equalsIgnoreCase("crop")) {
                crop = true;
            }
        }

        if (crop) {
            for (int i = 1; i < args.length; i++) {
                crop(new File(args[i]));
            }
        }
    }
}
