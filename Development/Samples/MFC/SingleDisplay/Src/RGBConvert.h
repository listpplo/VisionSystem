#ifndef __RGB_CONVERT_H__
#define __RGB_CONVERT_H__

#include "IMVAPI/IMVApi.h"

static IMV_EPixelType gFormatTransferTbl[] =
{
	// Mono Format
	gvspPixelMono1p,
	gvspPixelMono8,
	gvspPixelMono10,
	gvspPixelMono10Packed,
	gvspPixelMono12,
	gvspPixelMono12Packed,

	// Bayer Format		
	gvspPixelBayRG8,
	gvspPixelBayGB8,
	gvspPixelBayBG8,
	gvspPixelBayRG10,
	gvspPixelBayGB10,
	gvspPixelBayBG10,
	gvspPixelBayRG12,
	gvspPixelBayGB12,
	gvspPixelBayBG12,
	gvspPixelBayRG10Packed,
	gvspPixelBayGB10Packed,
	gvspPixelBayBG10Packed,
	gvspPixelBayRG12Packed,
	gvspPixelBayGB12Packed,
	gvspPixelBayBG12Packed,
	gvspPixelBayRG16,
	gvspPixelBayGB16,
	gvspPixelBayBG16,
	gvspPixelBayRG10p,
	gvspPixelBayRG12p,

	gvspPixelMono1c,

	// RGB Format
	gvspPixelRGB8,
	gvspPixelBGR8,

	// YVR Format
	gvspPixelYUV411_8_UYYVYY,
	gvspPixelYUV422_8_UYVY,
	gvspPixelYUV422_8,
	gvspPixelYUV8_UYV,
};
#define gFormatTransferTblLen	sizeof(gFormatTransferTbl)/sizeof(gFormatTransferTbl[0])

static int findMatchCode(IMV_EPixelType code)
{
	for (int i = 0; i < gFormatTransferTblLen; ++i)
	{
		if (gFormatTransferTbl[i] == code)
		{
			return i;
		}
	}
	return -1;
}

class FrameBuffer
{
private:
	unsigned char* Buffer_;

	unsigned int BufferSize_;

	unsigned int Width_;

	unsigned int Height_;

	unsigned int PaddingX_;

	unsigned int PaddingY_;

	unsigned int DataSize_;

	IMV_EPixelType PixelFormat_;

	uint64_t TimeStamp_;

	uint64_t BlockId_;

public:
	FrameBuffer(IMV_HANDLE devHandle, IMV_Frame frame)
	{
		if (frame.frameInfo.size < 0)
		{
			return;
		}

		int idx = findMatchCode((frame.frameInfo.pixelFormat));
		if (idx < 0)
		{
			return;
		}

		if (frame.frameInfo.pixelFormat == gvspPixelMono8)
		{
			BufferSize_ = frame.frameInfo.size;
		}
		else
		{
			BufferSize_ = frame.frameInfo.width * frame.frameInfo.height * 3;
		}

		Buffer_ = new(std::nothrow) unsigned char[BufferSize_];
		if (!Buffer_)
		{
			return;
		}

		Width_ = frame.frameInfo.width;
		Height_ = frame.frameInfo.height;
		PaddingX_ = frame.frameInfo.paddingX;
		PaddingY_ = frame.frameInfo.paddingY;
		DataSize_ = frame.frameInfo.size;
		PixelFormat_ = frame.frameInfo.pixelFormat;
		BlockId_ = frame.frameInfo.blockId;

		// Mono8无需转换直接源数据显示
		// Mono8 does not need to convert direct source data display
		if (frame.frameInfo.pixelFormat == gvspPixelMono8)
		{
			memcpy(Buffer_, frame.pData, DataSize_);
		}
		else
		{
			IMV_PixelConvertParam stPixelConvertParam;
			stPixelConvertParam.nWidth = Width_;
			stPixelConvertParam.nHeight = Height_;
			stPixelConvertParam.ePixelFormat = PixelFormat_;
			stPixelConvertParam.pSrcData = frame.pData;
			stPixelConvertParam.nSrcDataLen = DataSize_;
			stPixelConvertParam.nPaddingX = PaddingX_;
			stPixelConvertParam.nPaddingY = PaddingY_;
			stPixelConvertParam.eBayerDemosaic = demosaicNearestNeighbor;
			stPixelConvertParam.eDstPixelFormat = gvspPixelBGR8;
			stPixelConvertParam.pDstBuf = Buffer_;
			stPixelConvertParam.nDstBufSize = BufferSize_;

			int ret = IMV_PixelConvert(devHandle, &stPixelConvertParam);
			if (IMV_OK != ret)
			{
				if (Buffer_ != NULL)
				{
					delete[] Buffer_;
					Buffer_ = NULL;
				}
				TRACE("IMV_PixelConvert is failed.\n");
			}
		}
	}

	~FrameBuffer()
	{
		if(Buffer_ != NULL)
		{
			delete[] Buffer_;
			Buffer_ = NULL;
		}
	}

	bool Valid()
	{
		if( NULL != Buffer_ )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	unsigned int Width()
	{
		return Width_;
	}

	unsigned int Height()
	{
		return Height_;
	}

	unsigned int PaddingX()
	{
		return PaddingX_;
	}

	unsigned int PaddingY()
	{
		return PaddingY_;
	}

	unsigned int DataSize()
	{
		return DataSize_;
	}

	IMV_EPixelType PixelFormat()
	{
		return PixelFormat_;
	}

	uint64_t TimeStamp()
	{
		return TimeStamp_;
	}

	uint64_t BlockId()
	{
		return BlockId_;
	}

	unsigned char* bufPtr()
	{
		return Buffer_;
	}

	unsigned int BufferSize()
	{
		return BufferSize_;
	}

	void setWidth(unsigned int iWidth)
	{
		Width_ = iWidth;
	}

	void setPaddingX(unsigned int iPaddingX)
	{
		PaddingX_ = iPaddingX;
	}

	void setPaddingY(unsigned int iPaddingX)
	{
		PaddingY_ = iPaddingX;
	}

	void setHeight(unsigned int iHeight)
	{
		Height_ = iHeight;
	}

	void setDataSize(unsigned int dataSize)
	{
		DataSize_ = dataSize;
	}

	void setPixelFormat(IMV_EPixelType pixelFormat)
	{
		PixelFormat_ = pixelFormat;
	}

	void setTimeStamp(uint64_t timeStamp)
	{
		TimeStamp_ = timeStamp;
	}

	void setBufferSize(unsigned int BufferSize)
	{
		BufferSize_ = BufferSize;
	}
};

#endif //__RGB_CONVERT_H__
