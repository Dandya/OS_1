#!/usr/bin/python3

import fuse
from fuse import Fuse
import stat, errno, json
import requests as req

fuse.fuse_python_api = (0, 2)

class default_stat(fuse.Stat):
    """ Класс для инициализации нулевых атрибутов """
    def __init__(self):
        self.st_mode = 0
        self.st_ino = 0
        self.st_dev = 0
        self.st_nlink = 0
        self.st_uid = 0
        self.st_gid = 0
        self.st_size = 0
        self.st_atime = 0
        self.st_mtime = 0
        self.st_ctime = 0

class simple_fuse(Fuse):
    def getentry(self, path):
        response = req.get("http://127.0.0.1:8000/jlibrary" + path)
        data = json.loads(response.content)
        if (response.status_code != 200) or (data == '404'):
            return None
        return data

    def getattr(self, path):
        if path in ['.','..'] :
            st = default_stat()
            st.st_mode = stat.S_IFDIR | 0o755
            st.st_nlink = 2
            return st
        # else
        entry = self.getentry(path)
        if not entry: 
            return -errno.ENOENT
        st = default_stat()
        if 'items' in entry:
            st.st_mode = stat.S_IFDIR | 0o755
            st.st_nlink = 2
        else:
            st.st_mode = stat.S_IFREG | 0o755
            st.st_nlink = 1
            st.st_size = 0 if 'work' not in entry else len(str.encode(entry['work'])) # for support russian letters
        return st

    def open(self, path, flags):
        if (not self.getentry(path)):
            return -errno.ENOENT
    
    def read(self, path, size, offset):
        entry = self.getentry(path)
        if (not entry):
            return -errno.ENOENT
        data = '' if 'work' not in entry else entry['work']
        return data.encode('utf-8')
    
    def readdir(self, path, offset):
        entry = self.getentry(path)
        if (not entry):
            return -errno.ENOENT
        if entry['items'] != []:
            for d in entry['items']:
                if d == '':
                    continue
                yield fuse.Direntry(d)
        else:
            return fuse.Direntry("\n")

    def mkdir(self, path, mode_t):
        path = path.rstrip('/')
        path = path.split('/')
        req.post("http://127.0.0.1:8000/", data='{"author_name": "'+ path[len(path)-1].encode("utf-8").decode("latin-1")
                 +'","work_name": "", "byte": 0, "offset": 0}') 

    def create(self, path, mode_t, dev_t):
        path = path.rstrip('/')
        path = path.split('/')
        req.post(f"http://127.0.0.1:8000/{path[len(path)-2]}", data='{"author_name": "'+ path[len(path)-2].encode("utf-8").decode("latin-1") + '", "work_name": "'+ path[len(path)-1].encode("utf-8").decode("latin-1") +'", "byte": 0, "offset": 0}')
    
    def write(self, path, buf, offset):
        path = path.rstrip('/')
        path = path.split('/')
        req.put(f"http://127.0.0.1:8000/{path[len(path)-2]}/{path[len(path)-1]}", data='{"author_name": "'+
                path[len(path)-2].encode("utf-8").decode("latin-1") + '", "work_name": "'+
                path[len(path)-1].encode("utf-8").decode("latin-1") +'", "byte": '+str(buf[0])+', "offset": '+str(offset)+'}')
        return 1
    
    def unlink(self, path):
        path = path.rstrip('/')
        path = path.split('/')
        req.post(f"http://127.0.0.1:8000/{path[len(path)-2]}/{path[len(path)-1]}/del", data='{"author_name": "'+ path[len(path)-2].encode("utf-8").decode("latin-1") + '", "work_name": "'+ path[len(path)-1].encode("utf-8").decode("latin-1") +'", "byte": 0, "offset": 0}')
        return 0
    
    def rmdir(self, path):
        path = path.rstrip('/')
        path = path.split('/')
        req.post(f"http://127.0.0.1:8000/{path[len(path)-1]}/del", data='{"author_name": "'+ path[len(path)-1].encode("utf-8").decode("latin-1") + '", "work_name": "", "byte": 0, "offset": 0}')
        return 0

    def rename(self, path, newpath):
        req.post(f"http://127.0.0.1:8000/print/1")
        self.create(newpath, 0, 1)
        req.post(f"http://127.0.0.1:8000/print/2")
        data = bytearray(self.read(path, 0, 0))
        req.post(f"http://127.0.0.1:8000/print/3")
        for offset in range(len(data)):
            self.write(newpath, data, offset)
            data.pop(0)
        self.unlink(path)
        return 0

# fuctions, which are so useful  
    def truncate(self, path, length):
        return 0

    def flush(self, path):
        return 0

    def release(self, path, fh):
        return 0
 
    def fsync(self, path, fh):  
        return 0

    def close(self, path):
        return 0
    
if __name__ == '__main__':
    driver = simple_fuse(
        version = "%prog " + fuse.__version__,
        usage=Fuse.fusage,dash_s_do="setsingle"
    )
    driver.parse(errex=1) 
    driver.main()
