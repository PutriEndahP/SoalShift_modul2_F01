# Laporan Resmi Modul 2

1. Putri Endah Puspitasari (05111740000039)<br>
2. Garda Negara Wisnumurti (05111740000153)

## Materi Modul 2 Proses dan Daemon

### 1. Proses

#### 1.1 Proses
Proses merupakan keadaan ketika sebuah program sedang di eksekusi. Setiap proses juga memiliki PID atau Process ID yang merupakan nomor unik yang dapat digunakan untuk berinteraksi dengan proses bersangkutan.

Jalankan `$ ps` untuk menampilkan proses atau `$ ps -u` untuk lebih detailnya.

#### 1.2 Proses ID
##### Process ID (PID)
PID merupakan indentitas unik berupa angka yang digunakan dalam beberapa sistem operasi untuk mengidentifikasi suatu proses. Untuk mendapatkan PID gunakan perintah `System call getpid()`
##### Parent Process ID (PPID)
PPID merupakan induk dari PID dan merupakan creator dari suatu proses. Setiap proses memiliki satu induk proses (PPID). Untuk mendapatkan PPID gunakan perintah `System call getppid()`
##### Parent Process
Parent process merupakan proses yang menciptakan beberapa child process. Proses ini tercipta dengan mengeksekusi fungsi `fork`, lalu hasil pemanggilan fungsi `fork` tersebut menciptakan beberapa child process.
##### Child Process
Child process merupakan proses yang dibuat oleh parent process. Setiap proses dapat membuat banyak child process, namun setiap proses hanya memiliki satu parent process, kecuali untuk proses yang paling pertama, proses tersebut tidak memiliki parent. Proses pertama yang dipanggil unit dalam Linux dimulai oleh kernel pada saat boot dan tidak pernah berhenti.

#### 1.3 Melihat Proses
Melihat proses yang sedang berjalan menggunakan `$ ps -e`
```
  PID  PPID COMMAND
    1     0 /lib/systemd/systemd --system --deserialize 37
    2     0 [kthreadd]
    3     2 [kworker/0:0]
    4     2 [kworker/0:0H]
    6     2 [mm_percpu_wq]
    7     2 [ksoftirqd/0]
    8     2 [rcu_sched]
...........
(long list)
...........
23713  1844 /usr/lib/gnome-terminal/gnome-terminal-server
23767 23713 bash
23806 18599 /usr/lib/firefox/firefox -contentproc -childID 8 -isForBrowser -pref
23914     2 [kworker/u16:0]
23923     2 [kworker/u16:4]
23963 23767 ps -e -o pid,ppid,command
```

#### 1.4 Membunuh Proses
Membunuh proses menggunakan `$ kill {pid}`

Contoh :
```
$ kill 3789
```
terdapat beberapa macam signal yang digunakan dalam command kill, antara lain sebagi berikut :

| Signal name | Signal value  | Effect       |
| ------------|:-------------:| -------------|
| SIGHUP      | 1             | Hangup         |
| SIGINT      | 2             | Interrupt from keyboard  |
| SIGKILL     | 9             | Kill signal   |
| SIGTERM     | 15            | Termination signal
| SIGSTOP     | 17,19,23      | Stop the process

Secara default, `$ kill` menggunakan signal SIGTERM. Untuk menggunakan signal tertentu, gunakan 
```
$ kill -{signal value} {pid}
```
. Contoh, `$ kill -9 3789` untuk menggunakan SIGKILL.

#### 1.5 Membuat Proses
Untuk membuat proses ada dua cara umum yang sering digunakan, `fork` dan `exec`.
##### fork
`fork` digunakan untuk menduplikasi proses. Proses yang baru disebut dengan _child proses_, sedangkan proses yang memanggilnya disebut dengan _parent proses_. Setelah `fork` dipanggil, kita tidaklah tahu proses manakah yang pertama selesai.
Manual: `$ man 2 fork`

