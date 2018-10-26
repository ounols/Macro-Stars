#pragma once
#include "MacroDef.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#define RESMGR ResMgr::getInstance()

class ResMgr {
public:
	DECLARE_SINGLETONE(ResMgr);
	~ResMgr();

	void RegisterImage(const char* path, std::string name, int colorMode = CV_LOAD_IMAGE_COLOR);
	void RegisterImage(IplImage* img);
	void RegisterImage(IplImage* img, std::string name);

	IplImage* GetImage(std::string name) {
		return m_imagesMap[name];
	}

	bool TrainKNN(const char* classifications_path, const char* images_path);
	std::string Image2String(IplImage* src, int height_range = 0, int string_width = 0) const;
	std::vector<CvPoint> FindImages(IplImage* _src, std::string name, float match = 0.99f, int count = 99, bool disableScale = false, CvRect cropSrc = cvRect(0, 0, 0, 0));

	bool CheckRGB(IplImage* src, CvPoint target, CvScalar bgr, int match = 1) const;
	bool CheckRGB(IplImage* src, CvPoint target, int r, int g, int b, int match = 1) const;
	bool CheckRGB(IplImage* src, int x, int y, CvScalar bgr, int match = 1) const;
	bool CheckRGB(IplImage* src, int x, int y, int r, int g, int b, int match = 1) const;

	static IplImage* __ReAlloc(IplImage* src, IplImage* dst);
	static IplImage* __ResizeImage(IplImage* img, CvSize size);
	static IplImage* __CutImage(IplImage* img, CvRect cut_rect);
	static void __rotateImage(const IplImage* src, IplImage* dst, double degree);
	static IplImage* __MaskImage(IplImage* src, CvScalar bgr, int match = 1, IplImage* dst = nullptr);

	static cv::Mat gammaCorrection(const cv::Mat& img, const double gamma_);
	static cv::Mat basicLinearTransform(const cv::Mat &img, const double alpha_, const int beta_);


private:
	std::vector<IplImage*> m_imagesVector;
	std::map<std::string, IplImage*> m_imagesMap;

	cv::Ptr<cv::ml::KNearest>  m_kNearest;
};

#define REALLOC(src, dst) src = ResMgr::__ReAlloc(src, dst); dst = nullptr
#define RESIZE_IMAGE(src, size) src = ResMgr::__ResizeImage(src, size)
#define CUT_IMAGE(src, rect) src = ResMgr::__CutImage(src, rect)

#define IMG_WIDTH 1920
#define IMG_HEIGHT 1080
#define CONVERT_X(x, img) ((float)img->width / IMG_WIDTH)*x
#define CONVERT_Y(y, img) ((float)img->height / IMG_HEIGHT)*y