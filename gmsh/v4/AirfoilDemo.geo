//+
SetFactory("OpenCASCADE");

lc = 5.0e-1;
dx = 0.5;
R = 0.05;
r = R/10;
Dx = 10*dx;

angle = Atan2(dx,(R-r));

Point(1) = {0,0,0,lc/100};
Point(11) = {-R,0,0,lc/100};
Point(2) = {dx,0,0,lc/100};
Point(3) = {R*Cos(angle),R*Sin(angle),0,lc/100};
Point(4) = {R*Cos(angle),-R*Sin(angle),0,lc/100};
Point(5) = {dx+r*Cos(angle),r*Sin(angle),0,lc/100};
Point(6) = {dx+r*Cos(angle),-r*Sin(angle),0,lc/100};
Point(7) = {dx+r,0,0,lc/1200};

// We can constraint the min and max element sizes to stay within reasonnable
// values (see `t10.geo' for more details):
Mesh.MeshSizeMin = lc/1200;
Mesh.MeshSizeMax = lc;

//+
Line(1) = {3, 5};
//+
Line(2) = {6, 4};
//+
Circle(3) = {3, 1, 11};
Circle(33) = {11, 1, 4};
//+
Circle(4) = {6, 2, 7};
Circle(5) = {7, 2, 5};
//Recursive Delete {Point {1};
//Recursive Delete {Point {2};
Rectangle(1) = {-Dx, -Dx, 0, 2*Dx, 2*Dx, 0};
//+
Field[1] = BoundaryLayer;
//+
Field[1].CurvesList = {1, 3, 33, 2, 4,5};
//+
Field[1].Size = 0.0001;
//+
Field[1].SizeFar = 0.01;
Field[1].Thickness = 0.05;

BoundaryLayer Field =1;
Field[3] = MathEval;
Field[3].F="1.0e-2*(1+ (abs(2.0*(x-0.5)))^4)*max(1,abs(y)/0.1)";

Field[4] = MathEval;
Field[4].F="1.0e-2*(1+ (abs(2.0*(x-(1.0))))^2)*max(1,abs(y)/0.1)";

Field[5] = MathEval;
Field[5].F="5.0e-2*(1+ (abs((x-(3.0))))^1)*max(1,abs(y)/0.2)";

Field[7] = Min;
Field[7].FieldsList = {3,4,5};

Background Field = 7;
// We can activate the calculation of mesh element sizes based on curvature
// (here with a target of 20 elements per 2*Pi radians):
Mesh.MeshSizeFromCurvature = 20;

//+
Curve Loop(2) = {36, 37, 34, 35};
//+
Curve Loop(3) = {1, -5, -4, 2, -33, -3};
//+
Plane Surface(2) = {2, 3};
//+
Recursive Delete {
  Surface{1}; 
}
//+
Recursive Delete {
  Point{1}; 
}
//+
Recursive Delete {
  Point{2}; 
}
//+
Extrude {0, 0, .1} {
  Surface{2}; Layers {1}; 
}
