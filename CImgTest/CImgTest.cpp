
#include "CImg.h"

#define LOGD  printf
#define SAVE_FORMAT_BMP 0
#define SAVE_FORMAT_JPEG 1
#define SAVE_FORMAT_PNG  2

using namespace cimg_library_suffixed;
int CopyMe(int agrc, char** agrv)
{
	cimg::exception_mode(0);                                    // Enable quiet exception mode.
	try 
	{
		//...                                                       // Here, do what you want to stress CImg.
	}
	catch (CImgException &e)                                 // You succeeded: something went wrong!
	{
		std::fprintf(stderr, "CImg Library Error: %s", e.what());   // Display your custom error message.
		//...                                                       // Do what you want now to save the ship!
	}
	return 0;
}

int main(int agrc, char** agrv)
{
	char* file_source_path = "./picture.bmp";
	char* file_result_path = "./picture_save.bmp";
	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;
	float angle = 0.0; // 只做旋转测试
	float resizeScale = 0.5;  // 不等于1进行缩放处理
	int format = SAVE_FORMAT_BMP;
	int quality = 100;  // jpg 的保存质量
	int exifDegrees = 0;
	int exifTranslation = 1; // 镜像设置

	cimg::exception_mode(0);
	try
	{
		CImg<unsigned char> img(file_source_path);
		width = img.width();
		height = img.height();

		const int
			x0 = left, y0 = top,
			x1 = left + width - 1, y1 = top + height - 1;


		LOGD("left %d\ntop: %d\n", left, top);
		LOGD("width %d\nheight: %d\n", width, height);
		LOGD("angle %f\nresizeScale: %f\n", angle, resizeScale);
		LOGD("image size pre: %d x %d\n", img.width(), img.height());
		LOGD("exifDegrees: %d \nexifTranslation: %d\n", exifDegrees, exifTranslation);

		// Handle exif. However it is slow, maybe calculate warp field according to exif rotation/translation.
		if (exifDegrees != 0) {
			img.rotate(exifDegrees);
		}
		if (exifTranslation != 1) {
			img.mirror("x");
		}

		const int
			size_x = img.width() * resizeScale, size_y = img.height() * resizeScale,
			size_z = -100, size_c = -100, interpolation_type = 1;

		const unsigned int boundary_conditions = 0;
		const float
			centering_x = 0, centering_y = 0, centering_z = 0, centering_c = 0;
		if (resizeScale != 1) {
			img.resize(size_x, size_y, size_z, size_c, interpolation_type, boundary_conditions, centering_x, centering_y, centering_z, centering_c);
		}

		// Create warp field.
		CImg<float> warp(cimg::abs(x1 - x0 + 1), cimg::abs(y1 - y0 + 1), 1, 2);

		const float
			rad = angle * cimg::PI / 180,
			ca = std::cos(rad), sa = std::sin(rad),
			ux = cimg::abs(img.width() * ca), uy = cimg::abs(img.width() * sa),
			vx = cimg::abs(img.height() * sa), vy = cimg::abs(img.height() * ca),
			w2 = 0.5f * img.width(), h2 = 0.5f * img.height(),
			dw2 = 0.5f * (ux + vx), dh2 = 0.5f * (uy + vy);

		cimg_forXY(warp, x, y) {
			const float
				u = x + x0 - dw2, v = y + y0 - dh2;

			warp(x, y, 0) = w2 + u*ca + v*sa;
			warp(x, y, 1) = h2 - u*sa + v*ca;
		}

		img = img.get_warp(warp, 0, 1, 2);

		if (format == SAVE_FORMAT_JPEG) {
			img.save_jpeg(file_result_path, quality);
		}
		else if (format == SAVE_FORMAT_PNG) {
			img.save_png(file_result_path, 0);
		}
		else {
			img.save(file_result_path);
		}
		return 0;
	}
	catch (CImgException &e)
	{
		std::fprintf(stderr, "CImg Library Error: %s", e.what());
	}
	return 0;
}
