#include "ResMgr.h"
#include "GameClientMgr.h"

#include <math.h>

IMPLEMENT_SINGLETON(ResMgr);

class ContourWithData {
public:
	// member variables ///////////////////////////////////////////////////////////////////////////
	std::vector<cv::Point> ptContour;           // contour
	cv::Rect boundingRect;                      // bounding rect for contour
	float fltArea;                              // area of contour

												///////////////////////////////////////////////////////////////////////////////////////////////
	bool checkIfContourIsValid() {                              // obviously in a production grade program
		if (fltArea < 100) return false;           // we would have a much more robust function for 
		return true;                                            // identifying if a contour is valid !!
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
	}

};

ResMgr::ResMgr() {}


ResMgr::~ResMgr() {
	
	for(auto img : m_imagesVector) {
		cvReleaseImage(&img);
	}

	m_imagesVector.clear();

	for (auto img : m_imagesMap) {
		cvReleaseImage(&img.second);
	}

	m_imagesMap.clear();

}


void ResMgr::RegisterImage(const char* path, std::string name, int colorMode) {
	m_imagesMap[name] = cvLoadImage(path, colorMode);
}


void ResMgr::RegisterImage(IplImage* img) {
	m_imagesVector.push_back(img);
}


void ResMgr::RegisterImage(IplImage* img, std::string name) {
	m_imagesMap[name] = img;
}


bool ResMgr::TrainKNN(const char* classifications_path, const char* images_path) {

	cv::Mat matClassificationInts;      // we will read the classification numbers into this variable as though it is a vector

	cv::FileStorage fsClassifications(classifications_path, cv::FileStorage::READ);        // open the classifications file

	if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
		std::cout << "error, unable to open training classifications file, exiting program\n\n";    // show error message
		return false;                                                                                  // and exit program
	}

	fsClassifications["classifications"] >> matClassificationInts;      // read classifications section into Mat classifications variable
	fsClassifications.release();                                        // close the classifications file

																		// read in training images ////////////////////////////////////////////////////////////

	cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector

	cv::FileStorage fsTrainingImages(images_path, cv::FileStorage::READ);          // open the training images file

	if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
		std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
		return false;                                                                              // and exit program
	}

	fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
	fsTrainingImages.release();                                                 // close the traning images file

																				// train //////////////////////////////////////////////////////////////////////////////

	cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());            // instantiate the KNN object
	m_kNearest = cv::Ptr<cv::ml::KNearest>(cv::ml::KNearest::create());


																				// finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
																				// even though in reality they are multiple images / numbers
	return m_kNearest->train(matTrainingImagesAsFlattenedFloats, 0, matClassificationInts);

}


