#ifndef _FILE_H_
#define _FILE_H_
#include <iostream>
#include <Shlwapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
//#include"MyMacro.h"

using namespace std;
enum Extension
{
	Txt = 0,	//テキストファイル.
	Csv,		//Csvファイル.
	x,			//Xファイル.
	png,		//pngファイル.
};
struct FileSet
{
	string			sPath;			//ファイルパス.
	Extension		en_Extension;	//拡張子判定用.
	vector<string>	sDataAry;		//データの配列.
	int				iLineMax;		//配列の最大数.
};
class clsFile
{
public:
	clsFile();
	~clsFile();

	//最初に通して.
	void Init(const char* fileName);

	//ファイル書き込み.
	void Write(char* FilePath_A, char* FilePath);

	//ファイル書き込み.
	void Write(char* FilePath, int NewLine = NULL);

	//ファイル書き込み.
	//template< class T >
	void Write(char* FilePath, vector<float> DataAry);
	void Write(char* FilePath, vector<int> DataAry);
	void Write(char* FilePath, vector<char*> DataAry);

	//ファイル読込.
	void Read();

	//ファイルを開く(Txt,Csvのみ).
	inline void Open(HWND hWnd) {
		m_hWnd = hWnd;
		std::thread th1([this]{
			ShellExecute(m_hWnd, NULL, m_Data.sPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		});
	}

	//配列の最大数.
	inline int GetDataArrayNumMax() {
		return m_Data.iLineMax;
	}

	//拡張子がどれか.
	inline Extension GetExtension() {
		return m_Data.en_Extension;
	}

	//拡張子がどれか.
	inline string GetFilePath() {
		return m_Data.sPath;
	}

	//float型で返すよ.
	inline void GetDataArray(float& Data, int Array_Num)
	{
		stringstream is;
		float tmp;
		is.str(m_Data.sDataAry[Array_Num]);
		is >> tmp;
		Data = tmp;
		is.clear(stringstream::goodbit);
	}

	//int型で返すよ.
	inline int GetDataArray(int Array_Num)
	{
		stringstream is;
		int tmp;
		is.str(m_Data.sDataAry[Array_Num]);
		is >> tmp;
		return tmp;
		is.clear(stringstream::goodbit);
	}

	//int型で返すよ.
	template<class T>
	inline void GetDataArray(T* Data, int Array_Num)
	{
		stringstream is;
		T tmp;
		is.str(m_Data.sDataAry[Array_Num]);
		is >> tmp;
		*Data = tmp;
		is.clear(stringstream::goodbit);
	}

	//bool型で返すよ.
	inline void GetDataArray(bool& Data, int Array_Num)
	{
		stringstream is;
		bool tmp;
		is.str(m_Data.sDataAry[Array_Num]);
		is >> tmp;
		Data = tmp;
		is.clear(stringstream::goodbit);
	}

private:

	HWND m_hWnd;

	//文字列置換.
	string ReplaceString
	(
		string String1   // 置き換え対象.
		, string String2   // 置き換える内容.
		, string String3   // 検索対象.
		, string String4 = "NULL"   // 上の検索対象との間を消す.
	);

	template <typename List>
	void split(const std::string& s, const std::string& delim, List& result);

	//拡張子を出すよ(string型のみ).
	void ExtensionJudgment();

	FileSet	m_Data;
};

#endif