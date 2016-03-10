#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <windows.h>

using namespace std;

string getExt(const string &filename)
{
    string ext;
    string::size_type index = filename.rfind(".");
    if (index != string::npos)
    {
        ext = filename.substr(index+1);
    }
    return ext;
}

string Replace(const string &src, const string &orig, const string &now)
{
    string tmp;
    string::size_type index = src.find(orig);
    string::size_type first = 0;
    string::size_type src_len = src.length();
    string::size_type orig_len = orig.length();
    while (index != string::npos && index <= src_len)
    {
        tmp += src.substr(first, index-first);
        tmp += now;
        index += orig_len;
        first = index;
        index = src.find(orig, index);
    }
    if (first < src_len)
    {
        tmp += src.substr(first);
    }

    if (tmp.empty())
    {
        return src;
    }
    else
    {
        return tmp;
    }
}

void ProcessFile(const string &filename)
{
    ifstream ifile(filename.c_str());
    if (!ifile)
    {
        return;
    }

    string outfile = filename + ".bak";
    ofstream ofile(outfile.c_str());
    if (!ofile)
    {
        return;
    }

    string line;
    string orig("\t");
    string now("    ");
    while (getline(ifile, line))
    {
        string newLine = Replace(line, orig, now);
        ofile << newLine << endl;
    }

    ifile.close();
    ofile.close();
}

void ProcessDir(string &dirname)
{
    queue<string> dirs;

    int len = dirname.length();
    if (dirname[len-1] == '\\')
    {
        dirname = dirname.substr(0, len-1);
    }

    dirs.push(dirname);

    string findStr;
    while (!dirs.empty())
    {
        cout << "In directory: " << dirs.front() << endl;
        findStr = dirs.front() + "\\*.*";

        WIN32_FIND_DATA wfd;
        HANDLE hFind = FindFirstFile(findStr.c_str(), &wfd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (wfd.cFileName[0] == '.')
                    continue;
                string newDirName = dirs.front();
                newDirName += "\\";
                newDirName += wfd.cFileName;
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    cout << "<DIR>\t" << wfd.cFileName << endl;
                    dirs.push(newDirName);
                }
                else
                {
                    string ext = getExt(wfd.cFileName);
                    if (ext == string("h") || ext == string("cpp"))
                    {
                        cout << "Process " << newDirName << endl;
                        //ProcessFile(newDirName);
                        //DeleteFile(newDirName.c_str());
                    }
                    else if (ext == string("bak"))
                    {
                        string::size_type index = newDirName.rfind(".");
                        if (index != string::npos)
                        {
                            string newName = newDirName.substr(0, index);
                            //cout << "Rename " << newDirName << " To " << newName << endl;
                            rename(newDirName.c_str(), newName.c_str());
                        }
                    }
                }
            }
            while (FindNextFile(hFind, &wfd));
        }

        dirs.pop();
        FindClose(hFind);
    }
}

int main()
{
    string dir("D:\\apdateEx");
    ProcessDir(dir);
    cout << "Hello world!" << endl;
    return 0;
}
