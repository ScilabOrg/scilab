/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef TRIANGULATOR_HXX
#define TRIANGULATOR_HXX

#include <algorithm>
#include <list>
#include <vector>

/*
 * A structure representing a point.
 */
typedef struct
{
    double x;
    double y;
    double z;
} Vector3d;

/**
 * Triangulator class
 * An implementation of the ear-clipping triangulation algorithm,
 * an O(n^2) complexity algorithm, when n is the triangulated polygon's
 * number of points. The polygon must be simple and non-intersecting.
 * Triangulation occurs as if the polygon were plane, which is why only
 * its two largest dimensions are considered and the third is ignored.
 *
 * To do:
 *    -clean-up and comment
 *    -optimize: iterate only over the list of reflex vertices when performing
 *     the ear test. It requires the lists of convex and reflex vertices to be updated during the execution;
 *     they are not used at the moment, just filled at initialization time.
 *    -extend to take into account self-intersecting polygons
 *    -use a more efficient algorithm (such as the decomposition into monotone pieces O(n log n) algorithm)
 *    -use a dedicated library (more efficient and/or robust)
 */
class Triangulator
{
private:
    /** A pointer to the array of input points. */
    std::vector<Vector3d> inputPoints;

    /** A pointer to the array of points. */
    std::vector<Vector3d> points;

    /** The polygon's number of points. */
    int numPoints;

    /**
     * Specifies which of the polygon's axes is the smallest. 0, 1 and 2
     * respectively correspond to the x, y, and z axes.
     */
    int smallestAxis;

    /** Specifies the polygon's two largest axes. */
    int largestAxes[2];

    /**
     * Specifies whether the list of vertices must be flipped or not.
     * Vertices are ordered counter-clockwise if it is false, whereas they
     * are ordered clockwise if true.
     */
    bool flipped;

    /** The list of vertex indices. */
    std::list<int> vertexIndices;

    /** The list of ear vertex indices. */
    std::list<int> earList;

    /** The list of convex vertices. */
    std::list<int> convexList;

    /** The list of reflex vertices. */
    std::list<int> reflexList;

    /** The convexity flag array. */
    std::vector<int> flagList;

    /** The list of output triangle indices. */
    std::vector<int> triangleIndices;

    /** The number of insertions into the ear vertex list. */
    int numAddEars;

    /** The number of deletions from the ear vertex list. */
    int numDelEars;

    /** The number of steps taken by the triangulation's execution. */
    int numSteps;

    /** The number of ear tests performed. */
    int numEarTests;

private:
    void getAdjacentVertices(std::list<int>::iterator vi, std::list<int>::iterator& vim1, std::list<int>::iterator& vip1);

    int isAnEar(std::list<int>::iterator vertexIndex);

    void updateVertex(std::list<int>::iterator predit);

    void determineSmallestAxis(void);

    void fillPoints(void);

    double computeArea(void);

    void fillVertexIndices(void);

    void fillConvexVerticesList(void);

    void fillEarList(void);

public:
    /** Default constructor. */
    Triangulator(void);

    void addPoint(double x, double y, double z);

    void initialize(int numberPoints);

    int triangulate(void);

    int getNumberTriangles(void);

    int* getIndices(void);

    int getNumberEarTests();

    // Should be moved to a Vector class
    int pointInTriangle(Vector3d A, Vector3d B, Vector3d C, Vector3d P);

    double computeDotProduct(int im1, int i, int ip1);

    /**
     * Clears the internal lists.
     */
    void clear(void);

    // Should be moved to a Vector3d class
    static Vector3d minus(Vector3d v0, Vector3d v1);

    // Should be moved to a Vector3d class
    static double dot(Vector3d v0, Vector3d v1);

    // Should be moved to a Vector3d class
    static Vector3d perpendicularVector(Vector3d v);
};

#endif
