#pragma once


// 时间边界
enum AAUTimeBoundary {
	AAUTimeBoundaryNoLimit,	// 成年人没有限制
	AAUTimeBoundaryTimeLimit,	// 时间限制
	AAUTimeBoundaryCurfew,		// 宵禁，目前政策都是因为宵禁
};

class AAUServerDelegate {
public:
	
	
	virtual ~AAUServerDelegate() = default;
	// 用户被踢出，如果Boundary返回的是NoLimit，那么是被服务主动踢出
	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) = 0;
};