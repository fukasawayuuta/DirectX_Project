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
 120;
 0.00000;1.71938;-0.00000;,
 1.34429;1.07200;-0.00000;,
 1.34429;1.07200;-3.43883;,
 0.00000;1.71938;-3.43883;,
 1.67630;-0.38264;-0.00000;,
 1.67630;-0.38264;-3.43883;,
 0.74602;-1.54916;-0.00000;,
 0.74602;-1.54916;-3.43883;,
 -0.74603;-1.54916;-0.00000;,
 -0.74603;-1.54916;-3.43883;,
 -1.67630;-0.38264;-0.00000;,
 -1.67630;-0.38264;-3.43883;,
 -1.34429;1.07200;-0.00000;,
 -1.34429;1.07200;-3.43883;,
 0.00000;1.71938;-0.00000;,
 0.00000;1.71938;-3.43883;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 0.00000;-0.00000;-3.43883;,
 -0.00000;1.53420;-3.16097;,
 0.54505;1.27172;-3.16097;,
 0.54505;1.27172;-14.16718;,
 -0.00000;1.53420;-14.16718;,
 0.67967;0.68193;-3.16097;,
 0.67967;0.68193;-14.16718;,
 0.30248;0.20894;-3.16097;,
 0.30248;0.20894;-14.16718;,
 -0.30248;0.20894;-3.16097;,
 -0.30248;0.20894;-14.16718;,
 -0.67967;0.68193;-3.16097;,
 -0.67967;0.68193;-14.16718;,
 -0.54505;1.27172;-3.16097;,
 -0.54505;1.27172;-14.16718;,
 -0.00000;1.53420;-3.16097;,
 -0.00000;1.53420;-14.16718;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.83708;-3.16097;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;0.72759;-13.15052;,
 -0.00000;-1.39089;-3.16097;,
 -0.00000;-1.39089;-14.16718;,
 0.54505;-1.12841;-14.16718;,
 0.54505;-1.12841;-3.16097;,
 0.67967;-0.53861;-14.16718;,
 0.67967;-0.53861;-3.16097;,
 0.30248;-0.06562;-14.16718;,
 0.30248;-0.06562;-3.16097;,
 -0.30248;-0.06562;-14.16718;,
 -0.30248;-0.06562;-3.16097;,
 -0.67967;-0.53861;-14.16718;,
 -0.67967;-0.53861;-3.16097;,
 -0.54505;-1.12841;-14.16718;,
 -0.54505;-1.12841;-3.16097;,
 -0.00000;-1.39089;-14.16718;,
 -0.00000;-1.39089;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.69374;-3.16097;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 -0.00000;-0.80323;-13.15052;,
 0.00000;1.89030;-11.74829;,
 1.47791;1.17858;-11.74829;,
 1.47791;1.17858;-13.20661;,
 0.00000;1.89030;-13.20661;,
 1.84292;-0.42066;-11.74829;,
 1.84292;-0.42066;-13.20661;,
 0.82018;-1.70314;-11.74829;,
 0.82018;-1.70314;-13.20661;,
 -0.82018;-1.70314;-11.74829;,
 -0.82018;-1.70314;-13.20661;,
 -1.84292;-0.42066;-11.74829;,
 -1.84292;-0.42066;-13.20661;,
 -1.47791;1.17858;-11.74829;,
 -1.47791;1.17858;-13.20661;,
 0.00000;1.89030;-11.74829;,
 0.00000;1.89030;-13.20661;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;-0.00001;-11.74829;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;,
 0.00000;0.00000;-13.20661;;
 
 84;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 3;16,1,0;,
 3;17,4,1;,
 3;18,6,4;,
 3;19,8,6;,
 3;20,10,8;,
 3;21,12,10;,
 3;22,14,12;,
 3;23,3,2;,
 3;24,2,5;,
 3;25,5,7;,
 3;26,7,9;,
 3;27,9,11;,
 3;28,11,13;,
 3;29,13,15;,
 4;30,31,32,33;,
 4;31,34,35,32;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 3;46,31,30;,
 3;47,34,31;,
 3;48,36,34;,
 3;49,38,36;,
 3;50,40,38;,
 3;51,42,40;,
 3;52,44,42;,
 3;53,33,32;,
 3;54,32,35;,
 3;55,35,37;,
 3;56,37,39;,
 3;57,39,41;,
 3;58,41,43;,
 3;59,43,45;,
 4;60,61,62,63;,
 4;63,62,64,65;,
 4;65,64,66,67;,
 4;67,66,68,69;,
 4;69,68,70,71;,
 4;71,70,72,73;,
 4;73,72,74,75;,
 3;76,60,63;,
 3;77,63,65;,
 3;78,65,67;,
 3;79,67,69;,
 3;80,69,71;,
 3;81,71,73;,
 3;82,73,75;,
 3;83,62,61;,
 3;84,64,62;,
 3;85,66,64;,
 3;86,68,66;,
 3;87,70,68;,
 3;88,72,70;,
 3;89,74,72;,
 4;90,91,92,93;,
 4;91,94,95,92;,
 4;94,96,97,95;,
 4;96,98,99,97;,
 4;98,100,101,99;,
 4;100,102,103,101;,
 4;102,104,105,103;,
 3;106,91,90;,
 3;107,94,91;,
 3;108,96,94;,
 3;109,98,96;,
 3;110,100,98;,
 3;111,102,100;,
 3;112,104,102;,
 3;113,93,92;,
 3;114,92,95;,
 3;115,95,97;,
 3;116,97,99;,
 3;117,99,101;,
 3;118,101,103;,
 3;119,103,105;;
 
 MeshMaterialList {
  4;
  84;
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
   0.461176;0.461176;0.461176;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.389020;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.511373;0.034510;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.109804;0.109804;0.109804;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  46;
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.781831;0.623491;0.000000;,
  0.974927;-0.222523;-0.000000;,
  0.433881;-0.900970;-0.000000;,
  -0.433881;-0.900970;0.000000;,
  -0.974927;-0.222524;0.000000;,
  -0.781831;0.623490;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.000000;1.000000;0.000000;,
  0.781830;0.623491;0.000000;,
  0.974931;-0.222508;-0.000000;,
  0.433895;-0.900964;-0.000000;,
  -0.433894;-0.900964;0.000000;,
  -0.974931;-0.222508;0.000000;,
  -0.781830;0.623491;0.000000;,
  -0.628870;-0.493162;-0.601094;,
  -0.000000;-1.000000;0.000000;,
  0.781833;-0.623488;-0.000000;,
  0.974928;0.222519;0.000000;,
  0.433888;0.900967;0.000000;,
  -0.433888;0.900967;0.000000;,
  -0.974928;0.222518;0.000000;,
  -0.781832;-0.623489;0.000000;,
  -0.661114;0.535351;-0.525669;,
  0.000000;1.000000;0.000000;,
  0.781832;0.623489;0.000000;,
  0.974928;-0.222521;-0.000000;,
  0.433884;-0.900968;-0.000000;,
  -0.433885;-0.900968;0.000000;,
  -0.974928;-0.222520;0.000000;,
  -0.781832;0.623489;0.000000;,
  0.000000;-0.783387;-0.621534;,
  -0.811591;0.193475;-0.551260;,
  -0.370801;0.777975;-0.507209;,
  0.370801;0.777975;-0.507209;,
  0.811591;0.193475;-0.551260;,
  0.628869;-0.493162;-0.601094;,
  0.352976;-0.732963;-0.581527;,
  0.000000;0.865769;-0.500444;,
  -0.797230;-0.173637;-0.578165;,
  -0.344960;-0.707963;-0.616272;,
  0.344960;-0.707963;-0.616272;,
  0.797230;-0.173637;-0.578165;,
  0.661113;0.535352;-0.525669;,
  0.384824;0.799096;-0.461905;;
  84;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,1,1,7;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,9,9,15;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;16,32,16;,
  3;16,16,33;,
  3;34,33,34;,
  3;34,34,35;,
  3;36,35,36;,
  3;36,36,37;,
  3;38,37,32;,
  4;17,17,18,18;,
  4;18,18,19,19;,
  4;19,19,20,20;,
  4;20,20,21,21;,
  4;21,21,22,22;,
  4;22,22,23,23;,
  4;23,23,17,17;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;24,24,39;,
  3;24,40,24;,
  3;41,41,40;,
  3;41,42,41;,
  3;43,43,42;,
  3;43,44,43;,
  3;45,39,44;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,25,25,31;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;;
 }
 MeshTextureCoords {
  120;
  0.000000;0.000000;,
  0.142857;0.000000;,
  0.142857;1.000000;,
  0.000000;1.000000;,
  0.285714;0.000000;,
  0.285714;1.000000;,
  0.428571;0.000000;,
  0.428571;1.000000;,
  0.571429;0.000000;,
  0.571429;1.000000;,
  0.714286;0.000000;,
  0.714286;1.000000;,
  0.857143;0.000000;,
  0.857143;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.071429;0.000000;,
  0.214286;0.000000;,
  0.357143;0.000000;,
  0.500000;0.000000;,
  0.642857;0.000000;,
  0.785714;0.000000;,
  0.928571;0.000000;,
  0.071429;1.000000;,
  0.214286;1.000000;,
  0.357143;1.000000;,
  0.500000;1.000000;,
  0.642857;1.000000;,
  0.785714;1.000000;,
  0.928571;1.000000;,
  0.000000;0.000000;,
  0.142857;0.000000;,
  0.142857;1.000000;,
  0.000000;1.000000;,
  0.285714;0.000000;,
  0.285714;1.000000;,
  0.428571;0.000000;,
  0.428571;1.000000;,
  0.571429;0.000000;,
  0.571429;1.000000;,
  0.714286;0.000000;,
  0.714286;1.000000;,
  0.857143;0.000000;,
  0.857143;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.071429;0.000000;,
  0.214286;0.000000;,
  0.357143;0.000000;,
  0.500000;0.000000;,
  0.642857;0.000000;,
  0.785714;0.000000;,
  0.928571;0.000000;,
  0.071429;1.000000;,
  0.214286;1.000000;,
  0.357143;1.000000;,
  0.500000;1.000000;,
  0.642857;1.000000;,
  0.785714;1.000000;,
  0.928571;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.142857;1.000000;,
  0.142857;0.000000;,
  0.285714;1.000000;,
  0.285714;0.000000;,
  0.428571;1.000000;,
  0.428571;0.000000;,
  0.571429;1.000000;,
  0.571429;0.000000;,
  0.714286;1.000000;,
  0.714286;0.000000;,
  0.857143;1.000000;,
  0.857143;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.071429;0.000000;,
  0.214286;0.000000;,
  0.357143;0.000000;,
  0.500000;0.000000;,
  0.642857;0.000000;,
  0.785714;0.000000;,
  0.928571;0.000000;,
  0.071429;1.000000;,
  0.214286;1.000000;,
  0.357143;1.000000;,
  0.500000;1.000000;,
  0.642857;1.000000;,
  0.785714;1.000000;,
  0.928571;1.000000;,
  0.000000;0.000000;,
  0.142857;0.000000;,
  0.142857;1.000000;,
  0.000000;1.000000;,
  0.285714;0.000000;,
  0.285714;1.000000;,
  0.428571;0.000000;,
  0.428571;1.000000;,
  0.571429;0.000000;,
  0.571429;1.000000;,
  0.714286;0.000000;,
  0.714286;1.000000;,
  0.857143;0.000000;,
  0.857143;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.071429;0.000000;,
  0.214286;0.000000;,
  0.357143;0.000000;,
  0.500000;0.000000;,
  0.642857;0.000000;,
  0.785714;0.000000;,
  0.928571;0.000000;,
  0.071429;1.000000;,
  0.214286;1.000000;,
  0.357143;1.000000;,
  0.500000;1.000000;,
  0.642857;1.000000;,
  0.785714;1.000000;,
  0.928571;1.000000;;
 }
}