xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 58;
 0.00000;13.37516;-10.00000;,
 8.66025;13.37516;-5.00000;,
 8.66025;9.84950;-5.00000;,
 0.00000;9.84950;-10.00000;,
 8.66025;13.37516;5.00000;,
 8.66025;9.84950;5.00000;,
 -0.00000;13.37516;10.00000;,
 -0.00000;9.84950;10.00000;,
 -8.66025;13.37516;5.00000;,
 -8.66025;9.84950;5.00000;,
 -8.66025;13.37516;-5.00000;,
 -8.66025;9.84950;-5.00000;,
 0.00000;13.37516;-10.00000;,
 0.00000;9.84950;-10.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;13.37516;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;9.84950;0.00000;,
 0.00000;10.46703;-3.74357;,
 2.20041;10.46703;-3.02861;,
 2.20041;-11.41155;-3.02861;,
 0.00000;-11.41155;-3.74357;,
 3.56034;10.46703;-1.15683;,
 3.56034;-11.41155;-1.15683;,
 3.56034;10.46703;1.15683;,
 3.56034;-11.41155;1.15683;,
 2.20041;10.46703;3.02861;,
 2.20041;-11.41155;3.02861;,
 -0.00000;10.46703;3.74357;,
 -0.00000;-11.41155;3.74357;,
 -2.20042;10.46703;3.02861;,
 -2.20042;-11.41155;3.02861;,
 -3.56034;10.46703;1.15683;,
 -3.56034;-11.41155;1.15683;,
 -3.56034;10.46703;-1.15683;,
 -3.56034;-11.41155;-1.15683;,
 -2.20041;10.46703;-3.02861;,
 -2.20041;-11.41155;-3.02861;,
 0.00000;10.46703;-3.74357;,
 0.00000;-11.41155;-3.74357;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;,
 0.00000;-11.41155;-0.00000;;
 
 38;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 3;14,1,0;,
 3;15,4,1;,
 3;16,6,4;,
 3;17,8,6;,
 3;18,10,8;,
 3;19,12,10;,
 3;20,3,2;,
 3;21,2,5;,
 3;22,5,7;,
 3;23,7,9;,
 3;24,9,11;,
 3;25,11,13;,
 4;26,27,28,29;,
 4;27,30,31,28;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 3;48,29,28;,
 3;49,28,31;,
 3;50,31,33;,
 3;51,33,35;,
 3;52,35,37;,
 3;53,37,39;,
 3;54,39,41;,
 3;55,41,43;,
 3;56,43,45;,
 3;57,45,47;;
 
 MeshMaterialList {
  1;
  38;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.517647;0.517647;0.517647;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  19;
  0.000000;1.000000;0.000000;,
  0.500000;0.000000;-0.866025;,
  1.000000;0.000000;0.000000;,
  0.500000;0.000000;0.866026;,
  -0.500000;0.000000;0.866025;,
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.500000;0.000000;-0.866025;,
  0.000000;0.000000;-1.000000;,
  0.587785;0.000000;-0.809017;,
  0.951057;0.000000;-0.309017;,
  0.951057;0.000000;0.309017;,
  0.587785;0.000000;0.809017;,
  -0.000000;0.000000;1.000000;,
  -0.587785;0.000000;0.809017;,
  -0.951057;0.000000;0.309017;,
  -0.951057;0.000000;-0.309017;,
  -0.587785;0.000000;-0.809017;,
  0.000000;-1.000000;-0.000000;;
  38;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;7,7,7,7;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,8,8,17;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;;
 }
 MeshTextureCoords {
  58;
  0.000000;0.000000;,
  0.166667;0.000000;,
  0.166667;1.000000;,
  0.000000;1.000000;,
  0.333333;0.000000;,
  0.333333;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.666667;0.000000;,
  0.666667;1.000000;,
  0.833333;0.000000;,
  0.833333;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.083333;0.000000;,
  0.250000;0.000000;,
  0.416667;0.000000;,
  0.583333;0.000000;,
  0.750000;0.000000;,
  0.916667;0.000000;,
  0.083333;1.000000;,
  0.250000;1.000000;,
  0.416667;1.000000;,
  0.583333;1.000000;,
  0.750000;1.000000;,
  0.916667;1.000000;,
  0.000000;0.000000;,
  0.100000;0.000000;,
  0.100000;1.000000;,
  0.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.300000;0.000000;,
  0.300000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.700000;0.000000;,
  0.700000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  0.900000;0.000000;,
  0.900000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.050000;1.000000;,
  0.150000;1.000000;,
  0.250000;1.000000;,
  0.350000;1.000000;,
  0.450000;1.000000;,
  0.550000;1.000000;,
  0.650000;1.000000;,
  0.750000;1.000000;,
  0.850000;1.000000;,
  0.950000;1.000000;;
 }
}