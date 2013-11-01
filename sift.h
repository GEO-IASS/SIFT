#ifndef _SIFT_H
#define  _SIFT_H

#import "ImageX.tlb" no_namespace named_guids

#include <list>
#include <vector>

typedef unsigned char uchar;

typedef double pixel_t;



#define PI   3.1415926535897932384626433832795

#define INIT_SIGMA 0.5

#define SIGMA 1.6

#define INTERVALS 3

//�Ƚ�ʱ�İ뾶��ֵԽС���ȡ�õ�������Խ��
#define RATIO 10

#define MAX_INTERPOLATION_STEPS 5

//|D(x^)| < 0.03   0.03��ֵ��̫�࣬���ӻ�ʹ���������
#define DXTHRESHOLD 0.03

#define ORI_HIST_BINS 36

#define ORI_SIGMA_TIMES 1.5

#define ORI_WINDOW_RADIUS 3.0 * ORI_SIGMA_TIMES

#define ORI_SMOOTH_TIMES 2

#define ORI_PEAK_RATIO 0.8

#define FEATURE_ELEMENT_LENGTH 128

#define DESCR_HIST_BINS 8

#define IMG_BORDER 5

#define DESCR_WINDOW_WIDTH 4

#define DESCR_SCALE_ADJUST 3

#define DESCR_MAG_THR 0.2

#define INT_DESCR_FCTR 512.0

#define At(index, x, y) (PyrAt(dog_pyr, (index), (x), (y)))

#define Hat(i, j) (*(H+(i)*3 + (j)))

#define HIat(i, j) (*(H_inve+(i)*3 + (j)))

#define DAt(x, y) (*(data+(y)*step+(x)))

//�����ֵ
#define Parabola_Interpolate(l, c, r) (0.5*((l)-(r))/((l)-2.0*(c)+(r)))

#define MINPQ_INIT_NALLOCD 512

#define KDTREE_BBF_MAX_NN_CHKS 200

#define NN_SQ_DIST_RATIO_THR 0.36




//sift�㷨�����Ļ�����λ-Ӱ���
struct Block{
	pixel_t* pBuffer;
	int nXOrigin;
	int nYOrigin;
	int nXSize;
	int nYSize;
	Block(){pBuffer = NULL; nXOrigin = 0; nYOrigin = 0; nXSize = 0; nYSize = 0;}
	Block(pixel_t* buffer, int ox, int oy, int nx, int ny){pBuffer = buffer; nXOrigin = ox; nYOrigin = oy; nXSize = nx; nYSize = ny;}
};



struct Keypoint
{
	int octave; //�ؼ���������
	int interval;// �ؼ������ڲ�

	double offset_interval;//������Ĳ������

	int x; //x,y����,����octave��interval��ȡ�Ĳ���ͼ��
	int y;

	//scale = sigma0*pow(2.0, o+s/S)
	double scale; //�ռ�߶�����

	double dx; //���������꣬�����걻���ų�ԭͼ���С 
	double dy;

	double offset_x;
	double offset_y;

	//��˹���������ڸ���߶����꣬��ͬ�����ͬ���sigmaֵ��ͬ
	//�ؼ�������������ڳ߶�
	double octave_scale; //offset_i;

	double ori;//����

	int descr_length;
	double descriptor[FEATURE_ELEMENT_LENGTH]; //������

	double val;//��ֵ
	void* feature_data;
};

//��ȡ������
void sift(pixel_t* init_pbuf, std::vector<Keypoint>&features,int nxsize, int nysize);


//k-d���Ľڵ�
struct kd_node
{
	int ki;										//�������йؼ�λ������
	double kv;								// �������йؼ�λ��ֵ
	int leaf;									//�����Ҷ�ڵ�Ϊ1������Ϊ0
	Keypoint* features;				//�ýڵ��������
	int n;										//�ýڵ�������ĸ���
	kd_node* kd_left;					//����  
	kd_node* kd_right;				//�Һ���
};

struct bbf_data
{
	double d;
	void* old_data;
};

struct pq_node
{
	void* data;
	int key;
};


struct min_pq
{
	pq_node* pq_array;		//���ȶ���
	int nallocd;					//����Ԫ�صĸ���
	int n;								//������Ԫ�صĸ���
};




//k-d���Ļ�������
kd_node* kdtree_build(Keypoint* features, int n);

void kdtree_release(kd_node* kd_root);

int kdtree_bbf_knn(kd_node* kd_root, Keypoint* feat, int k, Keypoint*** nbrs, int max_nn_chks);

double descr_dist_sq(Keypoint* f1, Keypoint* f2);



//Ӱ���е�ͬ����
struct SamePoint
{
	double lx;
	double ly;
	double rx;
	double ry;

	SamePoint(){lx = 0; ly = 0; rx = 0; ry = 0;}
	SamePoint(double mlx, double mly, double mrx, double mry)
	{lx = mlx; ly = mly; rx = mrx; ry = mry;}
};


class Sift{
public:
	Sift()
	{
		CoInitialize(NULL);
		hthread = NULL;
		m_pImage = NULL;
		m_bFinish = false;
		::CoCreateInstance(CLSID_ImageDriver, NULL, CLSCTX_ALL, IID_IImage, (void**)&m_pImage);
	}
	~Sift()
	{
		m_pImage->Release();
		if (hthread != NULL)
		{
			delete []hthread;
			hthread = NULL;
		}
		::CoUninitialize();
	}
	void fetchFeatures(const char* szPathName);
	Keypoint* getFeatures();

public:
	std::list<Block> m_listBlock;
	std::list<Keypoint> m_listKeyPoint;
	bool m_bFinish;
	HANDLE hmutex;
	HANDLE hmutex2;

private:
	IImage* m_pImage;
	HANDLE* hthread;
};


#endif
