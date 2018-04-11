#include"File.h"
/*********************************************
*/
////ファイル読込み関数.
clsFile::clsFile()
{
	ZeroMemory(this, sizeof(clsFile));
}

clsFile::~clsFile()
{
	m_Data.sDataAry.clear();
	m_Data.sDataAry.shrink_to_fit();
}

//ファイル書き込み.
void clsFile::Write(char* FilePath, char* Content){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	ofs << Content << "\n" << endl;
}

//ファイル書き込み.
//template< class T >
void clsFile::Write(char* FilePath, vector<float> DataAry){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < DataAry.size(); i++)
	{
		ofs << DataAry[i] << "," << endl;
	}
}
void clsFile::Write(char* FilePath, vector<int> DataAry){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < DataAry.size(); i++)
	{
		ofs << DataAry[i] << "," << endl;
	}
}
void clsFile::Write(char* FilePath, vector<char*> DataAry){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < DataAry.size(); i++)
	{
		ofs << DataAry[i] << "," << endl;
	}
}

//ファイル書き込み.
void clsFile::Write(char* FilePath, int NewLine){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < m_Data.sDataAry.size(); i++)
	{
		if ((1 + i) % NewLine == 0)
		{
			ofs << m_Data.sDataAry[i] << "," << endl;
		}
		else{
			ofs << m_Data.sDataAry[i] << ",";
		}
	}
}

void clsFile::Read()
{
	ifstream ifs(m_Data.sPath);
	ifstream tmp;

	vector<string>	TmpArray;
	string buf;
	long i = 0;
	//m_Data.DataAry;
	if (ifs)
	{
		switch (m_Data.en_Extension)
		{
			case Txt:
				while (true)
				{
					////txt用.
					getline(ifs, buf, '_');

					//1行読み取ったので、インクリメントする.
					i++;
					while (buf.find('[') != buf.npos){
						buf = ReplaceString(buf, "", "[", "]");
					}

					split(buf, "\n", TmpArray);
					//NULLチェックして中身がなければ削除.
					for (int i = TmpArray.size() - 1; i >= 0; i--)
					{
						if (TmpArray[i].empty())
						{
							TmpArray.erase(TmpArray.begin() + i);
						}
					}

					// ファイルが終端まで来たら、break 文を実行して.
					// while 文を抜ける.
					if (ifs.eof()){
						break;
					}
				}
				TmpArray.swap(m_Data.sDataAry);
				m_Data.iLineMax = m_Data.sDataAry.size();
				break;
			case Csv:
				while (true)
				{
					//csv用.
					getline(ifs, buf, '_');
					while (buf.find('[') != buf.npos){
						buf = ReplaceString(buf, "", "[", "]");
					}
					while (buf.find('\t') != buf.npos){
						buf = ReplaceString(buf, ",", "\t");
					}
					while (buf.find('\n') != buf.npos){
						buf = ReplaceString(buf, ",", "\n");
					}
					while (buf.find('\"') != buf.npos){
						buf = ReplaceString(buf, ",", "\"");
					}

					split(buf, ",", TmpArray);
					// ファイルが終端まで来たら、break 文を実行して.
					// while 文を抜ける.
					if (ifs.eof()){
						break;
					}
				}
				//NULLチェックして中身がなければ削除.
				for (int i = TmpArray.size() - 1; i >= 0; i--)
				{
					if (TmpArray[i].empty())
					{
						TmpArray.erase(TmpArray.begin() + i);
					}
				}
				//vectorのNULLチェック.
				//if (TmpArray[0].empty())
				//{
				//	TmpArray.erase( TmpArray.begin() );
				//}

				//使っている領域の節約eraseでは領域確保を消せないため.
				TmpArray.swap(m_Data.sDataAry);
				//最大数投げ.
				m_Data.iLineMax = m_Data.sDataAry.size();
				break;
			default:
				//読めとらんよ.
				break;
		}
	}
}

void clsFile::ExtensionJudgment()
{
	size_t PathSize = NULL;
	string tmp;
	m_Data.sPath;
	PathSize = m_Data.sPath.find(".");
	tmp = m_Data.sPath.substr(PathSize + 1, m_Data.sPath.size());

	if (tmp == "txt"){
		m_Data.en_Extension = Txt;
	}
	else if (tmp == "csv"){
		m_Data.en_Extension = Csv;
	}
	else if (tmp == "x"){
		m_Data.en_Extension = x;
	}
	else if (tmp == "X"){
		m_Data.en_Extension = x;
	}
	else if (tmp == "png"){
		m_Data.en_Extension = png;
	}
	else{
		//エラー（対応していない）
	}
}

string clsFile::ReplaceString
(
string String1   // 置き換え対象.
, string String2   // 置き換える内容.
, string String3   // 検索対象.
, string String4   // 上の検索対象との間を消す.
)
{
	if (String4 == "NULL"){
		string::size_type  Pos(String1.find(String3));

		while (Pos != string::npos)
		{
			String1.replace(Pos, String3.length(), String2);
			Pos = String1.find(String3, Pos + String2.length());
		}
	}
	else
	{
		string::size_type  Pos1(String1.find(String3));

		string::size_type  Pos2(String1.find(String4));

		string::size_type  Pos3(String1.find(String4) - String1.find(String3));

		int tmp1;
		int tmp2;
		int tmp3;

		tmp1 = String1.find(String3);
		tmp2 = String1.find(String4);
		tmp3 = tmp2 - tmp1 + 1;
		String1.replace(tmp1, tmp3, String2);
	}
	return String1;
}

template <typename List>
void clsFile::split(const string& s, const string& delim, List& result)
{
	result.clear();

	using string = string;
	string::size_type pos = 0;

	while (pos != string::npos)
	{
		string::size_type p = s.find(delim, pos);
		if (p == string::npos)
		{
			result.push_back(s.substr(pos));
			break;
		}
		else {
			result.push_back(s.substr(pos, p - pos));
		}
		pos = p + delim.size();
	}
}

void clsFile::Init(const char* fileName)
{
	m_Data.sPath = fileName;
	ExtensionJudgment();
}