Tulislah kode berikut, simpan dalam format c.
```C
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  printf("This is the main program, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());

  child_id = fork();
  if (child_id != 0) {
    printf("This is the parent process, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());
  } else {
    printf("This is the child process, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());
  }

  return 0;
}
```
Coba jalankan dan perhatikan hasilnya. Semua perintah `printf` jalan, bagaimana bisa? Perhatikan PID, Child's ID, dan Parent ID setiap kalimat output.
```C
+-------------------------+
|   Parent Process        |
+-------------------------+
|   int main() {          |
|     pid_t child_id;     |
|                         |
|     pid = getpid();     |
|     ppid = getppid();   |
|                         |
|-->  child_id = fork();  |
+-------------------------+
|    pid = 20             |
|    child_id = undefined |
|    ppid = 10            |
+-------------------------+
         |\
         | \----------------------------------\
         |                                     |
         V                                     V
+-------------------------+        +-------------------------+
|   Parent Process        |        |     Child Process       |
+-------------------------+        +-------------------------+
|-->                      |        |-->                      |
|     pid = getpid();     |        |     pid = getpid();     |
|     ppid = getppid();   |        |     ppid = getppid();   |
|   }                     |        |   }                     |
+-------------------------+        +-------------------------+
|    pid = 20             |        |    pid = 23             |
|    child_id = 23        |        |    child_id = 0         |
|    ppid = 10            |        |    ppid = 20            |
+-------------------------+        +-------------------------+
```
Perhatikan, bahwa;
- `pid` Parent Process        == `ppid` Child 
- `child_id` Parent Process   == `pid` Child Process

##### exec
`exec` adalah function yang digunakan untuk menjalankan program baru dan mengganti program yang sedang berlangsung. `exec` adalah program family yang memiliki berbagai fungsi variasi, yaitu `execvp`, `execlp`, `execv`, dan lain lain.

Manual: `$ man 3 exec`

Contoh:
```C
#include <stdio.h>
#include <unistd.h>

int main () {
  
  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"list", "-l", "/home/[user]/", NULL};
  
  execv("/bin/ls", argv);

  printf("This line will not be executed\n");

  return 0;
}
```

##### fork and exec
__Permasalahan:__
Bagaimana cara menjalankan dua proses dalam satu program?

__Contoh Permasalahan:__
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c`?

Maka, bagaimana cara menjalankan `mkdir` __dan__ `touch` dalam satu program?

__Solusi:__
Gunakan `fork` dan `exec`!

Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Jalankan `exec` yang memanggil `touch` pada parent process

Visualisasi
```
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V                         
+--------+                     +--------+
| pid=7  |    forks            | pid=22 |
| ppid=4 | ------------------> | ppid=7 |
| bash   |                     | bash   |
+--------+                     +--------+
    |                              |
    | calls exec to run touch      | calls exec to run mkdir
    |                              |
    V                              V
