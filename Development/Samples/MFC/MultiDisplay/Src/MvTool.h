#ifndef __MVTOOL_H__
#define __MVTOOL_H__

#include "IMVAPI/IMVApi.h"

class CMvTool
{
public:
	/// 缺省构造函数
	CMvTool();

	~CMvTool();

	/// 得到从系统启动到现在的毫秒数
	static uint64_t getCurTimeMilliSecond();

	/// 得到从系统启动到现在的微秒数
	static uint64_t getCurTimeMicroSecond();

	/// 进入临界区。
	/// \return 操作是否成功
	bool enter();

	/// 尝试进入临界区，已经被占用直接返回，不等待。
	/// \return进入临界区是否成功
	bool tryEnter();

	/// 离开临界区。
	/// \return 操作是否成功
	bool leave();

private:
	HANDLE m_mutexHandle;
};

#endif // __MVTOOL_H__
