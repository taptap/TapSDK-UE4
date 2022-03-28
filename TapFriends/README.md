# TapFriends 好友功能

> TapFriends 分为好友模式和关注模式两种

## 共用模块

### 状态：上线/下线
```c++
	// 上线
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFrineds OnLine"))
	static void Online();

	// 下线
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFriends Offline"))
	static void Offline();
```

### 监听好友通知变化

```c++
   // 注册通知
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "register friends status changed"))
	static void RegisterFriendsStatusChangedListener();

	// 注销通知
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "remove friends status changed"))
	static void RemoveFriendsStatusChangedListener();
```

### 好友通知变化回调

```c++
    /** add Friend Request */
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestComming;

	// accept friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestAccepted;

	// decline friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestDeclined;

	// friend online
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOnline, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOnline OnFriendOnline;

	// friend offline
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOffline, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOffline OnFriendOffline;


	// friend rich presence changed
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendRichPresenceChanged, const FString&, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendRichPresenceChanged OnFriendRichPresenceChanged;

	// friend on connected
	DECLARE_MULTICAST_DELEGATE(FFriendConnectedCallback)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendConnectedCallback OnFriendConnected;

	// friend disconnected
	DECLARE_MULTICAST_DELEGATE(FFriendDisconnected)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendConnectedCallback OnFriendDisconnected;

	// friend disconnected
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOnConnectedError, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOnConnectedError OnConnectedError;
```

### 查询好友

```c++
	// 通过nick name查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friends By NickName"))
	static void QueryFiendsByName(FString nickName);
	// 回调
	FTapFriendsModule::OnQueryFriendsByNickName;

	// 通过short code查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friend By ShortCode"))
	static void QueryFriendByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnQueryFriendByShortCode;

	// 通过object id查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friend By ObjectId"))
	static void QueryFriendByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnQueryFriendByObjectId;
```

### 设置/清除富文本

```c++
	// 设置富信息
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFriends Save RichPresence"))
	static void SaveRichPresence(TMap<FString, FString> richPresence);

	// 清除富信息
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFirends clear RichPresence"))
	static void ClearRicePresence(TArray<FString> richPresenceKeys);
```

### 设置分享链接

```c++
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta =(DisplayName = "TapFriends set share link"))
	static void SetShareLink(FString link);
```

### 生成链接

在调用方法前，要先设置`SetShareLink`

```c++
	UFUNCTION(BlueprintCallable, Category="TapFriends Core",
	meta=(DisplayName ="TapFriends GenerateFriendInvitationLink"))
	static void GenerateFriendInvitationLink(FString RoleName, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnGenerateFriendInvitation;

	UFUNCTION(BlueprintCallable, Category="TapFriends Core",
		meta=(DisplayName ="TapFriends fast GenerateFriendInvitationLink"))
	static void FastGenerateFriendInvitationLink();
	// 回调
	FTapFriendsModule::OnGenerateFriendInvitation;

	UFUNCTION(BlueprintCallable, Category="TapFriends Core", meta=(DisplayName ="TapFriends ParseFriendInvitationLink"))
	static void ParseFriendInvitationLink(FString Uri);
	// 回调
	FTapFriendsModule::OnParseFriendLinkInfo;
```

## 好友模块

### 添加/删除好友

```c++
	// 通过 short code 添加好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends add Friends by shortCode"))
	static void AddFriendByShortCode(FString shortCode, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnAddFriendByShortCode;

	// 通过 object id 添加好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends add Friends by objectId"))
	static void AddFriendByObjectId(FString objectId, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnAddFriendByObjectId;

	// 通过 object id 删除好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends delete Friends by objectId"))
	static void DeleteFriendByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnFriendDeleteByObjectId;
```

### 处理好友请求

```c++
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends query Friend request"))
	static void QueryFriendRequestList(int64 status, int64 from, int64 limit);
	// 回调
	FTapFriendsModule::OnFriendQueryRequest;

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends query Friend request state list"))
	static void QueryFriendRequestWithFriendStateList(int64 status, int64 from, int64 limit);
	// 回调
	FTapFriendsModule::OnQueryFriendRequestWithFriendStateList;

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends accept Friend request"))
	static void AcceptFriendRequest(FString requestObjectId, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnAcceptFriendRequest;

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFriends decline Friend request"))
	static void DeclineFriendRequest(FString requestObjectId);
	// 回调
	FTapFriendsModule::OnFriendDeclineRequest;

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFriends delete Friend request"))
	static void DeleteFriendRequest(FString requestObjectId);
	// 回调
	FTapFriendsModule::OnFriendDeleteRequest;
```

### 查询好友列表

```c++
	UFUNCTION(BlueprintCallable, Category="TapFriends pattern", meta=(DisplayName= "TapFriends query Friends list"))
	static void QueryFriendsList(int64 from, int64 limit);
	// 回调
	FTapFriendsModule::OnQueryFriendsList;
	
	// 查询三方好友列表
		UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query friend list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);
	// 回调
	FTapFriendsModule::OnQueryThirdPartyFriendList;
```