```

Contoh:
```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    
    char *argv[] = {"mkdir", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent
  
    char *argv[] = {"touch", "sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
```

##### wait
`wait` adalah function yang digunakan untuk mendapatkan informasi ketika child proses berganti _state_-nya. Pergantian state dapat berupa _termination_, _resume_, atau _stop_. Pada modul ini, kita hanya menggunakan `wait` untuk menangkap state _termination_.

Fungsi `wait` pada parent process juga berguna untuk menangkap exit status dari child.

__Permasalahan:__  
Bagaimana cara membuat program yang menjalankan suatu proses tanpa menghentikan program?

__Contoh permasalahan:__  
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c` __di dalamnya__?

Maka, bagaimana cara menjalankan `mkdir` __lalu__ menjalankan `touch` dalam satu program?

__Solusi:__  
Gunakan `fork`, `exec`, dan `wait`!

Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Buat parent process menunggu (`wait`) hingga proses pada child selesai
4. Setelah child selesai, jalankan `exec` yang memanggil `touch` pada parent

Visualisasi
```
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V
+--------+             +--------+
| pid=7  |    forks    | pid=22 |
| ppid=4 | ----------> | ppid=7 |
| bash   |             | bash   |
+--------+             +--------+
    |                      |
    | waits for pid 22     | calls exec to run mkdir
    |                      V
    |                  +--------+
    |                  | pid=22 |
    |                  | ppid=7 |
    |                  | ls     |
    V                  +--------+
+--------+                 |
| pid=7  |                 | exits
| ppid=4 | <---------------+
| bash   |
+--------+
    |
    | calls exec to run touch
    |
    V
```
Contoh:
```C
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id == 0) {
    // this is child

    char *argv[4] = {"mkdir", "-p", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent

    // the parent waits for all the child processes
    while ((wait(&status)) > 0);

    char *argv[3] = {"touch", "sample-dir/sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
```
##### system
Ketika [system](http://man7.org/linux/man-pages/man3/system.3.html) dijalankan, program hanya memanggil _external command_ (kalau di Ubuntu berupa program `/bin/bash`). Penggunaan `system` sangat tergantung pada environment. 

Contoh, ketika user menjalankan `system("ls -l")`, ini sama seperti menjalankan `$ ls -l` pada bash.

Meskipun mudah digunakan, tidak disarankan menggunakan fungsi `system` karena beberapa asalan:
```
$ man system

  ....
  NOTES
  system() provides simplicity and convenience: it handles  all
  of  the details of calling fork(2), execl(3), and waitpid(2),
  as well as the necessary manipulations of signals.

  Do not use system() from a program with set-user-ID  or  set-
  group-ID privileges, because strange values for some environ‐
  ment variables might be used  to  subvert  system  integrity.
  ...
```

Contoh:
```C
#include <stdlib.h>

int main() {
  int return_value;
  return_value = system("ls -l /");
  return return_value;
}
```

Hasil
```
total 156
drwxr-xr-x   2 root root  4096 Sep 14 06:35 bin
drwxr-xr-x   4 root root  4096 Sep 20 00:24 boot
drwxrwxr-x   2 root root  4096 Agu 14 14:05 cdrom
drwxr-xr-x   3 root root  4096 Sep 12 19:11 data
(long list)
```

#### 1.6 Jenis-Jenis Proses
##### Zombie Process
Zombie Process terjadi karena adaanya child process yang di exit namun parrent processnya tidak tahu bahwa child process tersebut telah di terminate, misalnya disebabkan karena putusnya network. Sehingga parent process tidak merelease process yang masih digunakan oleh child process tersebut walaupun process tersebut sudah mati.
##### Orphan Process
Orphan Process adalah sebuah proses yang ada dalam komputer dimana parent process telah selesai atau berhenti bekerja namun proses anak sendiri tetap berjalan.
##### Daemon Process
Daemon Process adalah sebuah proses yang bekerja pada background karena proses ini tidak memiliki terminal pengontrol. Dalam sistem operasi Windows biasanya lebih dikenal dengan sebutan service. Daemon adalah sebuah proses yang didesain supaya proses tersebut tidak mendapatkan intervensi dari user.

### 2. Daemon
#### 2.1 Daemon
Daemon adalah proses yang berjalan di balik layar (background) dan tidak berinteraksi langsung dengan user melalui standard input/output.
#### 2.2 Proses Pembuatan Daemon
##### 1.  Fork Parent Process dan penghentian Parent Process 

Langkah pertama adalah men*spawn* proses menjadi induk dan anak dengan melakukan *forking*,  kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.

```c
pid_t pid, sid;
pid=fork();
if (pid < 0){
    exit(EXIT_FAILURE);
}
if (pid > 0){
    //catat PIP proses anak ke sebuah file
    exit(EXIT_SUCCESS);
}   //jika pembuatan proses berhasil, maka parent proses akan dimatikan
```

##### 2. Mengubah mode file menggunakan `umask(0)`

Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().

##### 3. Membuat Unique Session ID (SID)

Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().

```c
sid = setsid();
if(sid<0){
    exit(EXIT_FAILURE);
}  
```

##### 4. Mengubah Directory Kerja

Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi `chdir()`. Fungsi `chdir()` mengembalikan nilai -1 jika gagal.

```c
if((chdir("/"))<0) {
    exit(EXIT_FAILURE);
}
```

##### 5. Menutup File Descriptor Standar

Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().

```c
close(STDIN_FILENO);
close(STDERR_FILENO);
clode(STDOUT_FILENO);
```

##### 6. Membuat Loop utama 

Daemon bekerja dalam jangka waktu tertentu, sehingga diperlukan sebuah looping.

```c
while(1){
    sleep(5)
}
exit(EXIT_SUCCES);
```
#### 2.3 Contoh Implementasi Daemon
```C
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}
```

### Appendix
#### Soal Latihan
##### Latihan 1
Modifikasi code [soal1](https://github.com/yoshimaputri/sisop-modul-2/blob/master/soal1.c) agar output nya menjadi huruf abjad urut dari A sampai Z, tanpa menghapus fungsi yang sudah ada.
##### Latihan 2
Buatlah program yang bisa membuat folder "anak" yang berisi salinan file [warisan.txt](https://github.com/yoshimaputri/sisop-modul-2/blob/master/warisan.txt).   
Hint: gunakan `fork`, `exec`, dan `wait`.
##### Latihan 3
Buatlah sebuah daemon yang dapat melakukan backup isi dari file **sampah.txt** yang disimpan dalam file **log.log** lalu menghapus file **sampah.txt** tersebut.
Tidak diperbolehkan menggunakan `exec` dan `system`.
#### References
https://notes.shichao.io/apue/

http://www.linuxzasve.com/preuzimanje/TLCL-09.12.pdf

## SoalShift_modul2_F01

### Soal 1
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.
```javascript
		
		#include <sys/types.h>
		#include <sys/stat.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <fcntl.h>
		#include <errno.h>
		#include <unistd.h>
		#include <syslog.h>
		#include <string.h>
		#include <dirent.h>

		int main() {
			pid_t pid, sid;
			pid = fork();

			if (pid < 0){
				exit(EXIT_FAILURE);
			}

			if (pid > 0){
				exit(EXIT_SUCCESS);
			}

			umask(0);

			sid = setsid();

			if (sid < 0) {
				exit(EXIT_FAILURE);
			}

			if ((chdir("/home/fdh/Downloads/modul2/soal1")) < 0) {
				exit(EXIT_FAILURE);
			}

			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);

			while(1) {
			DIR *direktory;
			struct dirent *file;
			direktory = opendir("/home/garda/documents");
			if(direktory){
			while((file = readdir(direktory)) != NULL){

				if(strstr(&file->d_name[strlen(file->d_name)-5], ".png")){
				 char namafile[300]="";
				 strncpy(namafile, file->d_name,strlen(file->d_name)-4 );
				 strcat(namafile, "_grey.png");
				 rename(strcat(".", file->d_name), 
				strcat("/gambar", namafile));
			      }
			}
			closedir(direktory);
			      }
				sleep(10);
			}
			exit(EXIT_SUCCESS);

		}
```

Jadi di line pertama terdapat DIR untuk membuka directory. Strstr untuk memeriksa ekstensi png. Strcpy untuk menyisipkan grey.png. Rename untuk merename nama sesuai format soal. 


### Soal 2
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

	Langkah awal yang dilakukan adalah membuat directori hatiku pada /home/putri/sisop/modul2_soalshift yang berisi elen.ku. untuk menjalankan program yang diminta kami membuat file soal2.c yang berisi source code seperti berikut ini :

```javascript
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
        struct  stat info_file; //nama statnya adl info_file 
        char tempat[255] = "/home/putri/sisop/modul2_soalshift/hatiku/elen.ku";
        stat(tempat, &info_file); //mendapatkan informasi dari tempat di simpan di info_file 

        struct passwd *pwd = getpwuid(info_file.st_uid); //st.uid unt mendapatkan userID owner -> man 2 stat
        struct group *grup = getgrgid(info_file.st_gid); //st.gid unt mendapatkan group ID owner

        char own[100] = "www-data"; //string www-data disimpan di variabel own

        int us = strcmp(pwd->pw_name, own); //membandingkan yg ada di pwd dgn pw_name(membuka nama pw, dan own
        int grp = strcmp(grup->gr_name, own); //membandingkan yg ada di grp dgn gr_name(nama grup) dan own

        if(us==0 && grp==0) // jika us dan gr sama maka dihapus file nya
        {
                remove(tempat);
        }
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
  return 0;
}
```

	Berdasarkan source code diatas, kami menggunakan fork and exec dan main program nya berarti yang pertama kami membuat stat bernama info file. Stat yaitu struct yang berisi informasi tentang sesuatu yang kita minta. Kemudian kita deklarasikan variabel tempat dengan tipe data char yang berisi path dari file elen.ku. stat(tempat, &info_file), fungsi tersebut berarti kita akan mendapatkan informasi dati variabel tempat dan akan disimpan ke info_file.

	Yang kedua kita membuat struct fungsi passwd bertipe data pointer bernama pwd yaitu untuk mendapatkan user ID  owner dari info_file dengan fungsi st_uid yang kita bisa dapatkan dari man 2 stat.

	Yang ketika buat struct fungsi group bertipe data pointer bernama grup yaitu untuk mendapatkan group ID owner dari info_file dengan fungsi st_gid yang kita bisa dapatkan dari man 2 stat.

	Kemudian kita deklarasikan charakter www-data dengan nama own bertipe data character [100]. Kemudian buatlah variabel bernama us dan grp yang isinya membandingkan atau meng-compare yang ada di isinya pointer pwd dan grup dengan pw_name = nama password, gr_name = group name dan own.

	Jadi syarat agar file terhapus kan jika file tersebut owner dan grupnya bernama www-data. Maka jika us = grp sudah menjadi www-data, maka di hapus file tadi yang kita beri nama tempat.

	Sleep diatur 3 detik karena permintaan dari soal.

### Soal 3
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
Gunakan fork dan exec.
Gunakan minimal 3 proses yang diakhiri dengan exec.
Gunakan pipe
Pastikan file daftar.txt dapat diakses dari text editor

 Pada soal nomer 3 ini kami diminta untuk mengekstrak file campur2.zip yang kedua kita harus menyimpan daftar file dari folder "campur2" yang memiliki ekstensi .txt ke dalam file daftar.txt. Pertama kami membuat file soal3.c yang berisi source code berikut ini :

```javascript
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id_satu, child_id_dua, child_id_tiga;
  int pipe_satu[2], pipe_dua[2];

  pipe(pipe_satu);
  pipe(pipe_dua);

  child_id_satu = fork();
  
  if (child_id_satu < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id_satu == 0) {
    // this is child
    
    char *zip[] = {"unzip", "/home/putri/sisop/modul2_soalshift/campur2.zip", NULL};
    execv(zip[], zip);
  } else {
        while(wait(NULL) > 0);

        child_id_dua = fork();

if (child_id_dua < 0) {
   exit(EXIT_FAILURE);
  }

if (child_id_dua == 0) 
{
        dup2(pipesatu[2], STDOUT_FILENO);
        close(pipesatu[2]);
        close(pipedua[2]);

        char *ls[] = {"ls", "/home/putri/sisop/modul2_soalshift/campur2", NULL};
        execvp(ls[0], ls);
}
else {
        while(wait(NULL) > 0);

        child_id_tiga == fork();

if(child_id_tiga < 0) {
   exit(EXIT_FAILURE);
}
if(child_id_tiga == 0) 
{

}
}
  }
}
```
 Berdasrkan source code diatas yang merupakan source code yang belum selesai berarti kita punya 3 proses disini bersama tiga child_id yaitu child_id_satu, child_id_dua, dan child_id_tiga dan terdiri dari dua pipe yaitu pipe_satu dan pipe dua. Awalnya kita harus mendeklarasikan terlebih dahulu child dan pipe pipe nya.

``` char *zip[] = {"unzip", "/home/putri/sisop/modul2_soalshift/campur2.zip", NULL};
    execv(zip[], zip);
```
Pada proses pertama kita meng unzip file dengan source code diatas.
Proses kedua dengan menggunakan child_id_dua dan membutuhkan pipe dikarenakan output dari proses pertama dijadikan input untuk proses kedua. maka dari itu kita harus menghubungkan STDOUT dengan PIPE dengan source code dibawah ini :

``` javascript
  dup2(pipesatu[2], STDOUT_FILENO);
```

### Soal 4
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang0-30detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 

•	dilarang menggunakan crontab

•	Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

	Langkah awal yang dilakukan adalah membuat directori makanan yang berada pada /home/[user]/Documents/makanan yang berisi makan_enak.txt yang berada pada file soal4.cpp yang berisi source code seperti berikut ini :

```javascript
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <cstring>
#include <iostream>

using namespace std;

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

    // main program here

        int urutan_file = 1;
        while(1)
        {
                struct stat makan_enak;
                stat("/home/putri/Documents/makanan/makan_enak.txt",&makan_enak);
//              cout << "diff " << difftime(time(NULL),makan_enak.st_atime) << endl; 

                //membandingkan waktu program berjalan dgn last time program
                //st.atime untuk waktu program terakhir dibuka(last time)
                if (difftime(time(NULL),makan_enak.st_atime) <= 30)//time null untuk waktu program saat itu berjalan
                {
//                      cout << "-----------" << urutan_file << endl;
                        string makan_sehat = "/home/putri/Documents/makanan/makan_sehat";
                        makan_sehat = makan_sehat + to_string(urutan_file) + ".txt";

                        //c_str digunakan meng konversi string ke array of char
                        //awalnya kan dibaca berupa pointer terus dibaca isi pointernya
                        //char sehat[100]={*(makan_sehat.c_str())};

                        //membuat file makansehat agar bisa dilihat
                        FILE *makansehat = fopen((char*)makan_sehat.c_str(),"w");
                        //FILE *makansehat = fopen(sehat,"w");
                        fclose(makansehat);
                        urutan_file++;
                }
        sleep(5);
        }
 
  exit(EXIT_SUCCESS);
}
```

	Dari source code diatas, kita menggunakan fork and exec yang isi dari main programnya yaitu, awalnya meng-inisialisasikan nilai urutan yang diberi nama urutan_file = 1. Kemudian membuat stat yang bernama makan_enak. Stat merupakan struct yang berfungsi untuk mendapatkan suatu informasi tertentu. Stat tersebut mendapatkan informasi dari file makan_enak.txt kemudian di simpan ke &makan_enak.

	Untuk membandingkan atau mencari tahu perbedaan waktunya, apakah kurang dari lebih dari atau sama dengan 30 detik menggunakan fungsi difftime, yaitu dengan cara mengurangkan waktu program itu berjalan dengan waktu program terakhir dibuka. Jika hasil difftime tersebut <= 30, maka akan dibuatkan file bernama makan_sehar#.txt dimana # sendiri termasuk integer dari 1-tak hingga.

	Path makan_sehat di letakkan di variabel bernama makan_sehat, kemudian kita butuh untuk mengubah integer yang tadinya 1,2,3 dsb menjadi string yang terdiri dari makan_sehat1.txt, maka kita menggunakan concat yaitu untuk menggabungkan nya menjadi  string. 

	makan_sehat = makan_sehat + to_string(urutan_file) + ".txt" berarti, dari string makan_sehat sendiri adalah string makan_sehat + perubahan integer menjadi string + “.txt” maka nantinya file tersebut akan menjadi contoh makan_sehat1.txt begitu seterusnya.

	Kemudian kita membuat file untuk makan_sehat tersebut agar dapat dilihat yaitu file pointer makansehat yang akan membuka file makan_sehat yang sudah di konversi menjadi array of chat dengan perintah c_str kemudian di write file tersebut, jika sudah selesai prosesnya maka akan menutup file. Dan urutan_file akan increment terus menerus mulai dari 1. Untuk sleep sendiri menggunakan sleep 5 karena di soal diperintahkan setiap 5 detik sekali di cek nya.

### Soal 5
Kerjakan poin a dan b di bawah:
a.	Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
•	Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
•	Per menit memasukkan log#.log ke dalam folder tersebut
‘#’ : increment per menit. Mulai dari 1
b.	Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

	Program (a) ini menggunakan daemon. Langkah awal yang dilakukan adalah membuat file soal5daemonyang berisi source code seperti berikut ini :

```javascript
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>


int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
     //ambil waktu program sekarang
        time_t waktuwaktu;
        time(&waktuwaktu);

        struct tm *waktu;
        waktu = localtime(&waktuwaktu); //mengkonversi raw time di time_t biar jadi kepisah pisah hari, jam, bulan
        //1900 = 0 makanya + 1900 
        char fungsi_memisah[150]; //tm_sec dll didapat dari struct tm 
        sprintf(fungsi_memisah, "%d:%d:%d-%d:%d", waktu->tm_mday, waktu->tm_mon+1, waktu->tm_year+1900, waktu->tm_hour, waktu->tm_min); // untuk misah2 waktu disimpan di fungsi_memisah, cari struct tm
        //sprintf : nge print yang di print kemudian hasil print nya di simpan di yg kiri
        printf("%s", fungsi_memisah);
        char putri[150];
        sprintf(putri, "/home/putri/sisop/modul2_soalshift/%s", fungsi_memisah); //letak dimana mkdir ini akan dibuat
        mkdir(putri, 0777); //777 biar bisa akses semua (rwx)

        int incr;
        for(incr=1; incr<30; incr++)
        {
                FILE *file_syslog; //membuat file baru untuk syslog
                FILE *file_ygdibuat; //membuat file baru yang akan dibuat
                file_syslog = fopen("/var/log/syslog", "r");
                char nama_filebaru[255];
                sprintf(nama_filebaru, "/home/putri/sisop/modul2_soalshift/%s/log%d.log", fungsi_memisah, incr); //hasil print nya disimpan di nama_filebaru
                file_ygdibuat = fopen(nama_filebaru, "w");

                //pindah seluruh file ke log#.log
                char pindah; 
                while(fscanf(file_syslog, "%c", &pindah) != EOF)
                {
                        fprintf(file_ygdibuat, "%c", pindah);
                }
                fclose(file_syslog);
                fclose(file_ygdibuat);
                sleep(60);
        }

  }
  
  exit(EXIT_SUCCESS);
}
```

	Berdasarkan sourc code diatas, kita deklarasikan waktuwaktu dengan tipe data time_t, isi dari waktuwaktu tesebut disimpan di variabel time. Kemudian kita membuat struct tm bernama waktu bertipe data pointer. waktu = localtime(&waktuwaktu) berfungsi untuk mengkonversi waktu di time_t biar bisa kepisah pisah hari, jam, bulan.

	Selanjutnya buatlah array of character bernama fungsi_memisah. Kemudian simpan hari, bulan, tahun, dan minggu didalam array of char tadi lalu di print fungsi_memisah nya dalam bentuk string. Buat variabel character yang saya berinama putri. Variabel putri ini untuk menyimpan dari yang fungsi_memisah agar tersimpan path dimana file baru itu di buat. Buat folder bernama putri dengan chmod 777 biar bisa mengakses semua (read,write,execute).

	Deklarasi variabel bernama incr bertipe data integer. Buat perulangan untuk membuat folder setiap 30 menit. Dan di dalam folder tersebut buatlah file syslog dan file baru yang akan terbuat. Print file log#.log yang akan disimpan di nama_file baru kemudian buka file tersebut.

	Untuk memindahkan seluruh file log#.log deklarasikan pindah dengan tipe data char. Yang terakhir cek isi file syslog yang sudah kita buat kemudian isinya di simpan di suatu variabel file_ygdibuat. Kemudian tutup semua file yang kita buat. Digunakan sleep(60) dalam satuan detik karena perintah soal adalah membuat file setiap menit.

	Kemudian untuk perintah (b) yaitu membuat perintah kill untuk program tersebut. Printah kill tersebut saya simpan di file soal5b.c yang berisi source code seperti berikut ini :

```javascript
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

int main()
{
        int total_length = 10;
        char line[total_length];
        FILE * command = popen("pidof soal5daemon","r");

        fgets(line,total_length,command);

        pid_t pid = strtoul(line,NULL,10);
        kill(pid,SIGKILL);
        pclose(command);

```

	Source code diatas berarti kami membuat variabel yang bernama total_leng yang berarti panjang nya ada 10 integer. kemudian 
total_length dimasukkan kedalam character bernama line. 

``` FILE * command = popen("pidof soal5daemon","r");``` berarti kita membuat file command yang perintahnya membuka pid dari soal5daemon.

	Kemudian dapatkan command, line dan total_length dalam satu baris. Jika proses sudah di kill maka akan muncul pesan killed yang berarti proses ID tersebut sudah di kill.







