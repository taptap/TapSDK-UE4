#pragma once
#include "TUType.h"

class ILoginLang {
public:
	virtual ~ILoginLang() = default;

	virtual FString TitleUse() = 0;

	virtual FString TitleLogin() = 0;

	virtual FString QrTitleLogin() = 0;

	virtual FString QrRefresh() = 0;

	virtual FString QrNoticeUse() = 0;

	virtual FString QrNoticeClient() = 0;

	virtual FString QrNoticeScanToLogin() = 0;

	virtual FString WebLogin() = 0;

	virtual FString WebNotice() = 0;

	virtual FString WebButtonJumpToWeb() = 0;

	virtual FString QrNoticeCancel() = 0;

	virtual FString QrNoticeCancel2() = 0;

	virtual FString QrnNoticeSuccess() = 0;

	virtual FString QrnNoticeSuccess2() = 0;

	virtual FString WebNoticeLogin() = 0;

	virtual FString WebNoticeFail() = 0;

	virtual FString WebNoticeFail2() = 0;
};

class TULoginLanguage {
public:
	
	static TSharedPtr<ILoginLang> GetCurrentLang();

	// static void SetLangType(Type Type);

private:

	static TSharedPtr<ILoginLang> CurrentLang;
	static ELanguageType LanguageType;
};

class LoginLangCN : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("使用");
	}

	virtual FString TitleLogin() override {
		return TEXT("账号登录");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("安全扫码登录");
	}

	virtual FString QrRefresh() override {
		return TEXT("点击刷新");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("请使用");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("客户端");
	}

	virtual FString QrNoticeScanToLogin() override {
		return TEXT("请使用 <TapBoldStyle>TapTap</> <TapStyle>客户端</>\n扫描二维码登录");
	}

	virtual FString WebLogin() override {
		return TEXT("使用网页浏览器完成授权");
	}

	virtual FString WebNotice() override {
		return TEXT("点击下方按钮前往网页浏览器，\n授权 TapTap 账号，完成后将自动返回游戏。");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("跳转至 TapTap");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("您已取消此次登录");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("请重新扫码");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("扫码成功");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("请在手机上确认");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("正在登录中，请稍后");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("登录失败");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("请重新点击打开网页进行授权");
	}
};

class LoginLangIO : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("Log In with");
	}

	virtual FString TitleLogin() override {
		return TEXT("");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("Via QR Code");
	}

	virtual FString QrRefresh() override {
		return TEXT("Refresh");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("Use");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("App");
	}
	
	virtual FString QrNoticeScanToLogin() override {
		return TEXT("Scan the QR code \n with <TapBoldStyle>TapTap</> <TapStyle>app</>");
	}

	virtual FString WebLogin() override {
		return TEXT("Via Web Browser");
	}

	virtual FString WebNotice() override {
		return TEXT("Allow permission to log in with TapTap,\nYou'll be redirected back after login.");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("Go to TapTap");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("Failed to log in");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("Please try again");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("Success");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("Please confirm login on your phone");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("Logging in");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("Failed to log in");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("Please try again");
	}
};

class LoginLangID : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("Log In dengan ");
	}

	virtual FString TitleLogin() override {
		return TEXT("");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("Melalui Kode QR");
	}

	virtual FString QrRefresh() override {
		return TEXT("Refresh");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("Pindai kode QR ");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("");
	}
	
	virtual FString QrNoticeScanToLogin() override {
		return TEXT("Pindai kode QR \n dengan apliaksi <TapBoldStyle>TapTap</>");
	}

	virtual FString WebLogin() override {
		return TEXT("Melalui Browser Web");
	}

	virtual FString WebNotice() override {
		return TEXT("Berikan izin untuk log in dengan TapTap. \nKamu akan diarahkan kembali setelah log in.");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("Buka TapTap");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("Gagal log in");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("Silakan coba lagi.");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("Berhasil");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("Silakan konfirmasi log in di ponselmu.");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("Sedang log in...");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("Gagal log in");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("Silakan coba lagi.");
	}
};

