/*====================================================================
	自機に関するアニメーションの初期化データ
=====================================================================*/

//文字グラフィックの矩形範囲
RECT	Mj[]={//ビットマップ座標を予め登録
	{ 96,  0,112, 16},			// 0  0
	{112,  0,128, 16},			// 1  1
	{128,  0,144, 16},			// 2  2
	{144,  0,160, 16},			// 3  3
	{160,  0,176, 16},			// 4  4
	{176,  0,192, 16},			// 5  5
	{192,  0,208, 16},			// 6  6
	{208,  0,224, 16},			// 7  7
	{224,  0,240, 16},			// 8  8
	{240,  0,256, 16},			// 9  9
	{ 96, 16,112, 32},			// 10 A
	{112, 16,128, 32},			// 11 B
	{128, 16,144, 32},			// 12 C
	{144, 16,160, 32},			// 13 D
	{160, 16,176, 32},			// 14 E
	{176, 16,192, 32},			// 15 F
	{192, 16,208, 32},			// 16 G
	{208, 16,224, 32},			// 17 H
	{224, 16,240, 32},			// 18 I
	{240, 16,256, 32},			// 19 J
	{256, 16,272, 32},			// 20 K
	{272, 16,288, 32},			// 21 L
	{288, 16,304, 32},			// 22 M
	{304, 16,320, 32},			// 23 N
	{320, 16,336, 32},			// 24 O
	{336, 16,352, 32},			// 25 P
	{352, 16,368, 32},			// 26 Q
	{368, 16,384, 32},			// 27 R
	{384, 16,400, 32},			// 28 S
	{400, 16,416, 32},			// 29 T
	{416, 16,432, 32},			// 30 U
	{432, 16,448, 32},			// 31 V
	{448, 16,464, 32},			// 32 W
	{464, 16,480, 32},			// 33 X
	{480, 16,496, 32},			// 34 Y
	{496, 16,512, 32},			// 35 Z
	{512, 16,528, 32},			// 36 カーソル
	{  0,232,168,256},			// 37 CONTINUE ?
	{176,232,192,256},			// 38 0(大)
	{192,232,208,256},			// 39 1(大)
	{208,232,224,256},			// 40 2(大)
	{224,232,240,256},			// 41 3(大)
	{240,232,256,256},			// 42 4(大)
	{256,232,272,256},			// 43 5(大)
	{272,232,288,256},			// 44 6(大)
	{288,232,304,256},			// 45 7(大)
	{304,232,320,256},			// 46 8(大)
	{320,232,336,256}			// 47 9(大)
};

//自機の矩形範囲
RECT	SpJIKI[]={//ビットマップ座標を予め登録
	{  6,  0, 26, 32},			// 0 メイン
	{ 32,  0, 96, 16},			// 1 弾吸収時１
	{ 32, 16, 96, 32},			// 2 弾吸収時２
	{  0, 56, 32, 88},			// 3 アーム１
	{  0, 56, 32, 96},			// 4 アーム２
	{  0, 56, 32,104},			// 5 アーム３
	{  0, 56, 32,112},			// 6 アーム４
	{  0, 56, 32,120},			// 7 アーム５
	{  0, 56, 32,128},			// 8 アーム全開
	{ 32, 32, 96, 96},			// 9 爆発１
	{ 96, 32,160, 96},			//10 爆発２
	{160, 32,224, 96},			//11 爆発３
	{224, 32,288, 96},			//12 爆発４
	{ 32, 96, 96,160},			//13 爆発５
	{ 96, 96,160,160},			//14 爆発６
	{160, 96,224,160},			//15 爆発７
	{224, 96,288,160},			//16 爆発８
	{416, 64,544, 96},			//17 ゲージEXP枠
	{288, 96,416,128},			//18 ゲージEXP空白
	{288, 32,416, 64},			//19 ゲージEXP
	{416, 96,544,128},			//20 ゲージFUEL枠
	{416, 32,544, 64},			//21 ゲージFUEL空白
	{288, 64,416, 96},			//22 ゲージFUEL
	{  0,160,320,176},			//23 ゲージPWU枠
	{  0,176,320,192},			//24 ゲージPWU
	{  0,192,320,208},			//25 ゲージPWU枠2
	{  0,208,320,224},			//26 ゲージPWU2
	{  0,144, 32,160}			//27 OPT用枠
};

