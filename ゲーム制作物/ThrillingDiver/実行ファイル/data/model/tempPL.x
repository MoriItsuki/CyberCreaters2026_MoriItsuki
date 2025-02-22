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
 40;
 8.35644;8.35644;-10.00000;,
 10.00000;10.00000;0.00000;,
 10.00000;0.00000;0.00000;,
 10.00000;0.00000;-10.00000;,
 10.00000;10.00000;10.00000;,
 10.00000;0.00000;10.00000;,
 10.00000;-10.00000;-0.00000;,
 8.35644;-8.35644;-10.00000;,
 10.00000;-10.00000;10.00000;,
 10.00000;10.00000;10.00000;,
 0.00000;10.00000;10.00000;,
 0.00000;0.00000;10.00000;,
 10.00000;0.00000;10.00000;,
 -10.00000;10.00000;10.00000;,
 -10.00000;0.00000;10.00000;,
 0.00000;-10.00000;10.00000;,
 10.00000;-10.00000;10.00000;,
 -10.00000;-10.00000;10.00000;,
 -10.00000;10.00000;10.00000;,
 -10.00000;10.00000;0.00000;,
 -10.00000;0.00000;0.00000;,
 -10.00000;0.00000;10.00000;,
 -8.35644;8.35644;-10.00000;,
 -10.00000;0.00000;-10.00000;,
 -10.00000;-10.00000;0.00000;,
 -10.00000;-10.00000;10.00000;,
 -8.35644;-8.35644;-10.00000;,
 0.00000;10.00000;0.00000;,
 -10.00000;10.00000;0.00000;,
 10.00000;10.00000;0.00000;,
 0.00000;10.00000;-10.00000;,
 -8.35644;8.35644;-10.00000;,
 8.35644;8.35644;-10.00000;,
 -10.00000;-10.00000;0.00000;,
 0.00000;-10.00000;-0.00000;,
 10.00000;-10.00000;-0.00000;,
 -8.35644;-8.35644;-10.00000;,
 0.00000;-10.00000;-10.00000;,
 8.35644;-8.35644;-10.00000;,
 0.00000;0.00000;-30.00000;;
 
 28;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;3,2,6,7;,
 4;2,5,8,6;,
 4;9,10,11,12;,
 4;10,13,14,11;,
 4;12,11,15,16;,
 4;11,14,17,15;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;21,20,24,25;,
 4;20,23,26,24;,
 4;18,10,27,28;,
 4;10,4,29,27;,
 4;28,27,30,31;,
 4;27,29,32,30;,
 4;33,34,15,25;,
 4;34,35,8,15;,
 4;36,37,34,33;,
 4;37,38,35,34;,
 3;30,23,22;,
 3;30,0,3;,
 3;37,26,23;,
 3;37,3,7;,
 3;30,39,23;,
 3;3,39,30;,
 3;39,3,37;,
 3;37,23,39;;
 
 MeshMaterialList {
  1;
  28;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  34;
  -0.991896;0.097268;-0.081742;,
  0.000000;0.970083;-0.242775;,
  -0.970083;0.000000;-0.242775;,
  -0.991896;-0.097268;-0.081742;,
  0.000000;-0.970083;-0.242775;,
  0.991896;0.097268;-0.081742;,
  0.997972;0.048733;-0.040954;,
  0.970083;0.000000;-0.242775;,
  0.999159;0.000000;-0.041003;,
  0.991896;-0.097268;-0.081742;,
  0.997972;-0.048733;-0.040954;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.997972;0.048733;-0.040954;,
  -0.999159;0.000000;-0.041003;,
  -0.997972;-0.048733;-0.040954;,
  0.000000;0.999159;-0.041003;,
  0.000000;-0.999159;-0.041003;,
  -0.666667;0.666667;-0.333333;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.048733;0.997972;-0.040954;,
  0.048733;0.997972;-0.040954;,
  -0.097268;0.991896;-0.081742;,
  0.097268;0.991896;-0.081742;,
  -0.048733;-0.997972;-0.040954;,
  0.000000;-1.000000;-0.000000;,
  0.048733;-0.997972;-0.040954;,
  -0.097268;-0.991896;-0.081742;,
  0.097268;-0.991896;-0.081742;,
  0.000000;0.000000;-1.000000;,
  0.666667;0.666667;-0.333333;,
  0.666667;-0.666667;-0.333333;,
  -0.666667;-0.666667;-0.333333;;
  28;
  4;5,6,8,7;,
  4;6,11,11,8;,
  4;7,8,10,9;,
  4;8,11,11,10;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;19,13,14,19;,
  4;13,0,2,14;,
  4;19,14,15,19;,
  4;14,2,3,15;,
  4;20,20,16,21;,
  4;20,20,22,16;,
  4;21,16,1,23;,
  4;16,22,24,1;,
  4;25,17,26,26;,
  4;17,27,26,26;,
  4;28,4,17,25;,
  4;4,29,27,17;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;1,18,2;,
  3;7,31,1;,
  3;32,7,4;,
  3;4,2,33;;
 }
 MeshTextureCoords {
  40;
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;;
 }
}
