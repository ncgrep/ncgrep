#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <iostream>

#include "files.h"

using namespace std;

vector<string> listdir(string dirname, int lvl, int mode)
{
    DIR* d_fh;
    struct dirent* entry;
    vector<string> result;

    while ( (d_fh = opendir(dirname.c_str())) == NULL) {
        return result;
    }

    while ((entry=readdir(d_fh)) != NULL) {
        /* Don't descend up the tree or include the current directory */
        if(strncmp(entry->d_name, "..", 2) != 0 && strncmp(entry->d_name, ".", 1) != 0) {
            /* If it's a directory print it's name and recurse into it */
            if (entry->d_type == DT_DIR && mode == GET_DIRS_MODE_RECURSIVE) {
                /* Prepend the current directory and recurse */
                vector<string> tmp = listdir(dirname + "/" + entry->d_name, lvl+1, GET_DIRS_MODE_RECURSIVE);
                result.insert(result.end(), tmp.begin(), tmp.end());
            } else {
                char *ext_ch = strrchr(entry->d_name, '.'); 
                string ext;
                if (ext_ch) {
                    string filename(entry->d_name);
                    ext = filename.substr(ext_ch - entry->d_name, filename.length() - (ext_ch - entry->d_name));
                }

                /* Print some leading space depending on the directory level */
                if (ext.compare(".cpp") == 0 || ext.compare(".h") == 0 || ext.compare(".c") == 0 || ext.compare(".php") == 0) {
                    result.push_back(dirname + "/" + entry->d_name);
                }
            }
        }
    }
    closedir(d_fh);

    return result;
}

vector<match_dirs> getdirs(string dirname, int lvl, int group_lvl)
{
    DIR* d_fh;
    struct dirent* entry;
    vector<match_dirs> result;
    match_dirs dir;
    dir.dirname = dirname;

    while( (d_fh = opendir(dirname.c_str())) == NULL) {
        return result;
    }
    if (lvl > group_lvl) {
        return result;
    }
    if (lvl == group_lvl) {
        dir.mode = GET_DIRS_MODE_RECURSIVE;
        result.push_back(dir);
        return result;
    }
    dir.mode = GET_DIRS_MODE_FILE;
    result.push_back(dir);

    while((entry=readdir(d_fh)) != NULL) {
        /* Don't descend up the tree or include the current directory */
        if(strncmp(entry->d_name, "..", 2) != 0 && strncmp(entry->d_name, ".", 1) != 0) {
            if (entry->d_type == DT_DIR) {
                vector<match_dirs> result_tmp  = getdirs(dirname + "/" + entry->d_name, lvl+1, group_lvl);
                result.insert(result.end(), result_tmp.begin(), result_tmp.end());
            }
        }
    }
    closedir(d_fh);

    return result;
}