//各自機のアニメーションパターン数
BYTE	AmeJIKI[]={
	3,					//アーム発射時の自機
	11,					//アーム発射
	8,					//機体爆発
	2					//弾吸収中の自機						
};			
//アニメーションパターン
BYTE	PatJIKI[][12]={
	{2,1,2},					//アーム発射時の自機
	{3,4,5,6,7,8,7,6,5,4,3},	//アーム発射
	{9,10,11,12,13,14,15,16},	//機体爆発
	{2,1}						//弾吸収中の自機						
};
//アニメーションパターン交換タイミング
BYTE	FramJIKI[][12]={
	{6,7,6},					//アーム発射時の自機
	{1,1,1,1,1,1,1,1,1,1,1},	//アーム発射
	{10,10,10,10,10,10,10,10},	//機体爆発
	{2,2}						//弾吸収中の自機
};

//-----------------------------------------------
// 自機の発射する弾のアニメーションデータ
//-----------------------------------------------

//サーフェス上の弾グラフィックの矩形範囲
RECT	TamSPJIKI[]={
	//弾のグラフィック
	{544, 32,548,104},		// 0 レーザーレベル1(1)
	{548, 32,552,104},		// 1 レーザーレベル1(2)
	{552, 32,556,104},		// 2 レーザーレベル1(3)
	{556, 32,560,104},		// 3 レーザーレベル1(4)
	{560, 32,576, 64},		// 4 ミサイルレベル1(1)
	{560, 64,576, 96},		// 5 ミサイルレベル1(2)
	{582, 32,602, 64},		// 6 オプション(1)
	{614, 32,634, 64},		// 7 オプション(2)
	{582, 64,602, 96},		// 8 オプション(3)
	{614, 64,634, 96},		// 9 オプション(4) 
	{320,128,336,144},		//10 ホーミング(0°)
	{336,128,352,144},		//11 ホーミング(15°)
	{352,128,368,144},		//12 ホーミング(30°)
	{368,128,384,144},		//13 ホーミング(45°)
	{384,128,400,144},		//14 ホーミング(60°)
	{400,128,416,144},		//15 ホーミング(75°)
	{320,144,336,160},		//16 ホーミング(90°)
	{336,144,352,160},		//17 ホーミング(105°)
	{352,144,368,160},		//18 ホーミング(120°)
	{368,144,384,160},		//19 ホーミング(135°)
	{384,144,400,160},		//20 ホーミング(150°)
	{400,144,416,160},		//21 ホーミング(165°)
	{320,160,336,176},		//22 ホーミング(180°)
	{336,160,352,176},		//23 ホーミング(195°)
	{352,160,368,176},		//24 ホーミング(210°)
	{368,160,384,176},		//25 ホーミング(225°)
	{384,160,400,176},		//26 ホーミング(240°)
	{400,160,416,176},		//27 ホーミング(255°)
	{320,176,336,192},		//28 ホーミング(270°)
	{336,176,352,192},		//29 ホーミング(285°)
	{352,176,368,192},		//30 ホーミング(300°)
	{368,176,384,192},		//31 ホーミング(315°)
	{384,176,400,192},		//32 ホーミング(330°)
	{400,176,416,192},		//33 ホーミング(345°)
	{416,144,448,176},		//34 大玉花火1
	{448,144,480,176},		//35 大玉花火2
	{416,176,448,208},		//36 大玉花火3
	{448,176,480,208},		//37 大玉花火4
	{416,128,432,144},		//38 小玉花火1
	{432,128,448,144},		//39 小玉花火2
	{448,128,464,144},		//40 小玉花火3
	{464,128,480,144},		//41 小玉花火4
};

//各SPのアニメーションパターン数
BYTE	TamAmeJIKI[]={
	4,						//レーザー１
	2,						//ミサイル1
	6,						//オプション
	4,						//大玉花火
	4						//小玉花火
};			

//アニメーションパターン
BYTE	TamPatJIKI[][10]={
	{0,1,2,3},				//レーザー１
	{4,5},					//ミサイル1
	{6,7,8,9,8,7},			//オプション
	{34,35,36,37},			//大玉花火
	{38,39,40,41}			//小玉花火
};

//アニメーションパターンホーミングミサイル用
BYTE	HmmPatJIKI[24]={
	10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33
};

//アニメーション切り替えタイミング
BYTE	TamFrameJIKI[][10]={
	{1,1,1,1},				//レーザー１	
	{1,1},					//ミサイル1
	{1,1,1,1,1,1},			//オプション
	{2,2,2,2},				//大玉花火
	{2,2,2,2}				//小玉花火
};