std::string ResMgr::Image2String(IplImage* src, int height_range, int string_width) const {

	cv::Mat matTestingNumbers = cv::cvarrToMat(src);

	if (matTestingNumbers.empty()) {                                // if unable to open image
		std::cout << "error: image not read from file\n\n";         // show error message on command line
		return(0);                                                  // and exit program
	}

	std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
	std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly

	cv::Mat matGrayscale;           //
	cv::Mat matThresh;              //
	cv::Mat matThreshCopy;          //

	//cv::cvtColor(matTestingNumbers, matGrayscale, CV_BGR2GRAY);         // convert to grayscale

	matTestingNumbers = gammaCorrection(matTestingNumbers, 100);					//remove noise

								   // filter image from grayscale to black and white
	cv::adaptiveThreshold(matTestingNumbers,                           // input image
		matThresh,                            // output image
		255,                                  // make pixels that pass the threshold full white
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
		cv::THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
		5,                                   // size of a pixel neighborhood used to calculate threshold value
		1);                                   // constant subtracted from the mean or weighted mean

	matThreshCopy = matThresh.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

	std::vector<std::vector<cv::Point> > ptContours;        // declare a vector for the contours
	std::vector<cv::Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

	cv::findContours(matThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
		ptContours,                             // output contours
		v4iHierarchy,                           // output hierarchy
		cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
		cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

	//cv::imshow("이진", matThreshCopy);

	int avg_height = 0;

	for (int i = 0; i < ptContours.size(); i++) {               // for each contour
		ContourWithData contourWithData;                                                    // instantiate a contour with data object
		contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
		contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);         // get the bounding rect
		contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);               // calculate the contour area
		allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
		avg_height += contourWithData.boundingRect.height;
	}

	if(avg_height > 0) {
		avg_height /= ptContours.size();
	}

	for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
		if (allContoursWithData[i].checkIfContourIsValid()) {                   // check if valid
			validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
		}
	}
	// sort contours from left to right
	std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

	std::string strFinalString;         // declare final string, this will have the final number sequence by the end of the program

	for (int i = 0; i < validContoursWithData.size(); i++) {            // for each contour

																		// draw a green rect around the current char
		cv::rectangle(matTestingNumbers,                            // draw rectangle on original image
			validContoursWithData[i].boundingRect,        // rect to draw
			cv::Scalar(255, 255, 255),                        // green
			2);                                           // thickness

		cv::Rect boundingData = validContoursWithData[i].boundingRect;

		//글씨가 가로로 더 길다면 글씨가 아니다!
		if(boundingData.width > boundingData.height) {
			//높이가 오차범위 내라면 기회를 한번 더 줌
			if(abs(avg_height - boundingData.height) <= height_range) {
				int width_slice = (string_width <= 0) ? 2 : ((float)boundingData.width / string_width) + 0.5f;
				boundingData.width /= width_slice;

				validContoursWithData[i].boundingRect.x += boundingData.width;
				validContoursWithData[i].boundingRect.width -= boundingData.width;
				i--;

			}else
				continue;
		}

		cv::Mat matROI = matThresh(boundingData);          // get ROI image of bounding rect

		cv::Mat matROIResized;
		cv::resize(matROI, matROIResized, cv::Size(20, 30));     // resize image, this will be more consistent for recognition and storage

		cv::Mat matROIFloat;
		matROIResized.convertTo(matROIFloat, CV_32FC1);             // convert Mat to float, necessary for call to find_nearest

		cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

		cv::Mat matCurrentChar(0, 0, CV_32F);

		m_kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

		float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

		strFinalString = strFinalString + char(int(fltCurrentChar));        // append current char to full string
	}
	//cv::imshow("이진", matTestingNumbers);
	//cvWaitKey();
	std::cout  << "numbers read = " << strFinalString << std::endl;       // show the full string

	return strFinalString;

}


std::vector<CvPoint> ResMgr::FindImages(IplImage* _src, std::string name, float match, int count, bool disableScale, CvRect cropSrc) {

	if(_src == nullptr) {
		_src = GAME->GetScreenImage();
	}

	if (m_imagesMap[name] == nullptr) return std::vector<CvPoint>();

	IplImage* objectImg = (IplImage*)cvClone(m_imagesMap[name]);
	IplImage* src = (IplImage*)cvClone(_src);

	if(!disableScale) {
		CvSize size = cvSize(CONVERT_X(objectImg->width, src), CONVERT_Y(objectImg->height, src));
		RESIZE_IMAGE(objectImg, size);
	}

	if (cropSrc.height > 0 && cropSrc.width > 0) {
		CUT_IMAGE(src, cropSrc);
		//cvShowImage("sample", src);
	}

	//cvShowImage("이진", src);
	//cvWaitKey(1);

	//cvShowImage("sample", objectImg);
	//cvWaitKey();

	double min, max;
	CvPoint position;
	IplImage* c = cvCreateImage(cvSize(src->width - objectImg->width + 1, src->height - objectImg->height + 1), IPL_DEPTH_32F, 1);
	
	std::vector<CvPoint> points;

	for (int i = 0; i < count; i++) {
		

		cvMatchTemplate(src, objectImg, c, CV_TM_CCORR_NORMED);
		cvMinMaxLoc(c, &min, &max, nullptr, &position);
		if (max < match) break;
		std::cout << "min : " << min << ", max : " << max << ", x = " << position.x << ", y = " << position.y << std::endl;
		points.push_back(CvPoint(position));

		cvRectangle(src, position, cvPoint(position.x + objectImg->width, position.y + objectImg->height), CV_RGB(255, 0, 0), 4);

		//cvShowImage("sample", src);
		//cvWaitKey();
	}


	cvReleaseImage(&objectImg);
	cvReleaseImage(&src);
	cvReleaseImage(&c);

	return points;
}