class LoginLangJA : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("");
	}

	virtual FString TitleLogin() override {
		return TEXT("でログイン");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("QRコードを使う");
	}

	virtual FString QrRefresh() override {
		return TEXT("更新");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("アプリで");
	}
	
	virtual FString QrNoticeScanToLogin() override {
		return TEXT("<TapBoldStyle>TapTap</>アプリで\nQRコードをスキャン");
	}

	virtual FString WebLogin() override {
		return TEXT("ウェブブラウザを使う");
	}

	virtual FString WebNotice() override {
		return TEXT("TapTapへのログインを許可します。\nログイン後にリダイレクトされます。");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("TapTapを開く");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("ログインに失敗しました");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("もう一度お試しください。");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("成功");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("お使いの携帯電話でログインを確認してください。");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("ログイン中…");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("ログインに失敗しました");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("もう一度お試しください。");
	}
};

class LoginLangKO : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("");
	}

	virtual FString TitleLogin() override {
		return TEXT("으로 로그인");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("QR 코드 사용");
	}

	virtual FString QrRefresh() override {
		return TEXT("새로 고치기");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("앱으로");
	}

	virtual FString QrNoticeScanToLogin() override {
		return TEXT("<TapBoldStyle>Taptap</> 앱으로\nQR 코드를 스캔하세요");
	}

	virtual FString WebLogin() override {
		return TEXT("웹 브라우저 사용");
	}

	virtual FString WebNotice() override {
		return TEXT("Taptap으로 로그인하는 권한을 허락하세요.\n로그인 후에 원래 화면으로 돌아갑니다.");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("Taptap으로 이동");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("로그인 실패");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("다시 시도하세요.");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("성공");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("스마트폰에서 로그인을 확인하세요.");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("로그인 중...");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("로그인 실패");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("다시 시도하세요.");
	}
};

class LoginLangTH : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("ล็อกอินด้วย ");
	}

	virtual FString TitleLogin() override {
		return TEXT("");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("ผ่าน QR โค้ด");
	}

	virtual FString QrRefresh() override {
		return TEXT("รีเฟรช");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("");
	}
	
	virtual FString QrNoticeScanToLogin() override {
		return TEXT("สแกน QR โค้ด \n ด้วยแอป <TapBoldStyle>TapTap</>");
	}

	virtual FString WebLogin() override {
		return TEXT("ผ่านเว็บเบราว์เซอร์");
	}

	virtual FString WebNotice() override {
		return TEXT("อนุญาตให้ล็อกอินด้วย TapTap \nคุณจะถูกเปลี่ยนเส้นทางกลับหลังจากล็อกอิน");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("ไปที่ TapTap");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("ไม่สามารถล็อกอินได้");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("โปรดลองอีกครั้ง");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("สำเร็จ");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("โปรดยืนยันการล็อกอินบนโทรศัพท์ของคุณ");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("กำลังล็อกอิน...");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("ไม่สามารถล็อกอินได้");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("โปรดลองอีกครั้ง");
	}
};

class LoginLangZHTW : public ILoginLang {
public:
	virtual FString TitleUse() override {
		return TEXT("使用");
	}

	virtual FString TitleLogin() override {
		return TEXT("登入");
	}

	virtual FString QrTitleLogin() override {
		return TEXT("透過 QR 碼");
	}

	virtual FString QrRefresh() override {
		return TEXT("重新整理");
	}

	virtual FString QrNoticeUse() override {
		return TEXT("使用");
	}

	virtual FString QrNoticeClient() override {
		return TEXT("應用程式");
	}
	
	virtual FString QrNoticeScanToLogin() override {
		return TEXT("使用 <TapBoldStyle>TapTap</> <TapStyle>應用程式</>\n掃描 QR 碼");
	}

	virtual FString WebLogin() override {
		return TEXT("透過網路瀏覽器");
	}

	virtual FString WebNotice() override {
		return TEXT("請允許使用 TapTap 登入。\n登入後系統會將你重新導回此頁面。");
	}

	virtual FString WebButtonJumpToWeb() override {
		return TEXT("前往 TapTap");
	}

	virtual FString QrNoticeCancel() override {
		return TEXT("登入失敗");
	}

	virtual FString QrNoticeCancel2() override {
		return TEXT("請再試一次。");
	}

	virtual FString QrnNoticeSuccess() override {
		return TEXT("成功");
	}

	virtual FString QrnNoticeSuccess2() override {
		return TEXT("請在您的手機上確認登入。");
	}

	virtual FString WebNoticeLogin() override {
		return TEXT("正在登入……");
	}

	virtual FString WebNoticeFail() override {
		return TEXT("登入失敗");
	}

	virtual FString WebNoticeFail2() override {
		return TEXT("請再試一次。");
	}
};
