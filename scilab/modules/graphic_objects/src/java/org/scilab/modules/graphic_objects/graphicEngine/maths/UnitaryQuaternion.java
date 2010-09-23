/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.maths;

import java.io.Serializable;

/**
 * Author: Lando Pierre
 * Date: january 14, 2010
 *
 * This class provide a representation of unitary quaternion
 */

public class UnitaryQuaternion implements Serializable {
	private static final long serialVersionUID = 6682857231890676877L;
	private static final int MAX_SELF_OPERATION = 16;
	private int opCount = 0;
	private float a, b, c, d;
	private float[] rotationMatrix;
	
	public UnitaryQuaternion() {
		a = 1;
		b = 0;
		c = 0;
		d = 0;
		normalize();
	}

	private UnitaryQuaternion(float a, float b, float c, float d) {
		this.a = a;
		this.b = b;
		this.c = c;
		this.d = d;
		normalize();
	}
	
	private UnitaryQuaternion(double a, double b, double c, double d) {
		this.a = (float) a;
		this.b = (float) b;
		this.c = (float) c;
		this.d = (float) d;
		normalize();
	}
	
	public UnitaryQuaternion(UnitaryQuaternion q) {
		a = q.a;
		b = q.b;
		c = q.c;
		d = q.d;
		normalize();
	}
	
	public UnitaryQuaternion(float alpha, IVector3f v) {
		float t = (float) Math.cos(alpha/2);
		IVector3f nv = v.getNormalized().scale((float) Math.sin(alpha/2));
		
		a = t;
		b = nv.getX();
		c = nv.getY();
		d = nv.getZ();
		selfCheck();
	}
	
	public UnitaryQuaternion(float[] v) {
		if(v!=null && v.length==4) {
			a=v[0]; b=v[1]; c=v[2]; d=v[3];
		} else { // TODO throw invalid data exception
			a = 1;	b = 0;	c = 0;	d = 0;
		}
		normalize();
	}

    public UnitaryQuaternion(double alpha, IVector3f v) {
        this((float) alpha, v);
    }

    public float[] getRotationMatrix() {
		return rotationMatrix.clone();
	}
	public float[] getUnRotateMatrix() {
        return new float[] {
                rotationMatrix[0], rotationMatrix[4], rotationMatrix[8], rotationMatrix[12],
                rotationMatrix[1], rotationMatrix[5], rotationMatrix[9], rotationMatrix[13],
                rotationMatrix[2], rotationMatrix[6], rotationMatrix[10], rotationMatrix[14],
                rotationMatrix[3], rotationMatrix[7], rotationMatrix[11], rotationMatrix[15]
        };
	}
	
	public UnitaryQuaternion inv() {
		return new UnitaryQuaternion(a, -b, -c, -d);
	}
	public UnitaryQuaternion multiply(UnitaryQuaternion q) {
		float ar = a*q.a - b*q.b - c*q.c - d*q.d;
		float br = a*q.b + b*q.a + c*q.d - d*q.c;
		float cr = a*q.c - b*q.d + c*q.a + d*q.b;
		float dr = a*q.d + b*q.c - c*q.b + d*q.a;
		a = ar; b = br; c = cr; d = dr;
		selfCheck();
		return this;
	}
	
	private void selfCheck() {
		if(opCount++ > MAX_SELF_OPERATION)
			normalize();
		else
			computeRotationMatrix();
	}
	
	private void normalize() {
		opCount = 0;
		float f = 1f / (float) Math.sqrt(a*a+b*b+c*c+d*d);
		a*=f; b*=f; c*=f; d*=f;
		computeRotationMatrix();
	}
	
	
	public UnitaryQuaternion times(UnitaryQuaternion q) {
		return new UnitaryQuaternion(this).multiply(q);
	}
	
	public IVector3f conjugate(IVector3f v) {
		return new Vector3f(
				rotationMatrix[0]*v.getX()+rotationMatrix[4]*v.getY()+rotationMatrix[8]*v.getZ(),
				rotationMatrix[1]*v.getX()+rotationMatrix[5]*v.getY()+rotationMatrix[9]*v.getZ(),
				rotationMatrix[2]*v.getX()+rotationMatrix[6]*v.getY()+rotationMatrix[10]*v.getZ()
		);
	}
	public IVector3f conjugateT(IVector3f v) {
		return new Vector3f(
				rotationMatrix[0]*v.getX()+rotationMatrix[1]*v.getY()+rotationMatrix[2]*v.getZ(),
				rotationMatrix[4]*v.getX()+rotationMatrix[5]*v.getY()+rotationMatrix[6]*v.getZ(),
				rotationMatrix[8]*v.getX()+rotationMatrix[9]*v.getY()+rotationMatrix[10]*v.getZ()
		);
	}
	
	private void computeRotationMatrix() {
		float a2=a*a, b2=b*b, c2=c*c, d2=d*d;
		this.rotationMatrix = new float[] {
            a2+b2-c2-d2, 2*(b*c-a*d), 2*(a*c+b*d), 0,
            2*(a*d+b*c), a2-b2+c2-d2, 2*(c*d-a*b), 0,
            2*(b*d-a*c), 2*(a*b+c*d), a2-b2-c2+d2, 0,
            0, 0, 0, 1};
	}
	
	public String toString() {
		return "("+a+", "+b+", "+c+", "+d+")";
	}

	public IVector3f getVectorY() {
		return new Vector3f(rotationMatrix[1], rotationMatrix[5], rotationMatrix[9]);
	}
	
	public UnitaryQuaternion power(double T) {
		// TODO : do a better S.L.E.R.P.
		double cos = a;
		double sin = Math.sqrt(b*b+c*c+d*d);
		
		if(sin < 0.001) return new UnitaryQuaternion();
		
		double t = (T-1)  *  Math.atan2(sin, cos);
		double na = cos*Math.cos(t)-sin*Math.sin(t);
		
		double beta = Math.cos(t)+Math.sin(t)*(cos/sin);
		return new UnitaryQuaternion(na, beta*b, beta*c, beta*d);
	}

	public double[] getData() {
		return new double[] {a, b, c, d};
	}

    public boolean equals(UnitaryQuaternion uq2) {
        return uq2 != null && a == uq2.a && b == uq2.b && c == uq2.c && d == uq2.d;
    }

    public float[] data() {
        return new float[] {a, b, c, d};
    }
}
