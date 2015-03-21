#include "ContourOnMesh.hxx"

namespace org_modules_graphics
{

void Line::add(const std::vector<Point2D> & pts)
{
    if (points.back() == pts.front())
    {
        points.push_back(pts.back());
    }
    else if (points.back() == pts.back())
    {
        points.push_back(pts.front());
    }
    else if (points.front() == pts.back())
    {
        points.insert(points.begin(), pts.front());
    }
    else if (points.front() == pts.front())
    {
        points.insert(points.begin(), pts.back());
    }
}

void Line::add(Line * line)
{
    if (points.back() == line->points.front())
    {
        points.insert(points.end(), ++line->points.begin(), line->points.end());
    }
    else if (points.back() == line->points.back())
    {
        points.insert(points.end(), ++line->points.rbegin(), line->points.rend());
    }
    else if (points.front() == line->points.back())
    {
        points.insert(points.begin(), line->points.begin(), --line->points.end());
    }
    else if (points.front() == line->points.front())
    {
        points.insert(points.begin(), ++line->points.rbegin(), line->points.rend());
    }
}

void LevelCurve::intersect(const Point3D & A, const Point3D & B, const Point3D & C)
{
    // check the intersection with [AB] and z=zi
    if ((A.z <= zi && zi <= B.z) || (B.z <= zi && zi <= A.z))
    {
        if (A.z == B.z)
        {
            pts.push_back(Point2D(A.x, A.y));
            pts.push_back(Point2D(B.x, B.y));
            edges.push_back(Edge(A.index, B.index));
        }
        else
        {
            const double t = (zi - A.z) / (B.z - A.z);
            pts.push_back(Point2D(A.x + t * (B.x - A.x), A.y + t * (B.y - A.y)));
            edges.push_back(Edge(A.index, B.index));
        }
    }

    // check the intersection with [BC] and z=zi
    if (pts.size() != 2 && ((B.z <= zi && zi <= C.z) || (C.z <= zi && zi <= B.z)))
    {
        if (B.z == C.z)
        {
            pts.push_back(Point2D(B.x, B.y));
            pts.push_back(Point2D(C.x, C.y));
            edges.push_back(Edge(B.index, C.index));
        }
        else
        {
            const double t = (zi - B.z) / (C.z - B.z);
            pts.push_back(Point2D(B.x + t * (C.x - B.x), B.y + t * (C.y - B.y)));
            edges.push_back(Edge(B.index, C.index));
        }
    }

    // check the intersection with [CA] and z=zi
    if (pts.size() != 2 && ((C.z <= zi && zi <= A.z) || (A.z <= zi && zi <= C.z)))
    {
        if (C.z == A.z)
        {
            pts.push_back(Point2D(C.x, C.y));
            pts.push_back(Point2D(A.x, A.y));
            edges.push_back(Edge(C.index, A.index));
        }
        else
        {
            const double t = (zi - C.z) / (A.z - C.z);
            pts.push_back(Point2D(C.x + t * (A.x - C.x), C.y + t * (A.y - C.y)));
            edges.push_back(Edge(C.index, A.index));
        }
    }

    if (pts.size() == 2 && pts.back() != pts.front())
    {
        // we have a segment
        Edge & e1 = edges.front();
        Edge & e2 = edges.back();
        MapEdgeLine::iterator i = map.find(e1);
        MapEdgeLine::iterator j = map.find(e2);
        if (i == map.end() && j == map.end())
        {
            Line * line = new Line(pts);
            line->first = &(*map.insert(e1).first);
            line->second = &(*map.insert(e2).first);
            line->first->line = line;
            line->second->line = line;
        }
        else if (i == map.end())
        {
            Line * line = j->line;
            line->add(pts);
            const Edge * e = &(*map.insert(e1).first);
            e->line = line;
            if (*line->first == e2)
            {
                line->first = e;
            }
            else
            {
                line->second = e;
            }
            map.erase(j);
        }
        else if (j == map.end())
        {
            Line * line = i->line;
            line->add(pts);
            const Edge * e = &(*map.insert(e2).first);
            e->line = line;
            if (*line->first == e1)
            {
                line->first = e;
            }
            else
            {
                line->second = e;
            }
            map.erase(i);
        }
        else
        {
            Line * line1 = i->line;
            Line * line2 = j->line;
            line1->add(pts);
            line1->add(line2);

            const Edge * first, * second;

            if (*line1->first == e1)
            {
                first = line1->second;
            }
            else
            {
                first = line1->first;
            }
            if (*line2->first == e2)
            {
                second = line2->second;
            }
            else
            {
                second = line2->first;
            }

            line1->first = first;
            line1->second = second;
            second->line = line1;

            // line2 is now useless: we can delete it
            delete line2;

            map.erase(i);
            map.erase(j);
        }
    }

    clear();
}

void LevelCurve::computeLines()
{
    for (MapEdgeLine::const_iterator i = map.begin(), e = map.end(); i != e; ++i)
    {
        if (i->line)
        {
            Line * line = i->line;
            xC.push_back(zi);
            yC.push_back(line->points.size());
            for (std::vector<Point2D>::const_iterator j = line->points.begin(), _e = line->points.end(); j != _e; ++j)
            {
                xC.push_back(j->x);
                yC.push_back(j->y);
            }

            line->second->line = 0;
            line->first->line = 0;

            delete line;
        }
    }

    map.clear();
}

bool LevelCurve::compute(const unsigned int size, const double * x, const double * y, const double * z, const unsigned int elementSize, const unsigned int edgeSize, const double * elems, const unsigned int levelSize, const double * levels, std::vector<double> & xC, std::vector<double> & yC)
{
    LevelCurve lc(xC, yC);
    for (unsigned int i = 0; i < levelSize; ++i)
    {
        lc.setLevel(levels[i]);
        for (unsigned int j = 0; j < elementSize; ++j)
        {
            if (elems[j + 1 * elementSize] < 1)
            {
                return false;
            }
            const unsigned int indexA = (unsigned int)elems[j + 1 * elementSize] - 1;
            const Point3D A(indexA, x[indexA], y[indexA], z[indexA]);
            for (unsigned k = 2; k < edgeSize; ++k)
            {
                if (elems[j + k * elementSize] < 1)
                {
                    return false;
                }
                if (elems[j + (k + 1) * elementSize] < 1)
                {
                    return false;
                }
                const unsigned int indexB = (unsigned int)elems[j + k * elementSize] - 1;
                const unsigned int indexC = (unsigned int)elems[j + (k + 1) * elementSize] - 1;
                lc.intersect(A, Point3D(indexB, x[indexB], y[indexB], z[indexB]), Point3D(indexC, x[indexC], y[indexC], z[indexC]));
            }
        }
        lc.computeLines();
    }

    return true;
}
}
