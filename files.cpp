#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <iostream>

using namespace std;

vector<string> listdir(string dirname, int lvl)
{
    int i;
    DIR* d_fh;
    struct dirent* entry;
    vector<string> result;

    while( (d_fh = opendir(dirname.c_str())) == NULL) {
        cerr<<"Couldn't open directory:"<<dirname<<endl;
        return result;
    }

    while((entry=readdir(d_fh)) != NULL) {
        /* Don't descend up the tree or include the current directory */
        if(strncmp(entry->d_name, "..", 2) != 0 &&
                strncmp(entry->d_name, ".", 1) != 0) {

            /* If it's a directory print it's name and recurse into it */
            if (entry->d_type == DT_DIR) {
                for(i=0; i < 2*lvl; i++) {
                    //cout<<" ";
                }
                //cout<<entry->d_name<<"(d)"<<endl;

                /* Prepend the current directory and recurse */
                vector<string> tmp = listdir(dirname + "/" + entry->d_name, lvl+1);
                result.insert(result.end(), tmp.begin(), tmp.end());
            } else {
                char *ext_ch = strrchr(entry->d_name, '.'); 
                string ext;
                if (ext_ch) {
                    string filename(entry->d_name);
                    ext = filename.substr(ext_ch - entry->d_name, filename.length() - (ext_ch - entry->d_name));
                }

                /* Print some leading space depending on the directory level */
                for(i=0; i < 2*lvl; i++) {
                    //cout<<" ";
                }

                if (ext.compare(".cpp") == 0 || ext.compare(".h") == 0 || ext.compare(".c") == 0 || ext.compare(".php") == 0) {
                    result.push_back(dirname + "/" + entry->d_name);
                }
                //cout<<dirname<<"/"<<entry->d_name<<endl;
            }
        }
    }
    closedir(d_fh);

    return result;
}