bool ResMgr::CheckRGB(IplImage* src, CvPoint target, CvScalar bgr, int match) const {

	if(src == nullptr) {
		src = GAME->GetScreenImage();
	}

	CvScalar src_bgr = cvGet2D(src, target.y, target.x);

	//std::cout << "point(" << target.x << ", " << target.y << ") = RGB(" << src_bgr.val[2] << ", " << src_bgr.val[1] << ", " << src_bgr.val[0] << ")" << std::endl;

	if(std::abs(src_bgr.val[0] - bgr.val[0]) > match) {
		return false;
	}

	if (std::abs(src_bgr.val[1] - bgr.val[1]) > match) {
		return false;
	}

	if (std::abs(src_bgr.val[2] - bgr.val[2]) > match) {
		return false;
	}

	return true;

}



bool ResMgr::CheckRGB(IplImage* src, CvPoint target, int r, int g, int b, int match) const {

	return CheckRGB(src, target, cvScalar(b, g, r), match);

}


bool ResMgr::CheckRGB(IplImage* src, int x, int y, CvScalar bgr, int match) const {

	return CheckRGB(src, cvPoint(x, y), bgr, match);


}


bool ResMgr::CheckRGB(IplImage* src, int x, int y, int r, int g, int b, int match) const {

	return CheckRGB(src, cvPoint(x, y), cvScalar(b, g, r), match);


}


IplImage* ResMgr::__ReAlloc(IplImage* src, IplImage* dst) {

		if (src == nullptr) return dst;
		cvReleaseImage(&src);
		return dst;
}


IplImage* ResMgr::__ResizeImage(IplImage* img, CvSize size) {

	IplImage* resized = cvCreateImage(size, IPL_DEPTH_8U, 3);

	cvResize(img, resized, CV_INTER_LANCZOS4);
	cvReleaseImage(&img);

	return resized;

}


IplImage* ResMgr::__CutImage(IplImage* img, CvRect cut_rect) {

	cvSetImageROI(img, cut_rect);

	CvSize size = cvGetSize(img);
	IplImage* _img = cvCreateImage(size, IPL_DEPTH_8U, 3);

	cvCopy(img, _img);
	cvReleaseImage(&img);

	return _img;

}

void ResMgr::__rotateImage(const IplImage* src, IplImage* dst, double degree) {


	CvPoint2D32f centralPoint = cvPoint2D32f(dst->width / 2, dst->height / 2);            // 회전 기준점 설정(이미지의 중심점)
	CvMat* rotationMatrix = cvCreateMat(2, 3, CV_32FC1);                        // 회전 기준 행렬

																						   // Rotation 기준 행렬 연산 및 저장(90도에서 기울어진 각도를 빼야 본래이미지(필요시 수정))
	cv2DRotationMatrix(centralPoint, 90 - degree, 1, rotationMatrix);
	cv2DRotationMatrix(centralPoint, 90 - degree, 1, rotationMatrix);

	// Image Rotation
	cvWarpAffine(src, dst, rotationMatrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

	// Memory 해제
	cvReleaseMat(&rotationMatrix);
}


IplImage* ResMgr::__MaskImage(IplImage* src, CvScalar bgr, int match, IplImage* dst) {

	if (src == nullptr) return nullptr;

	if(dst == nullptr) {
		dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	}

	for(int x = 0; x < src->width; x++) {
		for(int y = 0; y < src->height; y++) {
			CvScalar src_bgr = cvGet2D(src, y, x);
			double dst_bgr = cvGetReal2D(dst, y, x);

			if (std::abs(src_bgr.val[0] - bgr.val[0]) > match) {
				cvSetReal2D(dst, y, x, dst_bgr);
				continue;
			}

			if (std::abs(src_bgr.val[1] - bgr.val[1]) > match) {
				cvSetReal2D(dst, y, x, dst_bgr);
				continue;
			}

			if (std::abs(src_bgr.val[2] - bgr.val[2]) > match) {
				cvSetReal2D(dst, y, x, dst_bgr);
				continue;
			}

			cvSetReal2D(dst, y, x, 255);


		}
	}

	return dst;

}


cv::Mat ResMgr::gammaCorrection(const cv::Mat& img, const double gamma_) {

	CV_Assert(gamma_ >= 0);
	//![changing-contrast-brightness-gamma-correction]
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

	cv::Mat res = img.clone();
	cv::LUT(img, lookUpTable, res);
	//![changing-contrast-brightness-gamma-correction]

	return res;
}


cv::Mat ResMgr::basicLinearTransform(const cv::Mat& img, const double alpha_, const int beta_) {

	cv::Mat res;
	img.convertTo(res, -1, alpha_, beta_);

	return res;

}