### 检查好友关系

```c++
	UFUNCTION(BlueprintCallable, Category="TapFrineds pattern", meta=(DisplayName="TapFriends check friendship"))
	static void CheckFriendShip(FString objectId);
	// 回调
	FTapFriendsModule::OnCheckFriendShip;
```


### 处理好友链接

```c++
	UFUNCTION(BlueprintCallable, Category="TapFriends Pattern",
		meta=(DisplayName ="TapFriends handFriendInvitationLink"))
	static void HandFriendInvitationLink(FString Uri);
	// 回调
	FTapFriendsModule::OnHandleFriendInvitationLink;
```

## 关注模式

### 关注/取消关注

```c++
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows follow user by code"))
	static void FollowByShortCode(FString shortCode, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnFollowByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows follow user by objectId"))
	static void FollowByObjectId(FString objectId, TMap<FString, FString> attrs);
	// 回调
	FTapFriendsModule::OnFollowByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows unfollow by objectId"))
	static void UnFollowByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnUnFollowByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows unfollow by shortCode"))
	static void UnFollowByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnUnFollowByShortCode;
```

### 拉黑/取消拉黑

```c++
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows block by objectId"))
	static void FollowBlockByObjectId(FString ObjectId);
	// 回调
	FTapFriendsModule::OnBlockByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows unblock by objectId"))
	static void FollowUnBlockByObjectId(FString ObjectId);
	// 回调
	FTapFriendsModule::OnUnBlockByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows block by shortCode"))
	static void FollowBlockByShortCode(FString ShortCode);
	// 回调
	FTapFriendsModule::OnBlockByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows unblock by shortCode"))
	static void FollowUnBlockByShortCode(FString ShortCode);
	// 回调
	FTapFriendsModule::OnUnBlockByShortCode;
```


### 查询关注关系列表

```c++
	// 查询互关列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query mutual list"))
	static void FollowQueryMutualList(int64 pageSize, FString cursor, FString sortCondition);
	// 回调
	FTapFriendsModule::OnQueryFollowMutalList;

	// 查询被关注列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query followee list"))
	static void FollowQueryFolloweeList(int64 pageSize, FString cursor, FString sortCondition);
	// 回调
	FTapFriendsModule::OnQueryFolloweeList;

	// 查询关注者列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query follower list"))
	static void FollowQueryFollowerList(int64 pageSize, FString cursor, FString sortCondition);
	// 回调
	FTapFriendsModule::OnQueryFollowerList;

	// 查询黑名单列表
	UFUNCTION(BlueprintCallable, Category="TapFollows Pattern", meta=(DisplayName="TapFollows query block list"))
	static void FollowQueryBlockList(int64 PageSize, FString cursor, FString sortCondition);
	// 回调
	FTapFriendsModule::OnFollowQueryBlockList;
```

### 关注/取关Tap User

```c++
   UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows follow TapUser by code"))
	static void FollowTapUserByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnFollowTapUserByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unfollow Tapuser by code"))
	static void UnFollowTapUserByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnUnFollowTapUserByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows follow TapUser by objectId"))
	static void FollowTapUserByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnFollowTapUserByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unfollow Tapuser by objectId"))
	static void UnFollowTapUserByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnUnFollowTapUserByObjectId;
```

### 拉黑/取消拉黑Tap User

```c++
	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
	meta=(DisplayName = "TapFollows block TapUser by code"))
	static void BlockTapUserByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnBlockTapUserByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unblock TapUser by code"))
	static void UnBlockTapUserByShortCode(FString shortCode);
	// 回调
	FTapFriendsModule::OnUnBlockTapUserByShortCode;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows block TapUser by objectId"))
	static void BlockTapUserByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnBlockTapUserByObjectId;

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unblock TapUser by objectId"))
	static void UnBlockTapUserByObjectId(FString objectId);
	// 回调
	FTapFriendsModule::OnUnBlockTapUserByObjectId;
```

### 查询三方关系列表

```c++
		UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query mutal list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendMutalList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);
	// 回调
	FTapFriendsModule::OnQueryThirdPartyFriendMutalList;

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query followee list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendFolloweeList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);
	// 回调
	FTapFriendsModule::OnQueryThirdPartyFriendFolloweeList;

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query follower list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendFollowerList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);
	// 回调
	FTapFriendsModule::OnQueryThirdPartyFriendFollowerList;

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query block list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendBlockList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);
	// 回调
	FTapFriendsModule::OnQueryThirdPartyFriendBlockList;
```

### 处理关注链接

```c++
	UFUNCTION(BlueprintCallable, Category="TapFollows Pattern",
		meta=(DisplayName ="TapFollows handFriendInvitationLink"))
	static void HandFollowInvitationLink(FString Uri);
	// 回调
	FTapFriendsModule::OnHandleFollowInvitationLink;
```


